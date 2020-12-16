#include "Desk.h"
#include <iostream>
using namespace std;

/*
	Constructor

*/
Desk::Desk(int _deskID,float _serviceTime) {
    this->deskID = _deskID;
    this->serviceTime = _serviceTime;
}

/*
	Copy constructor
*/Desk::Desk(const Desk& desk) {
    //cout << "Copy constructor is called." << endl;
    this->deskID = desk.deskID;
    this->customerID = desk.customerID;
    this->serviceTime = desk.serviceTime;
}
/*
	Overloading assignment operator
*/
Desk& Desk::operator=(const Desk& desk) {

    //cout << "Assinment operator called" << endl;
    if (this == &desk) {//If they are the same character objects
        return *this;
    }
    this->deskID = desk.deskID;
    this->customerID = desk.customerID;
    this->serviceTime = desk.serviceTime;
    return *this;
}

/*
	Destructor
*/
Desk::~Desk() {
    //cout << "Destructing"<< this->name << endl;
    //delete this->commitList;
}
