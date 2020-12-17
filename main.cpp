#include <iostream>
#include "Hacker.h"
#include "Event.h"
#include "Desk.h"
#include <fstream>
#include <queue>
#include <vector>
#include <iomanip>

using namespace std;



int main(int argc, char* argv[]) {
    //valgrind --tool=memcheck --leak-check=full --track-origins=yes ./main.out input.txt output.txt
    string infile_name = argv[1];//READ INPUT FILE NAME
    string outfile_name = argv[2];//READ OUTPUT FILE NAME

    //OPEN INPUT AND OUTPUT FILE STREAMS
    ifstream infile;
    infile.open(infile_name);
    ofstream outfile;
    outfile.open(outfile_name);

    int numOfHackers;//Reading number of hackers
    infile >> numOfHackers;
    vector<Hacker*> hackers;//creating a hacker list to store hacker objects

    for (int i = 1; i <= numOfHackers; i++)
    {
        //Create hackers with consecutive ID's put in hackers LIST
        Hacker *h = new Hacker(i);
        hackers.push_back(h);

        float arrivalTime;
        infile >> arrivalTime;//Reading arrival times of hackers
        //Create arrival events for hackers consecutively
        arrivalEvent *e = new arrivalEvent(h,arrivalTime);
        Event::eventHeap.push(e);
    }

    int totalCommits;//Total number of codes commited by hackers
    int sumOfCommitLengths=0;//Recording sum of all commits' lengths for output
    infile >> totalCommits;
    for (int i = 0; i < totalCommits; i++)
    {
        int hackerID,numOfLines;
        float commitTime;
        infile >> hackerID >> numOfLines >> commitTime;
        sumOfCommitLengths+= numOfLines;
        //Create commit events for given hackers with given number of lines and time
        commitEvent *e1 = new commitEvent(hackers[hackerID-1],numOfLines,commitTime);
        Event::eventHeap.push(e1);//PUT EVENT TO HEAP
    }

    int numOfQattempts;//Total number of attempts to enqueue
    infile >> numOfQattempts;
    for (int i = 0; i < numOfQattempts; ++i)
    {
        int hackerID;
        float attemptTime;
        infile >> hackerID >> attemptTime;
        //Create queue attempt event for given hacker with given time
        qAttemptEvent *e2 = new qAttemptEvent(hackers[hackerID-1],attemptTime);
        Event::eventHeap.push(e2);//PUT EVENT TO HEAP
    }

    int numOfStickerDesks;
    infile >> numOfStickerDesks;
    for (int i = 0; i < numOfStickerDesks; ++i)
    {
        float serviceTime;
        infile >> serviceTime;
        //Create sticker desks with given service times
        Desk *d = new Desk(i,serviceTime);
        Event::stickerDesks.push_back(d);
    }

    int numOfHoodieDesks;
    infile >> numOfHoodieDesks;
    for (int i = 0; i < numOfHoodieDesks; ++i)
    {
        float serviceTime;
        infile >> serviceTime;
        //Create hoodie desks with given service times
         Event::hoodieDesks.push_back(new Desk(i,serviceTime));
    }
    // EVENT SIMULATION 
    float lastEventTime = 0;
    while(!Event::eventHeap.empty()){
        Event *e = Event::eventHeap.top();//Get the event on top of the queue
        e->process();//Process the event and change the state of simulation
        if(Event::eventHeap.empty()){
            lastEventTime = e->time;//Recording last event time for output
        }
        delete Event::eventHeap.top();//Delete event from memory
        Event::eventHeap.pop();//Delete the event from queue
    }

    // float sumOfTurnAroundTimes = 0;//Recording sum of all turnaround times for output
    // for (int j = 0; j < Event::qE.size(); ++j) {
    //     sumOfTurnAroundTimes+=Event::dL[j]-Event::qE[j];
    // }
    float avgNumOfCommits = (float)totalCommits/hackers.size();
    float avgCommitLength = (float)sumOfCommitLengths/totalCommits;

    float totalNumOfGiftsPerHacker = 0;
    float totalWaitingStickerQ = 0;
    int totalQattempts = 0;
    float totalWaitingHoodieQ = 0;
    int IDofMostWaitedHacker=-1;
    float mostTimeSpentInQ=-1;
    int IDofLeastWaitedHacker=-1;
    float leastTimeSpentInQ = -1;
    bool notFound = true;

    for(auto h : hackers){
        totalNumOfGiftsPerHacker += h->gifts;
        totalWaitingStickerQ += h->tot(h->stickerQueueTimeSpent);
        totalQattempts += h->stickerQueueEntrances;
        totalWaitingHoodieQ += h->tot(h->hoodieQueueTimeSpent);
        float totalQtime = h->tot(h->stickerQueueTimeSpent)+ h->tot(h->hoodieQueueTimeSpent);
        if( (totalQtime - mostTimeSpentInQ) > 0.00001){
            IDofMostWaitedHacker = h->hackerID;
            mostTimeSpentInQ = h->tot(h->stickerQueueTimeSpent)+ h->tot(h->hoodieQueueTimeSpent);
        }
        if(h->gifts>=3){
            if(notFound || (leastTimeSpentInQ - totalQtime) > 0.00001){
                notFound = false;
                IDofLeastWaitedHacker = h->hackerID;
                leastTimeSpentInQ = h->tot(h->stickerQueueTimeSpent)+ h->tot(h->hoodieQueueTimeSpent);
            }
        }
    }

    //sumOfTurnAroundTimes /= (float)totalNumOfGiftsPerHacker;//to calculate before division of gifts
    float avgNumOfGiftsPerHacker = totalNumOfGiftsPerHacker/hackers.size();
    float avgWaitingStickerQ = totalWaitingStickerQ/totalQattempts;
    float avgWaitingHoodieQ = totalWaitingHoodieQ/totalQattempts;
    outfile << fixed;
    outfile << setprecision(3) << Event::maxSQsize << endl;
    outfile << setprecision(3) << Event::maxHQsize << endl;
    outfile << setprecision(3) << avgNumOfGiftsPerHacker << endl;
    outfile  << setprecision(3) << avgWaitingStickerQ << endl;
    outfile << setprecision(3) << avgWaitingHoodieQ << endl;
    outfile << setprecision(3) << avgNumOfCommits << endl;
    outfile << setprecision(3) << avgCommitLength << endl;
    outfile << setprecision(3) << Event::toplam/totalNumOfGiftsPerHacker << endl;
    outfile << setprecision(3) << Event::numOfInvalidQattempts << endl;
    outfile << setprecision(3) << Event::numOfGiftAbuse << endl;
    outfile << setprecision(3) << IDofMostWaitedHacker << " " << mostTimeSpentInQ << endl;
    outfile << setprecision(3) << IDofLeastWaitedHacker<< " " << leastTimeSpentInQ <<endl;
    outfile << setprecision(3) << lastEventTime<<endl ;
    infile.close();
    outfile.close();
    for (int i = 0; i < hackers.size(); ++i)
    {
        delete hackers[i];
        hackers[i] = NULL;
    }
    hackers.clear();
    for (int i = 0; i < Event::stickerDesks.size(); ++i)
    {
        delete Event::stickerDesks[i];
        Event::stickerDesks[i] = NULL;
    }
    Event::stickerDesks.clear();
    for (int i = 0; i < Event::hoodieDesks.size(); ++i)
    {
        delete Event::hoodieDesks[i];
        Event::hoodieDesks[i] = NULL;
    }
    Event::hoodieDesks.clear();
    return 0;
}