#include "catalog.h"
#include "query.h"
#include "sort.h"
#include "index.h"
#include <cstring>

/* Consider using Operators::matchRec() defined in join.cpp
 * to compare records when joining the relations */

//get tuple size in bytes
Status get_maxItems(string relName, int& maxItem){
    unsigned int unpin_page = bufMgr->numUnpinnedPages();
    Status status;
    RelDesc rel;
    status = relCat->getInfo(relName, rel);
    if (status != OK) return status;
    AttrDesc* attrs;
    status = attrCat->getRelInfo(relName, rel.attrCnt, attrs);
    if (status != OK) return status;
    int tuplelen = 0;
    for (int i = 0; i<rel.attrCnt; i++){
        tuplelen += attrs[i].attrLen;
    }
    maxItem = ((int)(0.8 * unpin_page))*PAGESIZE /tuplelen;
    return OK;
}
  
Status Operators::SMJ(const string& result,           // Output relation name
                      const int projCnt,              // Number of attributes in the projection
                      const AttrDesc attrDescArray[], // Projection list (as AttrDesc)
                      const AttrDesc& attrDesc1,      // The left attribute in the join predicate
                      const Operator op,              // Predicate operator
                      const AttrDesc& attrDesc2,      // The left attribute in the join predicate
                      const int reclen)               // The length of a tuple in the result relation
{
    cout << "Algorithm: SM Join" << endl;

    /* Your solution goes here */
    Status status;
    
    RID rid_old1;
    Record rec_old1;
    int maxitem1;
    status = get_maxItems(string(attrDesc1.relName), maxitem1);
    if (status != OK) return status;
    SortedFile table1 (attrDesc1.relName, attrDesc1.attrOffset, attrDesc1.attrLen,
            (Datatype)attrDesc1.attrType, maxitem1, status);
    if (status != OK) return status;

    RID rid_old2;
    Record rec_old2;
    int maxitem2;
    status = get_maxItems(string(attrDesc2.relName), maxitem2);
    if (status != OK) return status;
    SortedFile table2 (attrDesc2.relName, attrDesc2.attrOffset, attrDesc2.attrLen,
            (Datatype)attrDesc2.attrType, maxitem2, status);
    if (status != OK) return status;

    RID rid_new;
    Record rec_new;
    rec_new.length = reclen;
    rec_new.data = operator new (reclen);
    HeapFile new_hf(result, status);
    if (status != OK) return status;

    bool matched = false;
    bool end_loop = false;
    Record prev_rec1;
    int diff; 
    status = table1.next(rec_old1);
    if (status == FILEEOF) return OK;
    else if (status != OK) return status;
    status = table2.next(rec_old2);
    if (status == FILEEOF) return OK;
    else if (status != OK) return status;

    while (!end_loop){
        matched = false;
        int a, b;
        memcpy(&a, rec_old1.data+attrDesc1.attrOffset, attrDesc2.attrLen);
        memcpy(&b, rec_old2.data+attrDesc2.attrOffset, attrDesc2.attrLen);
        diff = matchRec(rec_old1, rec_old2, attrDesc1, attrDesc2); 
        if (diff < 0){
            //next tuple in rel1
            status = table1.next(rec_old1);
            if (status == FILEEOF){
                end_loop = true;
            } else if (status != OK){
                return status;
            }
        } else if (diff > 0){
            //next tuple in rel2
            status = table2.next(rec_old2);
            if (status == FILEEOF){
                end_loop = true;
            } else if (status != OK){
                return status;
            }
        }
        while ((diff == 0)&&(!end_loop)){ //match
            if (!matched){
                table2.setMark();
                matched = true;
            }
            //insert new tuple
            int offset = 0;
            for (int i=0; i<projCnt; i++){
                if (!strcmp(attrDescArray[i].relName, attrDesc1.relName)){  //in rel1
                    memcpy(rec_new.data+offset, rec_old1.data+attrDescArray[i].attrOffset,
                            attrDescArray[i].attrLen);
                    offset += attrDescArray[i].attrLen;
                } else {
                    memcpy(rec_new.data+offset, rec_old2.data+attrDescArray[i].attrOffset,
                            attrDescArray[i].attrLen);
                    offset += attrDescArray[i].attrLen;
                }
            }
            status = new_hf.insertRecord(rec_new, rid_new);
            if (status != OK) return status;
            //get next tuple
            status = table2.next(rec_old2);
            if (status == FILEEOF){ //if end of inner loop
                memcpy (&prev_rec1, &rec_old1, sizeof(Record));  //copy the previous rec in rel1
                status  = table1.next(rec_old1);
                if (status == FILEEOF){
                    end_loop = true;
                } else if (status != OK){
                    return status;
                }
                if (matchRec(prev_rec1, rec_old1, attrDesc1, attrDesc1)==0){  //is equal
                    table2.gotoMark();
                    table2.next(rec_old2);
                } else {
                    end_loop = true;
                }
            } else if (status != OK){
                return status;
            }
            diff = matchRec(rec_old1, rec_old2, attrDesc1, attrDesc2);
        }
        //not a match
        /*if (!matched){
            //next tuple in rel2
            status = table2.next(rec_old2);
            if (status == FILEEOF){
                end_loop = true;
            } else if (status != OK){
                return status;
            }
        } else {
            //next tuple in rel1
            memcpy(&prev_rec1, &rec_old1, sizeof(Record)); //copy previous one
            status = table1.next(rec_old1);
            if (status == FILEEOF){
                end_loop = true;
            } else if (status != OK){
                return status;
            }
            if (matchRec(prev_rec1, rec_old1, attrDesc1, attrDesc1)==0){ //next tuple is same
                table2.gotoMark();
                table2.next(rec_old2);
            }
        }*/
    }
    return OK;
}

