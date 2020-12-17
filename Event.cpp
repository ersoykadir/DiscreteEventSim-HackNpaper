#include "Event.h"
#include "Hacker.h"
#include "Desk.h"
#include <vector>
#include <iostream>
using namespace std;
priority_queue<Event*,vector<Event*>,Event::CompareEvents> Event::eventHeap;
queue<Hacker*>Event::stickerQ;
priority_queue<Hacker*,vector<Hacker*>,Event::CompareHackers>Event::hoodieQ;
vector<Desk*>Event::stickerDesks;
vector<Desk*>Event::hoodieDesks;
int Event::maxSQsize =0;
int Event::maxHQsize =0;
int Event::numOfInvalidQattempts = 0;
int Event::numOfGiftAbuse =0;
float Event::toplam = 0;
//vector<float> Event::qE;
//vector<float> Event::dL;
Event::Event(Hacker *_owner,float _time) {
    //cout << "Constructing Event"<< endl;
    this->owner = _owner;
    this->time = _time;
}
Event::Event(const Event& event) {
    //cout << "Copy constructor Event is called." << endl;
    this->owner = new Hacker(-1);
    this->owner = event.owner;
    this->time = event.time;
    this->toplam = event.toplam;
    this->numOfInvalidQattempts = event.numOfInvalidQattempts;
    this->numOfGiftAbuse = event.numOfGiftAbuse;
    this->maxHQsize = event.maxHQsize;
    this->maxSQsize = event.maxSQsize;
}
Event& Event::operator=(const Event& event) {
    //cout << "AA Event is called." << endl;
    if (this == &event) {//If they are the same event objects
        return *this;
    }
    if (this->owner != NULL) { //Deleting previously used health history array
        //cout << "Deleting previously used (Assignment operator)" << endl;
        delete this->owner;
    }
    this->owner = new Hacker(-1);
    this->owner = event.owner;
    this->time = event.time;
    this->toplam = event.toplam;
    this->numOfInvalidQattempts = event.numOfInvalidQattempts;
    this->numOfGiftAbuse = event.numOfGiftAbuse;
    this->maxHQsize = event.maxHQsize;
    this->maxSQsize = event.maxSQsize;
    return *this;
}
Event::~Event() {
    //cout << "Destructing event"<< endl;
    //delete this->owner;
}

arrivalEvent::arrivalEvent(Hacker *_owner,float _time)
        : Event(_owner,_time)
{
    //cout << "Constructing arrivalEvent"<< endl;
}
arrivalEvent::~arrivalEvent() {
    //cout << "Destructing arrivalEvent"<< endl;
    //delete this->owner;
}
void arrivalEvent:: process()
{
    //this->owner.arrive();
    this->owner->isArrived = true;
    this->owner->arrivalTime = this->time;
}
commitEvent::commitEvent(Hacker *_owner,int _numOfLines,float _commitTime)
        : Event(_owner,_commitTime)
{
    //cout << "Constructing commitEvent"<< endl;
    this->numOfLines = _numOfLines;
}
void commitEvent:: process()
{
    //this->owner.arrive();
    if(this->owner->isArrived){
        this->owner->commit(this->numOfLines);
    }
}
commitEvent::~commitEvent() {
    //cout << "Destructing commitEvent"<< endl;
    //delete this->owner;
}

qAttemptEvent::qAttemptEvent(Hacker *_owner,float _qAttemptTime)
        : Event(_owner,_qAttemptTime)
{
    //cout << "Constructing qAttemptEvent"<< endl;
}
void qAttemptEvent:: process() {

    if (this->owner->canEnqueue()){
        this->owner->stickerQueueEntrance = this->time;//Queue Entrance time
        //toplam -= this->time;
        //this->qE.push_back(this->time);
        this->owner->stickerQueueEntrances++;
        bool performed = false;//Are there any available sticker desk
        for (auto desk : stickerDesks) {
            if (desk->isAvailable) {
                performed = true;//YEEES!!
                desk->isAvailable = false;//Desk occupied by this hacker
                desk->customerID = this->owner->hackerID;
                //Desk leaving event created
                this->owner->stickerQueueLeft = this->time;
                float timeSpent = this->owner->stickerQueueLeft - this->owner->stickerQueueEntrance;
                this->owner->stickerQueueTimeSpent.push_back(timeSpent);
                eventHeap.push(new leaveSDEvent(this->owner, this->time + desk->serviceTime));
                break;
            }
        }
        if (!performed) {//No available desk
            this->owner->inStickerQ=true;//Can still commit
            stickerQ.push(this->owner);
            if(stickerQ.size()>maxSQsize) maxSQsize = stickerQ.size();//max length of sticker output
        }
    }
}
qAttemptEvent::~qAttemptEvent() {
    //cout << "Destructing qAttemptEvent"<< endl;
    //delete this->owner;
}

