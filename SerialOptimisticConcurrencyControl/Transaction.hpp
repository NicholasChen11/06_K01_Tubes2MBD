#ifndef _Transaction_hpp_
#define _Transaction_hpp_

#include <string>
#include <vector>

using namespace std;

class Transaction {
    private:
        int id;
        int startTime;
        int validateTime;
        int finishTime; 
        vector<char> readSet;
        vector<char> writeSet;

    public:
        Transaction(int id);
        int getID();
        int getStartTime();
        int getValidateTime();
        int getFinishTime();
        bool operator==(const Transaction& rhs);
        vector<char> getReadSet();
        vector<char> getWriteSet();
        vector<char> validateConflict(Transaction To);
        void setStart(int time);
        void setValidate(int time);
        void setFinish(int time); 
        void addRead(char object);
        void addWrite(char object);
        void printData();
};

#endif