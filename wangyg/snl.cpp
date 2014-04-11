#include "catalog.h"
#include "query.h"
#include "sort.h"
#include "index.h"

#include <cstring>
#include <algorithm>


static int reccmp(char* p1, char* p2, int p1Len, int p2Len, Datatype type, Operator op)
{
  int diff = 0;
  int iattr, ifltr;
  double fattr, ffltr;

  switch(type) {
  case INTEGER:
                      // word-alignment problem possible
    memcpy(&iattr, p1, sizeof(int));
    memcpy(&ifltr, p2, sizeof(int));
    diff = iattr - ifltr;
    break;
  case DOUBLE:
    double tmp;                 // word-alignment problem possible
    memcpy(&fattr, p1, sizeof(double));
    memcpy(&ffltr, p2, sizeof(double));
    tmp = fattr - ffltr;
    if (tmp < 0){
      diff = -1;
    }else if (tmp == 0){
      diff = 0;
    }else{
      diff = 1;
    }

    break;
  case STRING:
    diff = strncmp(p1, p2, std::min(p1Len, p2Len));
    break;
  default:
    break;
  }
  switch(op) {
  case LT:
    return (int) (diff < 0);
    break;
  case LTE:
    return (int) (diff <= 0);
    break;
  case EQ:
    return (int) (diff == 0);
    break;
  case GTE:
    return (int) (diff >= 0);
    break;
  case GT:
    return (int) (diff > 0);
    break;
  case NE:
    return (int) (diff != 0);
    break;
  case NOTSET:
    return -1;
    break;
  default:
    return -1;
    break;
  }
}



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
  int diff;

  //heapfile1.startScan(attrDesc1.attrOffset, attrDesc1.attrLen, (Datatype) attrDesc1.attrType, NULL, op);
  for (j = 0; j < reccnt1; j++){
    heapfile1.scanNext(outRid1, rec1);
    //heapfile2.startScan(attrDesc2.attrOffset, attrDesc2.attrLen, (Datatype) attrDesc2.attrType, NULL, op);
    for (k = 0; k < reccnt2; k++){
      heapfile2.scanNext(outRid2, rec2);
      diff = reccmp( (char*) rec1.data + attrDesc1.attrOffset, (char*) rec2.data + attrDesc2.attrOffset, attrDesc1.attrLen, attrDesc2.attrLen, (Datatype)attrDesc1.attrType, op);
      if (diff == -1){
        return NOTUSED1;
      }else{
        if (diff){
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
            resultfile.insertRecord(joinresult, outRid);
            //delete [] joinresult.data;
          }

        }
      }

    }
    heapfile2.endScan();
  }
  heapfile1.endScan();

  return OK;
}