leaveSDEvent::leaveSDEvent(Hacker *_owner,float _leavingTime)
        : Event(_owner,_leavingTime)
{
    //cout << "Constructing leaveSDEvent"<< endl;
}
void leaveSDEvent:: process()
{
    if(this->owner->isArrived) {
        bool performed = false;
        this->owner->hoodieQueueEntrance = this->time;
        for (auto hDesk : hoodieDesks) {//Search in hoodie desks
            if (hDesk->isAvailable) {
                performed = true;
                hDesk->isAvailable = false;
                hDesk->customerID = this->owner->hackerID;
                this->owner->hoodieQueueLeft = this->time;
                float timeSpent = this->owner->hoodieQueueLeft - this->owner->hoodieQueueEntrance;
                this->owner->stickerQueueTimeSpent.push_back(timeSpent);
                eventHeap.push(new leaveHDEvent(this->owner,this->time+hDesk->serviceTime));
                break;
            }
        }
        if (!performed) {//NO available desks
            //this->owner->hoodieQueueEntrance = this->time;
            this->owner->inHoodieQ = true;
            hoodieQ.push(this->owner);
            if(hoodieQ.size()>maxHQsize) maxHQsize = hoodieQ.size();//max length of hoodie, output
        }

       //Put someone from sticker Q to desk you left
        for(auto sDesk : stickerDesks){
            if(sDesk->customerID == this->owner->hackerID){
                if(!stickerQ.empty()) {
                    Hacker *h = stickerQ.front();
                    stickerQ.pop();
                    sDesk->customerID = h->hackerID;
                    h->inStickerQ = false;
                    h->stickerQueueLeft = this->time;
                    float timeSpent = h->stickerQueueLeft - h->stickerQueueEntrance;
                    h->stickerQueueTimeSpent.push_back(timeSpent);
                    eventHeap.push(new leaveSDEvent(h, this->time + sDesk->serviceTime));
                }
                else{
                    sDesk->isAvailable = true;
                    sDesk->customerID = -1;
                }
                break;
            }
        }
    }
}
leaveSDEvent::~leaveSDEvent() {
    //cout << "Destructing leaveSDEvent"<< endl;
    //delete this->owner;
}
leaveHDEvent::leaveHDEvent(Hacker *_owner,float _leavingTime)
        : Event(_owner,_leavingTime)
{
    //cout << "Constructing leaveSDEvent"<< endl;
}
void leaveHDEvent:: process() {
    if (this->owner->isArrived){
        this->owner->gifts++;
        this->owner->hoodieDeskLeaving = this->time;
        //toplam += this->time;
        //this->dL.push_back(this->time);
        toplam += this->time - this->owner->stickerQueueEntrance;
        float timeSpent = this->owner->hoodieDeskLeaving - this->owner->stickerQueueEntrance;
        //this->owner->turnAroundTime.push_back(timeSpent);
        for (auto hDesk : hoodieDesks) {
            if (this->owner->hackerID == hDesk->customerID) {
                if (!hoodieQ.empty()) {
                    Hacker *h = hoodieQ.top();
                    hoodieQ.pop();
                    hDesk->customerID = h->hackerID;
                    h->inHoodieQ = false;
                    h->hoodieQueueLeft = this->time;
                    float timeSpent = h->hoodieQueueLeft - h->hoodieQueueEntrance;
                    h->hoodieQueueTimeSpent.push_back(timeSpent);
                    eventHeap.push(new leaveHDEvent(h,this->time + hDesk->serviceTime));
                }
                else{
                    hDesk->isAvailable = true;
                    hDesk->customerID = -1;
                }
                break;
            }
        }
    }
}
leaveHDEvent::~leaveHDEvent() {
    //cout << "Destructing leaveSDEvent"<< endl;
    //delete this->owner;
}
