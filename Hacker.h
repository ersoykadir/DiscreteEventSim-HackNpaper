

#ifndef HACKER_H
#define HACKER_H

#include "Desk.h"
#include <vector>
using namespace std;

class Hacker {

public:
    int hackerID;//ID of hacker started from 1 and assigned consecutively
    vector<int> commitList;//List of every code hacker committed
    vector<float> stickerQueueTimeSpent;//list of wasted time in queue
    vector<float> hoodieQueueTimeSpent;
    vector<float> turnAroundTime;
    vector<float>stickerQueueEntranceL;
    vector<float>hoodieDeskLeavingL;
    float stickerQueueEntrance = -1;
    float hoodieQueueEntrance= -1;
    float stickerQueueLeft= -1;
    float hoodieQueueLeft= -1;
    float hoodieDeskLeaving= -1;
    int validCommits=0;//commits with more than 20 lines
    int gifts = 0;
    int stickerQueueEntrances = 0;
    bool isArrived = false;
    float arrivalTime = -1;
    bool inHoodieQ = false;
    bool inStickerQ = false;
    float avg(vector<float>v);
    float tot(vector<float>v);
    bool canEnqueue();
    bool canCommit();
    void commit(int lines);
    Hacker(int _hackerID);
    Hacker(const Hacker& hacker);
    Hacker& operator=(const Hacker& hacker);
    bool operator<(const Hacker& other);
    ~Hacker();
};


#endif //HACKER_H