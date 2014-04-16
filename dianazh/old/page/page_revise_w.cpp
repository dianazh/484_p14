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
    memset(data, 0, PAGESIZE - DPFIXED);
    slotCnt = 0;
    freePtr = 0;
    freeSpace = PAGESIZE - DPFIXED; // + sizeof(slot_t);
    //dummy = 0;
    prevPage = 0;
    nextPage = 0;
    curPage = pageNo;
    dumpPage();  //test
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
    /* Solution Here */
    //get slot
    int slotstart = 0;
    int getslot = 1;
    for (int i=0; i<slotCnt; i++){
        if (slot[slotstart-i].length == -1){
            getslot = slotstart-i;
        }
    }
    if ((getslot == 1) && (freeSpace < rec.length + sizeof(slot_t))){
        return NOSPACE;
    } else if ((getslot != 1) && (freeSpace < rec.length))
        return NOSPACE;
    }else{
        // find empty slot
        if (getslot == 1){
            //new slot block
            getslot = slotstart-slotCnt;
            freeSpace -= sizeof(slot_t);
        }
        slotCnt++;
        freePtr += rec.length;
        freeSpace -= rec.length;
        slot[getslot].length = rec.length;
        slot[getslot].offset = freePtr;

        //rec
        rid.pageNo = curPage;
        rid.slotNo = 

        
        int i = 0;
        for (i = 0; i > slotCnt; i--){
            if (slot[i].length == -1){
                slot[i].length = rec.length;



                freeSpace -= (slot[i].length + sizeof(slot_t));
                rid.pageNo = curPage;
                rid.slotNo = (slot[i].offset / slot[i].length) + 1;
                memcpy(data+slot[i].offset, rec.data, slot[i].length);
                break;
            }
        }
        if (i == slotCnt){
            slot[i].length = rec.length;
            slot[i].offset = freePtr;
            freePtr += slot[i].length;
            slotCnt--;
            freeSpace -= (slot[i].length + sizeof(slot_t));
            rid.pageNo = curPage;
            rid.slotNo = (slot[i].offset / slot[i].length) + 1;
            memcpy(data+slot[i].offset, rec.data, slot[i].length);
        }
        return OK;
    }
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
    }
    int i = 0;
    int slotNo;
    for (i = 0; i > slotCnt; i--){

        if (slot[i].length != -1){
            slotNo = (slot[i].offset / slot[i].length) + 1;
            if (slotNo == rid.slotNo){
                freeSpace += (slot[i].length + sizeof(slot_t));
                slot[i].length = -1;
                return OK;
            }
        }
    }
    return INVALIDSLOTNO;
}

// returns RID of first record on page
// return OK on success and NORECORDS if no valid RID in page
const Status Page::firstRecord(RID& firstRid) const
{
    /* Solution Here */
    int i;
    for (i = 0; i > slotCnt; i--){
        if (slot[i].length != -1){
            firstRid.pageNo = curPage;
            firstRid.slotNo = slot[i].offset / slot[i].length + 1;
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
    if (curRid.pageNo != curPage){
        return INVALIDSLOTNO;
    }
    int i = 0, j = 0;
    int slotNo;
    for (i = 0; i > slotCnt; i--){

        if (slot[i].length != -1){
            slotNo = (slot[i].offset / slot[i].length) + 1;
            if (slotNo == curRid.slotNo){
                break;
                //return OK;
            }
        }
    }
    for (j = i-1; j > slotCnt; j--){
        if (slot[j].length != -1){
            nextRid.pageNo = curPage;
            nextRid.slotNo = (slot[j].offset / slot[j].length) + 1;
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
    if (rid.pageNo != curPage){
        return INVALIDSLOTNO;
    }
    int i = 0, j = 0;
    int slotNo;
    for (i = 0; i > slotCnt; i--){

        if (slot[i].length != -1){
            slotNo = (slot[i].offset / slot[i].length) + 1;
            if (slotNo == rid.slotNo){
                rec.length = slot[i].length;
                rec.data =  (void*) new char [slot[i].length];
                memcpy(rec.data, data+slotNo-1, slot[i].length);
                return OK;
            }
        }
    }
    return INVALIDSLOTNO;
}
