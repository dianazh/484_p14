#include "catalog.h"
#include "query.h"
#include "sort.h"
#include "index.h"

/* 
 * Indexed nested loop evaluates joins with an index on the 
 * inner/right relation (attrDesc2)
 */

Status Operators::INL(const string& result,           // Name of the output relation
                      const int projCnt,              // Number of attributes in the projection
                      const AttrDesc attrDescArray[], // The projection list (as AttrDesc)
                      const AttrDesc& attrDesc1,      // The left attribute in the join predicate
                      const Operator op,              // Predicate operator
                      const AttrDesc& attrDesc2,      // The left attribute in the join predicate
                      const int reclen)               // Length of a tuple in the output relation
{
    cout << "Algorithm: Indexed NL Join" << endl;

    /* Your solution goes here */
    Status status;
    RID rid_old1;
    Record rec_old1;
    HeapFileScan table1(attrDesc1->relName, status);
    if (status != OK) return status;

    RID rid_old2;
    Record rec_old2;
    Index index(attrDesc2->relName, attrDesc2->attrOffset, attrDesc2->attrLen, (Datatype)
            attrDesc2->attrType, 0, status);
    if (status != OK) return status;
    HeapfileScan table2(attrDesc2->relName, status);
    if (status != OK) return status;


    return OK;
}

