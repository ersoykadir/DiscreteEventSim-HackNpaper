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

    int numOfHackers;
    infile >> numOfHackers;
    vector<Hacker*> hackers;

    for (int i = 1; i <= numOfHackers; i++)
    {
        //CREATE HACKER with ID = i;
        //PUT IN hackers LIST
        Hacker *h = new Hacker(i);
        hackers.push_back(h);
        float arrivalTime;
        infile >> arrivalTime;
        //CREATE ARRIVAL EVENT
        //PUT EVENT TO HEAP
        arrivalEvent *e = new arrivalEvent(h,arrivalTime);
        Event::eventHeap.push(e);
    }
    int totalCommits;
    int totalCommitLength=0;
    infile >> totalCommits;
    for (int i = 0; i < totalCommits; i++)
    {
        int hackerID,numOfLines;
        float commitTime;
        infile >> hackerID >> numOfLines >> commitTime;
        //CREATE COMMIT EVENT(hackerID,numOfLines,commitTime)
        totalCommitLength+= numOfLines;
        commitEvent *e1 = new commitEvent(hackers[hackerID-1],numOfLines,commitTime);
        Event::eventHeap.push(e1);
        //PUT EVENT TO HEAP
    }
    int numOfQattempts;
    infile >> numOfQattempts;
    for (int i = 0; i < numOfQattempts; ++i)
    {
        int hackerID;
        float attemptTime;
        infile >> hackerID >> attemptTime;
        //CREATE qAttempt EVENT
        //PUT EVENT TO HEAP
        qAttemptEvent *e2 = new qAttemptEvent(hackers[hackerID-1],attemptTime);
        Event::eventHeap.push(e2);
    }
    int numOfStickerDesks;
    infile >> numOfStickerDesks;
    for (int i = 0; i < numOfStickerDesks; ++i)
    {
        float serviceTime;
        infile >> serviceTime;
        //CREATE DESK WITH ID = i, serviceTime;
        //PUT IN stickerdesks LIST
        Desk *d = new Desk(i,serviceTime);
        Event::stickerDesks.push_back(d);
    }
    int numOfHoodieDesks;
    infile >> numOfHoodieDesks;
    for (int i = 0; i < numOfHoodieDesks; ++i)
    {
        float serviceTime;
        infile >> serviceTime;
        //CREATE DESK WITH ID = i, serviceTime;
        //PUT IN hoodiedesks LIST
         Event::hoodieDesks.push_back(new Desk(i,serviceTime));
    }
    float lastEventTime = 0;
    while(!Event::eventHeap.empty()){
        Event *e = Event::eventHeap.top();
        e->process();
        delete Event::eventHeap.top();
        Event::eventHeap.pop();
        if(Event::eventHeap.empty()){
            lastEventTime = e->time;
        }
    }
    float sum = 0;
    for (int j = 0; j < Event::qE.size(); ++j) {
        sum+=Event::dL[j]-Event::qE[j];
    }
    float x = Event::toplam;
    float avgNumOfGiftsPerHacker = 0;
    float avgWaitingStickerQ = 0;
    int totalSQattempts = 0;
    float avgWaitingHoodieQ = 0;
    int totalHQattempts = 0;
    float avgNumOfCommits = (float)totalCommits/hackers.size();
    float avgCommitLength = (float)totalCommitLength/totalCommits;
    int IDofMostWaitedHacker=-1;
    float mostTimeSpentInQ=-1;
    int IDofLeastWaitedHacker=-1;
    float leastTimeSpentInQ = -1;
    bool notFound = true;
    for(auto h : hackers){
        avgNumOfGiftsPerHacker += h->gifts;
        avgWaitingStickerQ += h->tot(h->stickerQueueTimeSpent);
        totalSQattempts += h->stickerQueueEntrances;
        avgWaitingHoodieQ += h->tot(h->hoodieQueueTimeSpent);
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

    sum /= (float)avgNumOfGiftsPerHacker;//to calculate before division of gifts
    avgNumOfGiftsPerHacker /= hackers.size();
    avgWaitingStickerQ /= totalSQattempts;
    avgWaitingHoodieQ /= totalSQattempts;
    outfile << fixed;
    outfile << setprecision(3) << Event::maxSQsize << endl;
    outfile << setprecision(3) << Event::maxHQsize << endl;
    outfile << setprecision(3) << avgNumOfGiftsPerHacker << endl;
    outfile  << setprecision(3) << avgWaitingStickerQ << endl;
    outfile << setprecision(3) << avgWaitingHoodieQ << endl;
    outfile << setprecision(3) << avgNumOfCommits << endl;
    outfile << setprecision(3) << avgCommitLength << endl;
    outfile << setprecision(3) << sum << endl;
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
    return 0;
}