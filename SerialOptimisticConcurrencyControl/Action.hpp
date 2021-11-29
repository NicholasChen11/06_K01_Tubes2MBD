#ifndef _Action_hpp_
#define _Action_hpp_

#include <string>
#include "Transaction.hpp"

using namespace std;

class Action {
    private:
        Transaction& T;
        string type;
        char object;
    
    public:
        Action(Transaction& T, string type, char object);
        Transaction& getTransaction();
        string getType();
        char getObject();
        void setObject(char a);
        void printAction();
};

#endif
