#include <iostream>
#include <string.h>
#include "page.h"

using namespace std;

// page class constructor
// please initialize all private data members. Note that the
// page starts off empty, dummy should NOT be touched, and there
// are no initial entries in slot array.
void Page::init(int pageNo)
{
    /* Solution Here */
    slotCnt = 0;
    freePtr = 0;
    freeSpace = PAGESIZE - DPFIXED + sizeof(slot_t);
    prevPage = 0; 
    nextPage = 0; 
    curPage = pageNo;
}

// dump page utlity
void Page::dumpPage() const
{
  int i;
  cout << "curPage = " << curPage <<", nextPage = " << nextPage
       << "\nfreePtr = " << freePtr << ",  freeSpace = " << freeSpace 
       << ", slotCnt = " << slotCnt << endl;
    
    for (i=0;i>slotCnt;i--)
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
    //slotCnt is (-inf, -1,0]
    //slotNo is 0,1,2,3 -> new slotNo = -slotCnt, slotCnt --
    /* Solution Here */
    int slot_cand = 1;  //is equal to slotNO, index <= 0
    if (freeSpace < rec.length){
        return NOSPACE;
    } else {
        //old slot exist
        for (int i=0; i>slotCnt; i--){
            if (slot[i].length == -1){
                slot_cand =  i;
                break;
            }
        }
        if ((slot_cand) == 1 && (freeSpace < (rec.length+sizeof(slot_t)))){
            return NOSPACE;
        }
    }
    //new slot
    if (slot_cand == 1){
        slot_cand = slotCnt;
        freeSpace -= sizeof(slot_t);
        slotCnt --;
    }
    //update slotNo
    rid.slotNo = -slot_cand;
    rid.pageNo = curPage;
    slot[slot_cand].length = rec.length;
    slot[slot_cand].offset = freePtr;
    freePtr += rec.length;
    freeSpace -= rec.length;
    memcpy(data+slot[slot_cand].offset,rec.data, rec.length);
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
    if (rid.pageNo != curPage){
        return INVALIDSLOTNO;
    }else if (rid.slotNo >= (-slotCnt)||(rid.slotNo < 0)){
        return INVALIDSLOTNO;
    }  else if (slot[-rid.slotNo].length == -1){
        return INVALIDSLOTNO;
    }
    
    //compact data
    int steplen = slot[-rid.slotNo].length;
    int currOffset = slot[-rid.slotNo].offset;
    int originOffset = currOffset;

    freeSpace += slot[-rid.slotNo].length;
    freePtr -= steplen;
    slot[-rid.slotNo].length = -1;
    if (freePtr == 0){
        return NORECORDS;
    }
    
    while (currOffset < freePtr){
        bcopy(data+currOffset+steplen, data+currOffset, steplen);
        currOffset += steplen;
    }
    
    for (int i=0; i>slotCnt; i--){
        if ((slot[i].offset > originOffset)&&(slot[i].length != -1)){
            slot[i].offset -= steplen;
        }
    }
    //slot# never decrease
    return OK;
}

// returns RID of first record on page
// return OK on success and NORECORDS if no valid RID in page
const Status Page::firstRecord(RID& firstRid) const
{
    /* Solution Here */
    for (int i=0; i>slotCnt; i--){
        if (slot[i].length != -1){
            firstRid.pageNo = curPage;
            firstRid.slotNo = -i;
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
    int curslot = curRid.slotNo;
    if ((curRid.slotNo >= (-slotCnt))||curRid.slotNo < 0){
        return ENDOFPAGE;
    } else {
        curslot++;
        while (curslot!=(-slotCnt)){
            if (slot[-curslot].length != -1){
                nextRid.pageNo = curRid.pageNo;
                nextRid.slotNo = curslot;
                return OK;
            }
            curslot++;
        }
        return ENDOFPAGE;
    }
    return ENDOFPAGE;
}

// returns length and pointer to record with RID rid
// returns OK on success and INVALIDSLOTNO if invalid rid 
const Status Page::getRecord(const RID & rid, Record & rec)
{
    /* Solution Here */
    if (rid.pageNo != curPage){
        return INVALIDSLOTNO;
    } else if (rid.slotNo >= (-slotCnt)||(rid.slotNo < 0)){
        return INVALIDSLOTNO;
    } else if (slot[-rid.slotNo].length == -1){
        return INVALIDSLOTNO;
    }
    rec.length = slot[-rid.slotNo].length;
    rec.data = (void*) data+slot[-rid.slotNo].offset;
    return OK;
}
