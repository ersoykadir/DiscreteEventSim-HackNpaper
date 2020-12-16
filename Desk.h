//
// Created by cmpe250student on 14.12.2020.
//

#ifndef DESK_H
#define DESK_H


using namespace std;

class Desk {

public:
    int deskID;
    int customerID = -1;
    bool isAvailable = true;
    float serviceTime;
    Desk(int _deskID,float _serviceTime);
    Desk(const Desk& desk);
    Desk& operator=(const Desk& desk);
    ~Desk();
};


#endif //DESK_H