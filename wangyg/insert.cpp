#include "catalog.h"
#include "query.h"
#include "index.h"
#include <string.h>  //Can I do this?
#include <algorithm>

/*
 * Inserts a record into the specified relation
 *
 * Returns:
 * 	OK on success
 * 	an error code otherwise
 */

 struct MyComp{
     bool operator() (AttrDesc& attrdesc_L, AttrDesc& attrdesc_R){
         return std::std::lexicographical_compare(attrdesc_L.attrName,attrdesc_L.attrName+MAXNAME,
                                                  attrdesc_R.attrName,attrdesc_R.attrName+MAXNAME);
     }
 } mycomp;


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
    RelDesc insert2which;
    relCat->getInfo(relation ,insert2which);
    if (attrCnt != insert2which.attrCnt || attrCnt == 0){
        Error::print(ATTRNOTFOUND);
        return ATTRNOTFOUND;
    }
    AttrDesc* attrDescList;
    attrCat->getRelInfo(relation, attrCnt, attrDescList);
    std::sort(attrDescList, attrDescList+attrCnt, mycomp);
    unsigned int i = 0;
    AttrDesc* it;
    for (i = 0; i < attrCnt; i++){
        it = std::lower_bound(attrDescList, attrDescList+attrCnt, mycomp);
        if (it->indexed != 0){

        }
    }
}
