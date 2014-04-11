#include "catalog.h"
#include "query.h"
#include "index.h"
#include <cstring>

Status Operators::IndexSelect(const string& result,       // Name of the output relation
                              const int projCnt,          // Number of attributes in the projection
                              const AttrDesc projNames[], // Projection list (as AttrDesc)
                              const AttrDesc* attrDesc,   // Attribute in the selection predicate
                              const Operator op,          // Predicate operator
                              const void* attrValue,      // Pointer to the literal value in the predicate
                              const int reclen)           // Length of a tuple in the output relation
{
    cout << "Algorithm: Index Select" << endl;
    Status status;
    RID rid_old;
    Record rec_old;
    Index index(attrDesc->relName, attrDesc->attrOffset, attrDesc->attrLen, (Datatype)attrDesc->attrType, 0,
            status);
    if (status != OK) return status;
    HeapFileScan old_hf(attrDesc->relName, status);
    if (status != OK) return status;

    RID rid_new;
    Record rec_new;
    rec_new.length = reclen;
    rec_new.data = operator new (reclen);
    HeapFile new_hf(result, status);
    if (status != OK) return status;

    //int recnum = old_hf.getRecCnt();

    index.startScan(attrValue);
    while (index.scanNext(rid_old)==OK){
        status = old_hf.getRandomRecord(rid_old, rec_old);
        if (status != OK) return status;
        int offset = 0;
        for (int i=0; i<projCnt; i++){
            memcpy(rec_new.data+offset, rec_old.data+projNames[i].attrOffset, projNames[i].attrLen);
            offset += projNames[i].attrLen;
        }
        status = new_hf.insertRecord(rec_new, rid_new);
        if (status!= OK) return status;
    }
    index.endScan();
    return OK;
}

