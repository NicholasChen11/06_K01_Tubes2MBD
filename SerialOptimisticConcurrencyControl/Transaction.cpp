#include "Transaction.hpp"
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

Transaction::Transaction(int id): id(id), startTime(0), validateTime(0), finishTime(0) {}

int Transaction::getID() {
    return this->id;
}

int Transaction::getStartTime() {
    return this->startTime;
}

int Transaction::getValidateTime() {
    return this->validateTime;
}

int Transaction::getFinishTime() {
    return this->finishTime;
}

bool Transaction::operator==(const Transaction& rhs) {
    bool ret = true;
    int i = 0;

    if(this->readSet.size() == rhs.readSet.size()) {
        for(char object: this->readSet) {
            if(object != rhs.readSet.at(i)) {
                ret = false;
            }
            ++i;
        }
        i = 0;
    } else {
        return false;
    }

    if(this->writeSet.size() == rhs.writeSet.size()) {
        for(char object: this->writeSet) {
            if(object != rhs.writeSet.at(i)) {
                ret = false;
            }
            ++i;
        }
    } else {
        return false;
    }

    return ret && (this->id==rhs.id);
}


vector<char> Transaction::getReadSet() {
    return this->readSet;
}

vector<char> Transaction::getWriteSet() {
    return this->writeSet;
}

vector<char> Transaction::validateConflict(Transaction To) {
    vector<char> ret;
    for(char read: this->readSet) {
        for(char write: To.writeSet) {
            if(read == write) {
                ret.push_back(read);
                continue;
            }
        }
    }
    return ret;
}

void Transaction::setStart(int time) {
    this->startTime = time;
}

void Transaction::setValidate(int time) {
    this->validateTime = time;
}

void Transaction::setFinish(int time) {
    this->finishTime = time;
}

void Transaction::addRead(char object) {
    bool available = true;
    for(char a: this->readSet) {
        if (a == object) {
            available = false;
        }
    }
    if(available) {
        this->readSet.push_back(object);
    }
}

void Transaction::addWrite(char object) {
    bool available = true;
    for(char a: this->writeSet) {
        if (a == object) {
            available = false;
        }
    }
    if(available) {
        this->writeSet.push_back(object);
    }
}

void Transaction::printData() {
    cout << "T" << this->id <<  ":" << endl;
    cout << "Start time: " << this->startTime << endl;
    cout << "Validate time: " << this->validateTime << endl;
    cout << "Finish time: " << this->finishTime << endl; 
    cout << "Read set: ";
    for(char a:  this->readSet) {
        cout << a << " ";
    }
    cout << "\n";
    cout << "Write set: ";
    for(char a:  this->writeSet) {
        cout << a << " ";
    }
    cout << "\n";
}