#include "catalog.h"
#include "query.h"
#include "index.h"
//Can I do this?
#include <string.h>
#include <vector>

/*
 * Inserts a record into the specified relation
 *
 * Returns:
 * 	OK on success
 * 	an error code otherwise
 */

Status Updates::Insert(const string& relation,      // Name of the relation
                       const int attrCnt,           // Number of attributes specified in INSERT statement
                       const attrInfo attrList[])   // Value of attributes specified in INSERT statement
{
    Status status;
    //lookup relcat
    int attrCnt_copy = attrCnt; //const int -> int
    bool has_index = false;
    RelDesc rel_info;
    status = relCat->getInfo(relation, rel_info);
    if (status != OK)   return status;

    //attr number doesn't match
    if (rel_info.attrCnt != attrCnt){
        return NOTUSED2; //NOT SURE
    }
    if (rel_info.indexCnt != 0) has_index = true;
    //lookup attrcat
    AttrDesc* attrs_info;
    int entry_size = 0;
    status = attrCat->getRelInfo(relation, attrCnt_copy, attrs_info);
    if (status != OK) return status;
    //get the order, size
    vector<int> match_num(attrCnt, -1);
    for (int i = 0; i < attrCnt; i++){
        for (int j = 0; j < attrCnt; j++){
            if (attrList[j].attrValue == NULL){
                return NOTUSED2; //NOT SURE
            }
            if (!strcmp((attrs_info+i)->attrName,attrList[j].attrName)){
                match_num[i] = j;
            }
        }
        //attr name not exist
        if (match_num[i] == -1){
            return NOTUSED2;
        }
        entry_size += (attrs_info+i)->attrLen;
    }
    //generate record
    Record new_rec;
    new_rec.length = entry_size;
    new_rec.data = operator new (entry_size);
    for (int i = 0; i < attrCnt; i++){
        memcpy(new_rec.data+(attrs_info+i)->attrOffset, attrList[match_num[i]].attrValue, (attrs_info+i)->attrLen);
    }
    //create or open the file
    HeapFile rel_heapfile(relation, status);  
    if (status != OK){
        operator delete (new_rec.data);
        return status;
    }
    //insert new_rec
    RID new_rid;
    status = rel_heapfile.insertRecord(new_rec, new_rid);  
    if (status != OK){
        operator delete (new_rec.data);
        return status;
    }
    /*************test if insert, seg fault if run :(************
    relCat->help(relation);
    HeapFileScan new_test(relation,status);
    cout<<"tuple count: "<<new_test.getRecCnt()<<endl; 
    cout<<"last inserted: "<<endl;
    Record test;
    RID testid;
    new_test.scanNext(testid, test); 
    RelDesc output1;
    relCat->getInfo(relation, output1);
    int count = output1.attrCnt;
    AttrDesc* output;
    attrCat->getRelInfo(relation,count,output);
    for (int i = 0; i< count; i++){
        cout<<"attr no. "<<i<<" "; 
        switch ((output+i)->attrType){
            case INTEGER:
                {
                    int temp1;
                    memcpy(&temp1, test.data+(output+i)->attrOffset, (output+i)->attrLen);
                    cout<<(output+i)->attrName<<": "<<temp1<<endl;
                    break;
                }
            case DOUBLE:
                {
                    double temp2;
                    memcpy(&temp2, test.data+(output+i)->attrOffset, (output+i)->attrLen);
                    cout<<(output+i)->attrName<<": "<<temp2<<endl;
                    break;
                }
            case STRING:
                {
                    char* temp3;
                    memcpy(temp3, test.data+(output+i)->attrOffset, (output+i)->attrLen);
                    cout<<(output+i)->attrName<<": "<<temp3<<endl;
                    break;
                }
        }
    }
    new_test.endScan();
    cout<<"exist"<<endl; //test
    //***************/
    //insert index
    if (has_index){
        for (int i=0; i < attrCnt; i++){
            AttrDesc temp = *(attrs_info+i);
            if (temp.indexed){
                Index rel_index(relation, temp.attrOffset, temp.attrLen, (Datatype)temp.attrType, 0, status);
                //should unique?(1) or not?(0)
                if (status != OK){
                    operator delete (new_rec.data);
                    return status;
                }
                status = rel_index.insertEntry(attrList[match_num[i]].attrValue, new_rid);
                if (status != OK){
                    operator delete (new_rec.data);
                    return status;
                }
            } 
        }
    }
    operator delete (new_rec.data);
    return OK;
}
