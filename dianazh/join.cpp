#include "catalog.h"
#include "query.h"
#include "sort.h"
#include "index.h"
#include <cmath>
#include <cstring>

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define DOUBLEERROR 1e-07

/*
 * Joins two relations
 *
 * Returns:
 * 	OK on success
 * 	an error code otherwise
 */

Status Operators::Join(const string& result,           // Name of the output relation 
                       const int projCnt,              // Number of attributes in the projection
    	               const attrInfo projNames[],     // List of projection attributes
    	               const attrInfo* attr1,          // Left attr in the join predicate
    	               const Operator op,              // Predicate operator
    	               const attrInfo* attr2)          // Right attr in the join predicate
{
    /* Your solution goes here */
    /*static Status SNL(const string & result,          // output relation name
	             const int projCnt,              // number of attributes in the projection
                     const AttrDesc attrDescArray[], // The projection list (as AttrDesc)
                     const AttrDesc & attrDesc1,     // The left attribute in the join predicate
                     const Operator op,              // The join operation
                     const AttrDesc & attrDesc2,     // The left attribute in the join predicate
                     const int reclen);*/
    Status status;
    string rel1 (attr1->relName);
    string rel2 (attr2->relName);
    AttrDesc attrdesc1;
    AttrDesc attrdesc2;
    status = attrCat->getInfo(rel1, attr1->attrName, attrdesc1);
    if (status != OK) return status;
    status = attrCat->getInfo(rel2, attr2->attrName, attrdesc2);
    if (status != OK) return status;
    AttrDesc proj_info[projCnt];
    int reclen = 0;
    for (int i=0; i<projCnt; i++){
        status = attrCat->getInfo(projNames[i].relName, projNames[i].attrName, proj_info[i]);
        if (status != OK)   return status;
        reclen += proj_info[i].attrLen;
    }
    if (op != EQ){
        status = SNL(result, projCnt, proj_info, attrdesc1, op, attrdesc2, reclen);
        if (status != OK)   return status;
    } else if ((attrdesc1.indexed == 1)||(attrdesc2.indexed == 1)){
        status = INL(result, projCnt, proj_info, attrdesc1, op, attrdesc2, reclen);
        if (status != OK)   return status;
    } else {
        status = SMJ(result, projCnt, proj_info, attrdesc1, op, attrdesc2, reclen);
        if (status != OK)   return status;
    }

    return OK;
}

// Function to compare two record based on the predicate. Returns 0 if the two attributes 
// are equal, a negative number if the left (attrDesc1) attribute is less that the right 
// attribute, otherwise this function returns a positive number.
int Operators::matchRec(const Record& outerRec,     // Left record
                        const Record& innerRec,     // Right record
                        const AttrDesc& attrDesc1,  // Left attribute in the predicate
                        const AttrDesc& attrDesc2)  // Right attribute in the predicate
{
    int tmpInt1, tmpInt2;
    double tmpFloat1, tmpFloat2, floatDiff;

    // Compare the attribute values using memcpy to avoid byte alignment issues
    switch(attrDesc1.attrType)
    {
        case INTEGER:
            memcpy(&tmpInt1, (char *) outerRec.data + attrDesc1.attrOffset, sizeof(int));
            memcpy(&tmpInt2, (char *) innerRec.data + attrDesc2.attrOffset, sizeof(int));
            return tmpInt1 - tmpInt2;

        case DOUBLE:
            memcpy(&tmpFloat1, (char *) outerRec.data + attrDesc1.attrOffset, sizeof(double));
            memcpy(&tmpFloat2, (char *) innerRec.data + attrDesc2.attrOffset, sizeof(double));
            floatDiff = tmpFloat1 - tmpFloat2;
            return (fabs(floatDiff) < DOUBLEERROR) ? 0 : floatDiff;

        case STRING:
            return strncmp(
                (char *) outerRec.data + attrDesc1.attrOffset, 
                (char *) innerRec.data + attrDesc2.attrOffset, 
                MAX(attrDesc1.attrLen, attrDesc2.attrLen));
    }

    return 0;
}
