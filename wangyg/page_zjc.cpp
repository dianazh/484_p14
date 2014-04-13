#include <iostream>
#include <string.h>
#include <vector>
#include <algorithm>  
#include "page.h"


using namespace std;


// page class constructor
// please initialize all private data members. Note that the
// page starts off empty, dummy should NOT be touched, and there
// are no initial entries in slot array.
struct shift_entry{
  short slotIndex;  //this is a non-positive number
  short offset;
  short length;
  shift_entry(short slotIndex, short offset, short length):
  slotIndex(slotIndex), offset(offset), length(length){}
};


struct compare{
  bool operator() (const shift_entry &left, const shift_entry &right) 
  { return left.offset < right.offset;}
};




void Page::init(int pageNo)
{
    /* Solution Here */
  //cout << "init the page" << pageNo << endl;
  slot[0].length = -1;
  slot[0].offset = 0;
  slotCnt = 1;
  freePtr = 0;
  freeSpace = PAGESIZE - DPFIXED;
  curPage = pageNo;
}


// dump page utlity
void Page::dumpPage() const
{
  int i;
  cout << "curPage = " << curPage <<", nextPage = " << nextPage
       << "\nfreePtr = " << freePtr << ",  freeSpace = " << freeSpace 
       << ", slotCnt = " << slotCnt << endl;
    
    for (i=0;i>-slotCnt;i--)
      cout << "slot[" << i << "].offset = " << slot[i].offset 
	   << ", slot[" << i << "].length = " << slot[i].length << endl;
}


const int Page::getPrevPage() const
{
   return prevPage;
}


void Page::setPrevPage(int pageNo)
{
    prevPage = pageNo;
}


void Page::setNextPage(int pageNo)
{
    nextPage = pageNo;
}


const int Page::getNextPage() const
{
    return nextPage;
}


const short Page::getFreeSpace() const
{
    /* Solution Here */
  return freeSpace;
}
    
// Add a new record to the page. Returns OK if everything went OK
// otherwise, returns NOSPACE if sufficient space does not exist.
// RID of the new record is returned via rid parameter.
// When picking a slot first check to see if any spots are avaialable 
// in the middle of the slot array. Look from least negative to most 
// negative.
const Status Page::insertRecord(const Record & rec, RID& rid)
{
    /* Solution Here */
    if (rec.length > freeSpace)
      return NOSPACE;


    //void bcopy(const void *sor1, void *des2, size_t n);
    for (int i = 0; i > -slotCnt; i--){
        short length = slot[i].length;
        if (length == -1){
          rid.pageNo = curPage;
          rid.slotNo = -i + 1;
          slot[i].offset = freePtr;
          slot[i].length = rec.length;
          memcpy(data + freePtr, rec.data, rec.length);
          freeSpace -= rec.length;
          freePtr += rec.length;
          return OK;
        }
    }


    if (rec.length > freeSpace - 4)
      return NOSPACE;


    slot[-slotCnt].offset = freePtr;
    slot[-slotCnt].length = rec.length;
    rid.pageNo = curPage;
    rid.slotNo = ++slotCnt;
    memcpy(data + freePtr, rec.data, rec.length);
    freeSpace -= (rec.length + 4);
    freePtr += rec.length;
    return OK;
}




// delete a record from a page. Returns OK if everything went OK,
// if invalid RID passed in return INVALIDSLOTNO
// if the record to be deleted is last record on page return NORECORDS
// compacts remaining records but leaves hole in slot array
// use bcopy and not memcpy when shifting overlapping memory. 
const Status Page::deleteRecord(const RID & rid)
{
    /* Solution Here */
    short slotNo = rid.slotNo;
    if (curPage != rid.pageNo || slotNo > slotCnt || slotNo < 1) return INVALIDSLOTNO;
    short offset = slot[-slotNo + 1].offset;
    short length = slot[-slotNo + 1].length;
    slot[-slotNo + 1].length = -1;
    vector<shift_entry> shifting_entries;
    bool is_last = true;


    for (int index = 0; index > -slotCnt; index--){
      short curr_length = slot[index].length;
      short curr_offset = slot[index].offset;
      if (curr_length != -1) is_last = false;
      if (curr_length != -1 && curr_offset > offset)
        shifting_entries.push_back(shift_entry(index, curr_offset, curr_length));
    }


    if (is_last) { 
      freeSpace += length; 
      freePtr = 0; 
      return NORECORDS;
    }
    compare cp;
    sort(shifting_entries.begin(), shifting_entries.end(), cp);


    short prev_offset = offset;
    for (int i = 0; i < shifting_entries.size(); i++){
      short index = shifting_entries[i].slotIndex;
      short curr_offset = slot[index].offset;
      bcopy(data + curr_offset, data + prev_offset, slot[index].length);
      slot[index].offset = prev_offset;
      prev_offset += slot[index].length;
    }
    freeSpace += length;
    freePtr = prev_offset;
    return OK;
}


// returns RID of first record on page
// return OK on success and NORECORDS if no valid RID in page
const Status Page::firstRecord(RID& firstRid) const
{
    /* Solution Here */
    for (int i = 0; i > -slotCnt; i--){
        short length = slot[i].length;
        if (length != -1){
          firstRid.pageNo = curPage;
          firstRid.slotNo = -i + 1;
          return OK;
        }
    }
    return NORECORDS;
}


// returns RID of next record on the page
// returns ENDOFPAGE if no more records exist on the page; otherwise OK
const Status Page::nextRecord (const RID &curRid, RID& nextRid) const
{
    /* Solution Here */
    if (curRid.pageNo != curPage && curRid.slotNo <= 0) return ENDOFPAGE;


    for (int i = -curRid.slotNo; i > -slotCnt; i--){
      short length = slot[i].length;
      if (length != -1){
        nextRid.pageNo = curPage;
        nextRid.slotNo = -i + 1;
        return OK;
      }
    }
    return ENDOFPAGE;
}


// returns length and pointer to record with RID rid
// returns OK on success and INVALIDSLOTNO if invalid rid 
const Status Page::getRecord(const RID & rid, Record & rec)
{
    /* Solution Here */
    short slotNo = rid.slotNo;
    if (curPage != rid.pageNo || slotNo > slotCnt || slotNo < 1) return INVALIDSLOTNO;
    short length = slot[-rid.slotNo + 1].length;
    short offset = slot[-rid.slotNo + 1].offset;
    if (length == -1)
      return INVALIDSLOTNO;


    memcpy(rec.data, data + offset, length);
    rec.length = length;
    return OK;
}
