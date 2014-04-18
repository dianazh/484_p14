#include "catalog.h"
#include "query.h"
#include "index.h"
#include <cstring>


/* 
 * A simple scan select using a heap file scan
 */
bool comp_num(double inrel, double given, const Operator op){
    //LT, LTE, EQ, GTE, GT, NE, NOTSET
    switch (op){
        case LT:
            return (inrel < given);
        case LTE:
            return (inrel <= given);
        case EQ:
            return (inrel == given);
        case GTE:
            return (inrel >= given);
        case GT:
            return (inrel > given);
        case NE:
            return (inrel != given);  
        case NOTSET:
            return false; 
    }
}

bool comp_str(char* inrel, char* given, const Operator op){
    //LT, LTE, EQ, GTE, GT, NE, NOTSET
    /*Returns an integral value indicating the relationship between the strings:
A zero value indicates that both strings are equal.
A value greater than zero indicates that the first character that does not match has a greater value
in str1 than in str2; And a value less than zero indicates the opposite.*/
    int rel = strcmp(inrel,given);
    switch (op){
        case LT:
            return (rel<0);
        case LTE:
            return (rel<=0);
        case EQ:
            return (rel==0);
        case GTE:
            return (rel>=0);
        case GT:
            return (rel>0);
        case NE:
            return (rel!=0);   
        case NOTSET:
            return false;
    }
}


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
    RID rid_new;
    Record rec_new;
    rec_new.length = reclen;
    rec_new.data = operator new (reclen);
    HeapFile new_hf(result, status_new);
    if (status_new != OK) return status_new;
   
    int recnum = old_hf.getRecCnt();

    if (attrDesc == NULL){  //no filter
        for (int i=0; i < recnum; i++){
            status_old = old_hf.scanNext(rid_old, rec_old);
            if (status_old != OK) return status_old;
            //copy in the data needed
            int offset = 0;
            for (int i=0; i<projCnt; i++){
                memcpy(rec_new.data+offset, rec_old.data+projNames[i].attrOffset,
                        projNames[i].attrLen);
                offset += projNames[i].attrLen;
            }
            status_new = new_hf.insertRecord(rec_new, rid_new);
            if (status_new != OK) return status_new;
            
        }
        //old_hf.endScan();   //called in destructor
    } else {  //has filter
        old_hf.startScan(attrDesc->attrOffset, attrDesc->attrLen, (Datatype) attrDesc->attrType,
                (char*)attrValue, op);
        while (old_hf.scanNext(rid_old, rec_old) == OK){
            int offset = 0;
            for (int i=0; i<projCnt; i++){
                memcpy(rec_new.data+offset, rec_old.data+projNames[i].attrOffset,
                        projNames[i].attrLen);
                offset+= projNames[i].attrLen;
            }
            status_new = new_hf.insertRecord(rec_new, rid_new);
            if (status_new != OK) return status_new;
        }
        
        //old_hf.endScan();
    }
    return OK;
}
