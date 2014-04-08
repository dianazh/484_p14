#include "catalog.h"
#include "query.h"
#include "index.h"

/*
 * Selects records from the specified relation.
 *
 * Returns:
 * 	OK on success
 * 	an error code otherwise
 */
Status Operators::Select(const string & result,      // name of the output relation
	                 const int projCnt,          // number of attributes in the projection
		         const attrInfo projNames[], // the list of projection attributes
		         const attrInfo *attr,       // attribute used inthe selection predicate 
		         const Operator op,         // predicate operation
		         const void *attrValue)     // literal value in the predicate
{
    Status status;
    //get rel_info
    attrInfo sample = projNames[0];
    RelDesc rel_info;
    status = relCat->getInfo(sample.relName, rel_info);
    if (status)   return status;
    //get attrdesc for projs & reclen of projs
    AttrDesc proj_info[projCnt];
    int reclen = 0;
    for (int i=0; i<projCnt; i++){
        status = attrCat->getInfo(rel_info.relName, projNames[i].attrName, proj_info[i]);
        if (status)   return status;
        reclen += proj_info[i].attrLen;
    }

    if (!attr){ //no filter, use scan
        status = ScanSelect(result, projCnt, proj_info, NULL, op, attrValue, reclen);
    } else {
        //get attrDesc
        AttrDesc attr_info;
        status = attrCat->getInfo(rel_info.relName, attr->attrName, attr_info);
        if (status)   return status;

        if ((attr_info.indexed) && (op == EQ)){  //use index select
            status = ScanSelect(result, projCnt, proj_info, &attr_info, op, attrValue, reclen);
            if (status) return status;  
        } else {  //use scan
            status = IndexSelect(result, projCnt, proj_info, &attr_info, op, attrValue, reclen);
            if (status) return status; 
        }
    }
    return OK;
}

