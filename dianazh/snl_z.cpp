#include "catalog.h"
#include "query.h"
#include "sort.h"
#include "index.h"

Status Operators::SNL(const string& result,           // Output relation name
                      const int projCnt,              // Number of attributes in the projection
                      const AttrDesc attrDescArray[], // Projection list (as AttrDesc)
                      const AttrDesc& attrDesc1,      // The left attribute in the join predicate
                      const Operator op,              // Predicate operator
                      const AttrDesc& attrDesc2,      // The left attribute in the join predicate
                      const int reclen)               // The length of a tuple in the result relation
{
    cout << "Algorithm: Simple NL Join" << endl;

    /* Your solution goes here */
    Status status;
    RID rid_old1;
    Record rec_old1;
    HeapFileScan table1(attrDesc1.relName, status);
    if (status != OK) return status;

    RID rid_old2;
    Record rec_old2;
    HeapFileScan table2(attrDesc2.relName, status);
    if (status != OK) return status;

    RID rid_new;
    Record rec_new;
    rec_new.length = reclen;
    rec_new.data = operator new (reclen);
    HeapFile new_hf(result, status);
    if (status != OK) return status;
    
    int i = attrDesc2.attrLen;
    void* value = operator new (i);
    while (table2.scanNext(rid_old2, rec_old2) == OK){
        memcpy(value, rec_old2.data+attrDesc2.attrOffset, attrDesc2.attrLen); //get value
        table1.startScan(attrDesc1.attrOffset, attrDesc1.attrLen, (Datatype)attrDesc1.attrType,
                (char*)value, op);
        while (table1.scanNext(rid_old1, rec_old1) == OK){
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
            if (status != OK) return status;
        }
    }

    return OK;
}

