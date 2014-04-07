#include "catalog.h"
#include "query.h"
#include "index.h"
#include <string.h>  //Can I do this? 

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
    /* Your solution goes here */
    Status status;
    //lookup relcat
    int t_attrCnt = attrCnt;
    bool has_index = false;
    RelDesc rel_info;
    status = relCat->getInfo(relation, rel_info);
    if (status != OK){
        error.print(status);
       return status;
    }
    cout<<rel_info.attrCnt<<" "<<rel_info.indexCnt<<" "<<rel_info.relName<<endl; //test
    if (rel_info.indexCnt != 0) has_index = true;
    if (rel_info.attrCnt != attrCnt){
        return NOTUSED2; //NOT SURE  //attr number doesn't match
    }
    //lookup attrcat
    AttrDesc* attrs_info;
    int entry_size = 0;
    int* match_num;
    match_num = new int [attrCnt];   //in case not in order, prefilled with -1;
    for (int i = 0; i < attrCnt; i++){
        match_num[i] = -1;
    }
    status = attrCat->getRelInfo(relation, t_attrCnt, attrs_info);
    if (status != OK){
        error.print(status);
        delete[] match_num;
        return status;
    }
    int i = 0;
    while (i<t_attrCnt){
        cout<<(attrs_info+i)->attrName<<endl; //test
        i++;
    }
    //get the order, size, index
    for (int i = 0; i < attrCnt; i++){
        for (int j = 0; j < attrCnt; j++){
            if (!strcmp((attrs_info+i)->attrName,attrList[j].attrName)){
                match_num[i] = j;
            }
        }
        cout<<"after loop j "<<match_num[i]<<endl; //test
        if (match_num[i] == -1){
            delete[] match_num;
            return NOTUSED2;  //attr name not exist
        }
        entry_size += (attrs_info+i)->attrLen;
        cout<<"size: "<<entry_size<<endl; //test
    }
    //insert record
    Record new_rec;
    new_rec.length = entry_size;
    cout<<"inserting "<<new_rec.data<<endl; //test
    for (int i = 0; i < attrCnt; i++){
        void* temp = (new_rec.data)+(attrs_info+i)->attrOffset;
        cout<<"temp: "<<temp<<" "<<(attrs_info+i)->attrOffset<<" "<<(attrs_info+i)->attrLen<<endl; //test
        memcpy(temp, attrList[match_num[i]].attrValue, (attrs_info+i)->attrLen);
        //cout<<(char*)new_rec.data<<endl; //test
    }
    RID new_rid;
    cout<<"open file"<<endl; //test
    HeapFile rel_heapfile(relation, status);  //create or open the file
    cout<<"open file"<<endl; //test
    if (status != OK){
        error.print(status);   
        delete[] match_num;
        return status;
    }
    cout<<"insert entry"<<endl; //test
    cout<<"new_rec: "<<new_rec.length<<endl<<(char*)new_rec.data<<endl; //test
    status = rel_heapfile.insertRecord(new_rec, new_rid);  //insert new_rec
    if (status != OK){
        error.print(status);   
        delete[] match_num;
        return status;
    }
    //insert index
    cout<<"insert index"<<endl; //test
    if (has_index){
        for (int i=0; i < attrCnt; i++){
            AttrDesc temp = *(attrs_info+i);
            if (temp.indexed){
                Index rel_index(relation, temp.attrOffset, temp.attrLen, (Datatype)temp.attrType, 0, status);
                //should unique?(1) or not?(0) 
                rel_index.insertEntry(attrList[match_num[i]].attrValue, new_rid);
            }
        }
    }
    delete[] match_num;

    return OK;
}
