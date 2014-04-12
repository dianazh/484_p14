#include "catalog.h"
#include "query.h"
#include "index.h"


/*
 * Inserts a record into the specified relation
 *
 * Returns:
 * 	OK on success
 * 	an error code otherwise
 */

typedef struct {
     bool operator() (const attrInfo& Left, const attrInfo& Right){
         return std::lexicographical_compare(Left.attrName, Left.attrName+MAXNAME,
                                             Right.attrName, Right.attrName+MAXNAME);
     }
 } MyComp;

typedef struct {
     bool operator() (const attrInfo& Left, const AttrDesc& Right){
         return std::lexicographical_compare(Left.attrName, Left.attrName+MAXNAME,
                                             Right.attrName, Right.attrName+MAXNAME);
     }
 } MyComp2;

typedef struct {
    int numInDesc;
    int numInattrList;
} Bookkeep;

Status Updates::Insert(const string& relation,      // Name of the relation
                       const int attrCnt,           // Number of attributes specified in INSERT statement
                       const attrInfo attrList[])   // Value of attributes specified in INSERT statement
{
    RelDesc insert2which;
    int i = 0;
    Status isOK;

    int attrCnt_cp = attrCnt;
    string relation_cp = relation;
    AttrDesc* attrDescList;
    unsigned int totalLen = 0;

    isOK = relCat->getInfo(relation ,insert2which);

    if (isOK != OK){
        //Error::print(isOK);
        return isOK;
    }

    if (attrCnt != insert2which.attrCnt || attrCnt == 0){
        //Error::print(ATTRNOTFOUND);
        return ATTRNOTFOUND;
    }


    attrCat->getRelInfo(relation_cp, attrCnt_cp, attrDescList);
    for (i = 0; i < attrCnt; i++){
        totalLen += attrDescList[i].attrLen;
    }

    char* newRecordData = new char [totalLen];

    HeapFile relationFile(relation, isOK);

    if (isOK != OK){
        //Error::print(isOK);
        return isOK;
    }
    Record newRecord;
    newRecord.data = (void *)newRecordData;
    newRecord.length = totalLen;

    attrInfo* attrList_cp = (attrInfo*) (unsigned long)attrList;
    std::sort(attrList_cp, attrList_cp+attrCnt, MyComp());

    attrInfo* it;
    RID newRid;

    std::vector<int> indexedAttr;

    for (i = 0; i < attrCnt; i++){
        it = std::lower_bound(attrList_cp, attrList_cp+attrCnt, attrDescList[i], MyComp2());
        assert(strcmp(it->attrName, attrDescList[i].attrName)==0);
        //std::cout << "\tattrName: " << it->attrName << "\tattrType: " << it->attrType << "\tattrLen: " << it->attrLen << '\n';
        //std::cout << "\tattrName: " << attrDescList[i].attrName << "\tattrType: " << attrDescList[i].attrType << "\tattrLen: "
        //        << attrDescList[i].attrLen << "\tattrOffset " << attrDescList[i].attrOffset <<'\n';

        unsigned long beginAddr = (unsigned long) newRecord.data + attrDescList[i].attrOffset;
        //std::cout << beginAddr << "\t" << attrDescList[i].attrOffset << '\n';

        memcpy((void*) beginAddr, it->attrValue, attrDescList[i].attrLen);

        if (attrDescList[i].indexed != 0){
            indexedAttr.push_back(i);
        }

    }

    relationFile.insertRecord(newRecord, newRid);
    assert( indexedAttr.size() ==  (unsigned)insert2which.indexCnt );
    if (insert2which.indexCnt != 0){
        for (i = 0; (unsigned) i < indexedAttr.size(); i++){
            Index idx(insert2which.relName, attrDescList[indexedAttr[i]].attrOffset, attrDescList[indexedAttr[i]].attrLen,
                    (Datatype) attrDescList[indexedAttr[i]].attrType, 0, isOK);
            if (isOK != OK){
                //Error::print(isOK);
                return isOK;
            }
            idx.insertEntry( (void *)((unsigned long)newRecord.data+attrDescList[indexedAttr[i]].attrOffset), newRid);
        }
    }
    delete[] newRecordData;
    return OK;
}
