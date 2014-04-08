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
typedef struct match_t{
    int* match_num;
    match_t(int attrCnt){
        match_num = new int [attrCnt];
        for (int i = 0; i < attrCnt; i++){
            match_num[i] = -1;
        }
    }
    ~match_t(){
        delete[] match_num;
    }
} match;
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
    if (status != OK){
        error.print(status);
       return status;
    }
    //cout<<rel_info.relName<<": "<<rel_info.attrCnt<<" index: "<<rel_info.indexCnt<<endl; //test
    //attr number doesn't match
    if (rel_info.attrCnt != attrCnt){
        return NOTUSED2; //NOT SURE
    }
    if (rel_info.indexCnt != 0) has_index = true;
    //lookup attrcat
    AttrDesc* attrs_info;
    int entry_size = 0;
    status = attrCat->getRelInfo(relation, attrCnt_copy, attrs_info);
    if (status != OK){
        error.print(status);
        //delete[] match_num;
        return status;
    }
    /*int i = 0;
    while (i<attrCnt_copy){
        cout<<(attrs_info+i)->attrName<<endl; //test
        i++;
    }*/
    //get the order, size, index
    /*int* match_num;
    match_num = new int [attrCnt];   //in case not in order, prefilled with -1;
    for (int i = 0; i < attrCnt; i++){
        match_num[i] = -1;
    }*/
    match match_arr(attrCnt);
    for (int i = 0; i < attrCnt; i++){
        for (int j = 0; j < attrCnt; j++){
            if (!strcmp((attrs_info+i)->attrName,attrList[j].attrName)){
                match_arr.match_num[i] = j;
            }
        }
        //attr name not exist
        if (match_arr.match_num[i] == -1){
            //delete[] match_num;
            return NOTUSED2;
        }
        entry_size += (attrs_info+i)->attrLen;
        //cout<<"at loop: "<<i<<"entry_size: "<<entry_size<<endl; //test
    }
    //insert record
    Record new_rec;
    new_rec.length = entry_size;
    new_rec.data = operator new (entry_size);
    for (int i = 0; i < attrCnt; i++){
        void* temp = (void*)(((unsigned int long)new_rec.data)+(attrs_info+i)->attrOffset);
        //cout<<"temp: "<<temp<<" "<<(attrs_info+i)->attrOffset<<" "<<(attrs_info+i)->attrLen<<endl; //test
        memcpy(temp, attrList[match_arr.match_num[i]].attrValue, (attrs_info+i)->attrLen);
    }
    RID new_rid;
    //cout<<"open file"<<endl; //test
    HeapFile rel_heapfile(relation, status);  //create or open the file
    if (status != OK){
        error.print(status);
        //delete[] match_num;
        operator delete (new_rec.data);
        return status;
    }

    status = rel_heapfile.insertRecord(new_rec, new_rid);  //insert new_rec
    //cout<<"record count: "<<rel_heapfile.getRecCnt()<<endl; //test
    if (status != OK){
        error.print(status);
        //delete[] match_num;
        operator delete (new_rec.data);
        return status;
    }
    //insert index
    if (has_index){
        for (int i=0; i < attrCnt; i++){
            AttrDesc temp = *(attrs_info+i);
            if (temp.indexed){
                //cout<<"insert index name: "<<temp.attrName<<endl; //test
                Index rel_index(relation, temp.attrOffset, temp.attrLen, (Datatype)temp.attrType, 0, status);
                //should unique?(1) or not?(0)
                if (status != OK){
                    error.print(status);
                    //delete[] match_num;
                    operator delete (new_rec.data);
                    return status;
                }
                status = rel_index.insertEntry(attrList[match_arr.match_num[i]].attrValue, new_rid);
                if (status != OK){
                    error.print(status);
                    //delete[] match_num;
                    operator delete (new_rec.data);
                    return status;
                }
            } /*else {
                cout<<"no index: "<<temp.attrName<<endl; //test
            }*/
        }
    }

    //delete[] match_num;
    operator delete (new_rec.data);
    return OK;
}
