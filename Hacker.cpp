#include "Hacker.h"
#include "Desk.h"
#include "Event.h"
#include <vector>
#include <iostream>
using namespace std;

/*
    Constructor

*/
Hacker::Hacker(int _hackerID) {
    this->hackerID = _hackerID;
}

/*
    Copy constructor
*/Hacker::Hacker(const Hacker& hacker) {
    //cout << "Copy constructor is called." << endl;
    this->hackerID = hacker.hackerID;
    this->commitList = hacker.commitList;
    this->stickerQueueTimeSpent = hacker.stickerQueueTimeSpent;
    this->hoodieQueueTimeSpent = hacker.hoodieQueueTimeSpent;
    this->turnAroundTime = hacker.turnAroundTime;

    this->stickerQueueEntrance = hacker.stickerQueueEntrance;
    this->hoodieQueueEntrance = hacker.hoodieQueueEntrance;
    this->stickerQueueLeft = hacker.stickerQueueLeft;
    this->hoodieQueueLeft = hacker.hoodieQueueLeft;
    this->hoodieDeskLeaving = hacker.hoodieDeskLeaving;

    this->validCommits = hacker.validCommits;
    this->gifts = hacker.gifts;
    this->stickerQueueEntrances = hacker.stickerQueueEntrances;
    this->isArrived = hacker.isArrived;
    this->arrivalTime = hacker.arrivalTime;
    this->inHoodieQ = hacker.inHoodieQ;
    this->inStickerQ = hacker.inStickerQ;
}
/*
    Overloading assignment operator
*/
Hacker& Hacker::operator=(const Hacker& hacker) {

    //cout << "Assinment operator called" << endl;
    if (this == &hacker) {//If they are the same character objects
        return *this;
    }
    this->hackerID = hacker.hackerID;
    this->commitList = hacker.commitList;
    this->stickerQueueTimeSpent = hacker.stickerQueueTimeSpent;
    this->hoodieQueueTimeSpent = hacker.hoodieQueueTimeSpent;
    this->turnAroundTime = hacker.turnAroundTime;

    this->stickerQueueEntrance = hacker.stickerQueueEntrance;
    this->hoodieQueueEntrance = hacker.hoodieQueueEntrance;
    this->stickerQueueLeft = hacker.stickerQueueLeft;
    this->hoodieQueueLeft = hacker.hoodieQueueLeft;
    this->hoodieDeskLeaving = hacker.hoodieDeskLeaving;

    this->validCommits = hacker.validCommits;
    this->gifts = hacker.gifts;
    this->stickerQueueEntrances = hacker.stickerQueueEntrances;
    this->isArrived = hacker.isArrived;
    this->arrivalTime = hacker.arrivalTime;
    this->inHoodieQ = hacker.inHoodieQ;
    this->inStickerQ = hacker.inStickerQ;

    return *this;
}
/*
    Overloading comparison operator
*/
bool Hacker::operator<(const Hacker& other) {
    // wrooooooooooong compa
    if(this->validCommits == other.validCommits){
        return (this->hackerID < other.hackerID);
    }
    else{
        return (this->validCommits < other.validCommits) ;
    }
}

float Hacker::avg(vector<float> v) {
    float tot = 0;
    for(auto e : v){
        tot+=e;
    }
    return tot/v.size();
}

float Hacker::tot(vector<float> v) {
    float total = 0;
    for(auto e : v){
        total += e;
    }
    return total;
}
bool Hacker::canEnqueue(){
    if(!this->isArrived ) return false;//can hacker attempt enqueue in already que or desk?

    if (this->validCommits < 3) {//Have enough valid commits?
        Event::numOfInvalidQattempts++;
        return false;
    }
    if (this->gifts >= 3) {//Have taken enough gifts?
        Event::numOfGiftAbuse++;
        return false;
    }
    return true;
}
bool Hacker::canCommit() {//can commit in sticker desk
    if(this->inHoodieQ || !this->isArrived) return false;
    return true;
}
/*
 * Commits code, adds to commit List
 */
void Hacker::commit(int numOfLines){
    if(this->canCommit()){
        this->commitList.push_back(numOfLines);
        if(numOfLines >= 20) this->validCommits++;//increase valid commits
    }
}
/*
    Destructor
*/
Hacker::~Hacker() {
    //cout << "Destructing"<< this->name << endl;
    //delete this->commitList;
}
