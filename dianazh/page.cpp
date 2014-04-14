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
    //cout<<"--------------init"<<endl; //test
    slotCnt = 0;
    freePtr = 0;
    freeSpace = PAGESIZE - DPFIXED;
    prevPage = 0; //?
    nextPage = 0; //?
    curPage = pageNo;
    //cout<<"pageid: "<<curPage<<" slotCnt: "<<slotCnt<<endl; //test
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
    //cout<<"--------------insertRecord"<<rid.slotNo<<endl; //test
    int slot_cand = 1;  //index: <= 0
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
    //cout<<"--------------insertRecord slotNo: "<<rid.slotNo<<endl; //test
    slot[slot_cand].length = rec.length;
    slot[slot_cand].offset = freePtr;
    freePtr += rec.length;
    freeSpace -= rec.length;
    memcpy(data+slot[slot_cand].offset,rec.data, rec.length);
    //cout<<"pageid: "<<curPage<<" slotCnt: "<<slotCnt<<endl; //test
    //dumpPage(); //test
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
    //cout<<"--------------deleteRecord rid.slotNo: "<<rid.slotNo<<endl; //test
    
    if (rid.pageNo != curPage){
        return INVALIDSLOTNO;
    }else if (rid.slotNo >= (-slotCnt)){
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
    //slotCnt++;  //slot# never decrease
    
    //cout<<"pageid: "<<curPage<<" slotCnt: "<<slotCnt<<endl; //test
    return OK;
}

// returns RID of first record on page
// return OK on success and NORECORDS if no valid RID in page
const Status Page::firstRecord(RID& firstRid) const
{
    /* Solution Here */
    //cout<<"--------------firstRecord"<<endl; //test
    //cout<<"pageid: "<<curPage<<" slotCnt: "<<slotCnt<<endl; //test
    //dumpPage();  //test
    //what is first rec? offset = 1? +++ or first slot? 
    //in page offset
    /*for (int i=0; i>slotCnt; i--){
        if ((slot[i].length!=-1)&&(slot[i].offset==0)){
            firstRid.pageNo = curPage;
            firstRid.slotNo = -i;
            return OK;
        }
    }*/
    
    //in slot[]
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
    //cout<<"--------------nextRecord"<<endl; //test
    //cout<<"pageid: "<<curPage<<" slotCnt: "<<slotCnt<<endl; //test
    //dumpPage();  //test
    //in page offset
    int currOffset = slot[-curRid.slotNo].offset;
    int steplen = slot[-curRid.slotNo].length;
    /*for (int i=0; i>slotCnt; i--){
        if (slot[i].offset == currOffset + steplen){
            nextRid.pageNo = curRid.pageNo;
            nextRid.slotNo = -i;
            return OK;
        }
    }*/
    //check: curRid valid? 
    //in slot[]
    int curslot = curRid.slotNo;
    if (curRid.slotNo >= (-slotCnt)){
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
    //cout<<"--------------getRecord slotNo: "<<rid.slotNo<<endl; //test
    //cout<<"pageid: "<<curPage<<" slotCnt: "<<slotCnt<<endl; //test
    //dumpPage();//test
    if (rid.pageNo != curPage){
        //cout<<"case 1"<<endl; //test
        /*cout<<"slotno: "<<rid.slotNo<<" offset: "<<
            slot[-rid.slotNo].offset<<" "<<slot[-rid.slotNo].length<<endl; //test*/
        return INVALIDSLOTNO;
    } else if (rid.slotNo >= (-slotCnt)){
        /*cout<<"case 2"<<endl; //test
        cout<<"slotno: "<<rid.slotNo<<" offset: "<<
            slot[-rid.slotNo].offset<<" "<<slot[-rid.slotNo].length<<endl; //test*/
        return INVALIDSLOTNO;
    } else if (slot[-rid.slotNo].length == -1){
        /*cout<<"case 3"<<endl; //test
        cout<<"slotno: "<<rid.slotNo<<" offset: "<<
            slot[-rid.slotNo].offset<<" "<<slot[-rid.slotNo].length<<endl; //test*/
        return INVALIDSLOTNO;
    }
    rec.length = slot[-rid.slotNo].length;
    //cout<<"slotno: "<<rid.slotNo<<" offset: "<<
    //    slot[-rid.slotNo].offset<<" "<<slot[-rid.slotNo].length<<endl; //test
    //memcpy(rec.data, (data+slot[rid.slotNo].offset), rec.length);
    rec.data = (void*) data+slot[-rid.slotNo].offset;
    return OK;
}
