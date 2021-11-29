#include "Action.hpp"
#include <string>
#include <iostream>

using namespace std;


Action::Action(Transaction& T, string type, char object): T(T) {
    if(type != "read" && type != "write" && type != "start" && type != "validate" && type != "finish") {
        throw "Invalid action type! Action type must be either \"read\", \"write\", \"start\", \"validate\", or \"finish\"";
    }
    else {
        this->type = type;
        if(type == "start" || type == "validate" || type == "finish") {
            this->object = '-';
        }
        else {
            this->object = object;
        }
    }
} 

Transaction& Action::getTransaction() {
    return this->T;
}

string Action::getType() {
    return this->type;
}

char Action::getObject() {
    return this->object;
}

void Action::setObject(char a) {
    this->object = a;
}

void Action::printAction() {
    if(this->object == '-') {
        cout<<"T" << this->T.getID() << "." << this->type << endl;
    } else {
        cout<<"T" << this->T.getID() << "." << this->type << "(" << this->object << ")" << endl;
    }
}