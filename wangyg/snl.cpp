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

  assert(op != EQ);

  std::string relName1 (attrDesc1.relName);
  std::string relName2 (attrDesc2.relName);

  std::vector<int> attrList1;
  std::vector<int> attrList2;

  Status isOK;

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

  HeapFileScan heapfile1(relName1, isOK);
  if (isOK != OK){
    return isOK;
  }
  HeapFileScan heapfile2(relName2, isOK);
  if (isOK != OK){
    return isOK;
  }

  int reccnt1 = heapfile1.getRecCnt();
  int reccnt2 = heapfile2.getRecCnt();
  RID outRid1, outRid2;
  int j = 0, k = 0, m= 0;
  Record rec1, rec2;
  unsigned resulttupleidx = 0;

  if (attrDesc1.attrType != attrDesc2.attrType){
    return ATTRTYPEMISMATCH;
  }
  RID outRid;
  int isOp;

  //heapfile1.startScan(attrDesc1.attrOffset, attrDesc1.attrLen, (Datatype) attrDesc1.attrType, NULL, op);
  for (j = 0; j < reccnt1; j++){
    isOK = heapfile1.scanNext(outRid1, rec1);
    if (isOK != OK){
      return isOK;
    }
    //isOK = heapfile2.startScan(attrDesc2.attrOffset, attrDesc2.attrLen, (Datatype) attrDesc2.attrType, NULL, op);
    for (k = 0; k < reccnt2; k++){
      isOK = heapfile2.scanNext(outRid2, rec2);
      if (isOK != OK){
        return isOK;
      }
      isOp = reccmp_t( (char*) rec1.data + attrDesc1.attrOffset, (char*) rec2.data + attrDesc2.attrOffset, attrDesc1.attrLen, attrDesc2.attrLen, (Datatype)attrDesc1.attrType, op);
      if (isOp == -1){
        return NOTUSED1;
      }else{
        if (isOp){
          Record joinresult;
          joinresult.length = reclen;
          joinresult.data = (void*) new char [reclen];
          resulttupleidx = 0;
          for (m = 0; m < projCnt; m++){
            if (std::binary_search(attrList1.begin(), attrList1.end(), m)){
              memcpy(joinresult.data+resulttupleidx, rec1.data + attrDescArray[m].attrOffset, attrDescArray[m].attrLen);
              resulttupleidx += attrDescArray[m].attrLen;
            }else{
              memcpy(joinresult.data+resulttupleidx, rec2.data + attrDescArray[m].attrOffset, attrDescArray[m].attrLen);
              resulttupleidx += attrDescArray[m].attrLen;
            }

          }
          resultfile.insertRecord(joinresult, outRid);
          delete [] joinresult.data;

        }
      }

    }
    heapfile2.endScan();
  }
  heapfile1.endScan();

  return OK;
}
