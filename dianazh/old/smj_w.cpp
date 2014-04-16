#include "catalog.h"
#include "query.h"
#include "sort.h"
#include "index.h"


/* Consider using Operators::matchRec() defined in join.cpp
 * to compare records when joining the relations */

Status Operators::SMJ(const string& result,           // Output relation name
                      const int projCnt,              // Number of attributes in the projection
                      const AttrDesc attrDescArray[], // Projection list (as AttrDesc)
                      const AttrDesc& attrDesc1,      // The left attribute in the join predicate
                      const Operator op,              // Predicate operator
                      const AttrDesc& attrDesc2,      // The left attribute in the join predicate
                      const int reclen)               // The length of a tuple in the result relation
{
  cout << "Algorithm: SM Join" << endl;

  /* Your solution goes here */
  assert(op == EQ);
  assert(attrDesc1.indexed == 0);
  assert(attrDesc2.indexed == 0);

  Status isOK;
  std::vector<int> attrList1;
  std::vector<int> attrList2;
  int m = 0;
  unsigned resulttupleidx = 0;
  RID outRid;

  for (int i = 0; i < projCnt; i++){
    if ( strcmp(attrDescArray[i].relName, attrDesc1.relName) == 0 ){
      attrList1.push_back(i);
    }else if (strcmp(attrDescArray[i].relName, attrDesc2.relName) == 0){
      attrList2.push_back(i);
    }else{
      return ATTRNOTFOUND;
    }
  }

  HeapFile resultfile(result, isOK);
  if (isOK != OK){
    return isOK;
  }
  //relname1 = std::string(attrDesc1.relName);
  //relname2 = std::string(attrDesc2.relName);
  if (attrDesc1.attrType != attrDesc2.attrType){
    return ATTRTYPEMISMATCH;
  }

  SortedFile sortedfile1(attrDesc1.relName, attrDesc1.attrOffset, attrDesc1.attrLen, (Datatype)attrDesc1.attrType, 0.8*(bufMgr->numUnpinnedPages()), isOK);
  if (isOK != OK){
    return isOK;
  }

  SortedFile sortedfile2(attrDesc1.relName, attrDesc2.attrOffset, attrDesc2.attrLen, (Datatype)attrDesc2.attrType, 0.8*(bufMgr->numUnpinnedPages()), isOK);
  if (isOK != OK){
    return isOK;
  }
  Record rec1_old, rec1_new, rec2_old, rec2_new;
  rec1_old.data = NULL;
  rec1_old.length = -1;
  rec1_new.data = NULL;
  rec1_new.length = -1;
  rec2_old.data = NULL;
  rec2_old.length = -1;

  isOK = OK;
  while( 1 ){
    isOK = sortedfile1.next(rec1_new);
    if (isOK == FILEEOF){
      if ( reccmp_t(rec1_old.data+attrDesc1.attrOffset, rec1_new+attrDesc1, attrDesc1.attrLen, attrDesc1.attrLen, (Datatype)attrDesc1.attrType, EQ) ){
      }
    }
    if ( reccmp_t(rec1_new) ){}
  }


  return OK;
}

