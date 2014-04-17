#include "catalog.h"
#include "query.h"
#include "sort.h"
#include "index.h"


/* 
 * Indexed nested loop evaluates joins with an index on the 
 * inner/right relation (attrDesc2)
 */

Status Operators::INL(const string& result,           // Name of the output relation
                      const int projCnt,              // Number of attributes in the projection
                      const AttrDesc attrDescArray[], // The projection list (as AttrDesc)
                      const AttrDesc& attrDesc1,      // The left attribute in the join predicate
                      const Operator op,              // Predicate operator
                      const AttrDesc& attrDesc2,      // The left attribute in the join predicate
                      const int reclen)               // Length of a tuple in the output relation
{
    cout << "Algorithm: Indexed NL Join" << endl;
    /* Your solution goes here */
    Status status;
    RID rid_old1;
    Record rec_old1;
    HeapFileScan table1(attrDesc1.relName, status);
    if (status != OK) return status;

    RID rid_old2;
    Record rec_old2;
    Index index(attrDesc2.relName, attrDesc2.attrOffset, attrDesc2.attrLen, (Datatype)
            attrDesc2.attrType, 0, status);
    if (status != OK) return status;
    HeapFileScan table2(attrDesc2.relName, status);
    if (status != OK) return status;

    HeapFile new_hf(result, status);
    if (status != OK) return status;
    
    RID rid_new;
    Record rec_new;
    rec_new.length = reclen;
    rec_new.data = operator new (reclen);

    int i = attrDesc1.attrLen;
    void* value = operator new (i);
    while (table1.scanNext(rid_old1, rec_old1) == OK){
        memcpy(value, rec_old1.data+attrDesc1.attrOffset, attrDesc1.attrLen); //get value
        index.startScan(value);
        while (index.scanNext(rid_old2) == OK){
            status = table2.getRandomRecord(rid_old2, rec_old2);
            if (status != OK) {
                operator delete (value);
                operator delete (rec_new.data);   
                return status;
            }
            //fill in rec_new.data
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
            if (status != OK){
                operator delete (value);
                operator delete (rec_new.data);
                return status;
            }
        }
        index.endScan();
    }
    operator delete (value);
    operator delete (rec_new.data);
    return OK;
}

