#include "catalog.h"
#include "query.h"
#include "index.h"
#include <cstring>
//#include <algorithm>
//using std::lexicographical_compare;


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
    cout<<"relname: "<<projNames[0].relName<<endl; //test
    string relation(projNames[0].relName);
    HeapFileScan old_hf(relation, status_old);
    cout<<"here2"<<endl; //test
    if (status_old) return status_old;
    
    cout<<"init old"<<endl; //test
    Status status_new;
    RID rid_new;
    Record rec_new;
    rec_new.length = reclen;
    rec_new.data = operator new (reclen);
    HeapFile new_hf(result, status_new);
    if (status_new) return status_new;
    cout<<"init new"<<endl; //test

    if (!attrDesc){  //no filter
        cout<<"in no filter"<<endl; //test
        int i = 0; //test
        do {
            status_old = old_hf.scanNext(rid_old, rec_old);
            cout<<"after old_hf "<<i++<<endl; //test
            //copy in the data needed
            int offset = 0;
            for (int i=0; i<projCnt; i++){
                memcpy(rec_new.data+offset, rec_old.data+projNames[i].attrOffset,
                        projNames[i].attrLen);
                offset += projNames[i].attrLen;
            }
            status_new = new_hf.insertRecord(rec_new, rid_new);
            if (status_new != OK) return status_new;
            RelDesc test; //test
            relCat->getInfo(result, test); //test
            cout<<"after new_hf"<<test.attrCnt<<" "<<test.relName<<endl; //test
            if (status_new) return status_new;
        } while (status_old == OK);
        old_hf.endScan();
    } else {  //has filter
        do {
            status_old = old_hf.scanNext(rid_old, rec_old);
            switch (attrDesc->attrType){
                case INTEGER:
                {
                    int value1 = *((int*)attrValue);
                    int inrel1;
                    memcpy(&inrel1, rec_old.data+attrDesc->attrOffset, attrDesc->attrLen);
                    if (comp_num(inrel1,value1,op)){
                        int offset = 0;
                        for (int i=0; i<projCnt; i++){
                            memcpy(rec_new.data+offset, rec_old.data+projNames[i].attrOffset,
                                    projNames[i].attrLen);
                            offset += projNames[i].attrLen;
                        }
                        status_new = new_hf.insertRecord(rec_new, rid_new);
                        if (status_new) return status_new;    
                    }
                }
                case DOUBLE:
                {
                    double value2 = *((double*)attrValue);
                    double inrel2;
                    memcpy(&inrel2, rec_old.data+attrDesc->attrOffset, attrDesc->attrLen);
                    if (comp_num(inrel2, value2, op)){
                        int offset = 0;
                        for (int i=0; i<projCnt; i++){
                            memcpy(rec_new.data+offset, rec_old.data+projNames[i].attrOffset,
                                    projNames[i].attrLen);
                            offset += projNames[i].attrLen;
                        }
                        status_new = new_hf.insertRecord(rec_new, rid_new);
                        if (status_new) return status_new;
                    }
                }
                case STRING:
                {
                    char* value3 = (char*)attrValue;
                    char* inrel3; 
                    memcpy(inrel3, rec_old.data+attrDesc->attrOffset, attrDesc->attrLen);
                    if (comp_str(inrel3, value3,op)){
                        int offset = 0;
                        for (int i=0; i<projCnt; i++){
                            memcpy(rec_new.data+offset, rec_old.data+projNames[i].attrOffset,
                                    projNames[i].attrLen);
                            offset += projNames[i].attrLen;
                        }
                        status_new = new_hf.insertRecord(rec_new, rid_new);
                        if (status_new) return status_new;
                    }

                }
            }
        } while (status_old);
        old_hf.endScan();
    }
    return OK;
}
