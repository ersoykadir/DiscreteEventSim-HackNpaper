
#ifndef EVENT_H
#define EVENT_H

#include "Hacker.h"
#include "Desk.h"
#include <queue>
#include <vector>
#include <iostream>
using namespace std;

class Event {
public:
    class CompareHackers {
    public:
        bool operator() (Hacker const *h1,Hacker const *h2) {
            //comparison code here
            if(h1->validCommits == h2->validCommits){
                if(abs(h1->hoodieQueueEntrance - h2->hoodieQueueEntrance) < 0.00001){
                    return (h1->hackerID > h2->hackerID);
                }
                else{
                    return (h1->hoodieQueueEntrance - h2->hoodieQueueEntrance) > 0.00001;
                }
            }
            else{
                return (h1->validCommits < h2->validCommits) ;
            }
        }
    };
    class CompareEvents{
    public:
        bool operator() (Event const *e1,Event const  *e2) {
            if (abs(e1->time - e2->time)<0.00001) {
                return e1->owner->hackerID > e2->owner->hackerID;
            } else {
                return (e1->time - e2->time) > 0.00001;
            }
        }
    };
    static priority_queue<Event*,vector<Event*>,CompareEvents> eventHeap;
    static queue<Hacker*>stickerQ;
    static priority_queue<Hacker*,vector<Hacker*>,CompareHackers>hoodieQ;
    static vector<Desk*>stickerDesks;
    static vector<Desk*>hoodieDesks;
    static float toplam;
    float time;
    Hacker* owner = NULL;
    static int numOfInvalidQattempts ;
    static int numOfGiftAbuse ;
    static int maxSQsize ;
    static int maxHQsize ;
    virtual void process() = 0;
    Event(Hacker *_owner,float _time);
    Event(const Event& event);
    Event& operator=(const Event& event);
    virtual ~Event();
};

class arrivalEvent : public Event{

public:
    arrivalEvent(Hacker *_owner,float _time);
    void process();
    ~arrivalEvent();
};

class commitEvent : public Event{

public:
    int numOfLines;
    commitEvent(Hacker *_owner,int _numOfLines,float _commitTime);//CANT INITIALIZE PARENT CLASS VARIABLES, CALL PARENTS CONSTRUCTOR
    void process();
    ~commitEvent();
};

class qAttemptEvent : public Event{

public:
    qAttemptEvent(Hacker *_owner,float _qAttemptTime);
    void process();
    ~qAttemptEvent();
};

class leaveSDEvent : public Event{

public:
    leaveSDEvent(Hacker *_owner,float _leavingTime);
    void process();
    ~leaveSDEvent();
};

 class leaveHDEvent : public Event{

 public:
     leaveHDEvent(Hacker *_owner,float _leavingTime);
     void process();
     ~leaveHDEvent();
 };


#endif //EVENT_H