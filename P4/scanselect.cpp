#include "catalog.h"
#include "query.h"
#include "index.h"


/* 
 * A simple scan select using a heap file scan
 */

Status Operators::ScanSelect(const string& result,       // Name of the output relation
                             const int projCnt,          // Number of attributes in the projection
                             const AttrDesc projNames[], // Projection list (as AttrDesc)
                             const AttrDesc* attrDesc,   // Attribute in the selection predicate
                             const Operator op,          // Predicate operator
                             const void* attrValue,      // Pointer to the literal value in the predicate
                             const int reclen)           // Length of a tuple in the result relation
{
    cout << "Algorithm: File Scan" << endl; 
    Status status_old;
    RID rid_old;
    Record rec_old;
    string relation(projNames[0].relName);
    HeapFileScan old_hf(relation, status_old); 
    if (status_old != OK) return status_old;
    
    Status status_new;
    HeapFile new_hf(result, status_new);
    if (status_new != OK) return status_new;
    RID rid_new;
    Record rec_new;
    rec_new.length = reclen;
    rec_new.data = operator new (reclen);

    if (attrDesc != NULL){  //has filter
        old_hf.startScan(attrDesc->attrOffset, attrDesc->attrLen, (Datatype) attrDesc->attrType,
                (char*)attrValue, op);
    }    
        
    while (old_hf.scanNext(rid_old, rec_old) == OK){
        int offset = 0;
        for (int i=0; i<projCnt; i++){
            memcpy(rec_new.data+offset, rec_old.data+projNames[i].attrOffset,
                    projNames[i].attrLen);
            offset+= projNames[i].attrLen;
        }
        status_new = new_hf.insertRecord(rec_new, rid_new);
        if (status_new != OK) {
            operator delete (rec_new.data);
            return status_new;
        }
    }
    operator delete (rec_new.data);
    return OK;
}
