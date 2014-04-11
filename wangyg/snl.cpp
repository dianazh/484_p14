#include "catalog.h"
#include "query.h"
#include "sort.h"
#include "index.h"

Status Operators::SNL(const string& result,           // Output relation name
                      const int projCnt,              // Number of attributes in the projection
                      const AttrDesc attrDescArray[], // Projection list (as AttrDesc)
                      const AttrDesc& attrDesc1,      // The left attribute in the join predicate
                      const Operator op,              // Predicate operator
                      const AttrDesc& attrDesc2,      // The left attribute in the join predicate
                      const int reclen)               // The length of a tuple in the result relation
{
  cout << "Algorithm: Simple NL Join" << endl;

  /* Your solution goes here */
  std::string relName1 (attrDesc1.relName);
  std::string relName2 (attrDesc2.relName);

  std::vector<unsigned> attrList1;
  std::vector<unsigned> attrList2;

  Status isOK1, isOK2;

  for (unsigned int i = 0; i < projCnt; i++){
    if ( strcmp(attrDescArray[i].relName, attrDesc1.relName) == 0 ){
      attrList1.push_back(i);
    }else if (strcmp(attrDescArray[i].relName, attrDesc2.relName == 0){
      attrList2.push_back(i);
    }else{
      return ATTRNOTFOUND;
    }
  }

  HeapFileScan heapfile1(relName1, isOK1);
  if (isOK1 != OK){
    return isOK1;
  }
  HeapFileScan heapfile1(relName1, isOK2);
  if (isOK2 != OK){
    return isOK2;
  }



  if (op == LT){

  }

  return OK;
}

