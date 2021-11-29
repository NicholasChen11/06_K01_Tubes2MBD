#include <iostream>
#include <string>
#include <vector>
#include "Action.hpp"

using namespace std;


int main() {
    try{
        Transaction one(1);
        Transaction two(2);
        Transaction three(3);

        Action act1(one, "start", '-');
        Action act2(one, "read", 'x'); 
        Action act3(two, "start", '-');
        Action act4(two, "validate", '-');
        Action act5(two, "write", 'x');
        Action act6(two, "write", 'y');
        Action act7(three, "start", '-');
        Action act8(three, "validate", '-');
        Action act9(three, "write", 'y');
        Action act10(one, "validate", '-');
        Action act11(one, "write", 'y');
        Action act12(one, "finish", '-');
        Action act13(two, "finish", '-');
        Action act14(three, "finish", '-');

        vector<Action> actions;

        actions.push_back(act1);
        actions.push_back(act2); 
        actions.push_back(act3);
        actions.push_back(act4);
        actions.push_back(act5);
        actions.push_back(act6);
        actions.push_back(act7);
        actions.push_back(act8);
        actions.push_back(act9);
        actions.push_back(act10);
        actions.push_back(act11);
        actions.push_back(act12);
        actions.push_back(act13);
        actions.push_back(act14);

        vector<Transaction> order;
        vector<Transaction> rollback;
        vector<int> finishTime;
        int i = 1;
        for(Action b: actions) {
            if(b.getType() == "start") {
                b.getTransaction().setStart(i);
            }
            else if(b.getType() == "finish") {
                b.getTransaction().setFinish(i);
            }
            else if(b.getType() == "read") {
                b.getTransaction().addRead(b.getObject());
            }
            else if(b.getType() == "write") {
                b.getTransaction().addWrite(b.getObject());
            }
            ++i;
        }
        i = 0;
        for(Action b: actions) {
            if(b.getType() == "validate") {
                b.getTransaction().setValidate(i);
                order.push_back(b.getTransaction());
            }
            ++i;
        }

        one.printData();
        cout << "\n";
        two.printData();
        cout << "\n";
        three.printData();
        cout << "\n";

        bool tFinish = false;
        while(!tFinish) {
            int j = 0;
            for(Transaction x: order) {
                if(x==order.at(0)) {
                    cout << "Transaksi T" << x.getID() << " berhasil dijalankan" << endl; 
                } else {
                    bool berhasil = true;
                    for(int k = 0; k < j; ++k) {
                        if(order.at(k).getFinishTime() < x.getStartTime()) {
                            cout << "Transaksi T" << x.getID() << " berhasil divalidasi dengan " << 
                            "T" << order.at(k).getID() << "." << endl;
                        }
                        else if(order.at(k).getValidateTime() > x.getStartTime() 
                            && order.at(k).getValidateTime() < x.getFinishTime()) {
                            if(x.validateConflict(order.at(k)).size() > 0) {
                                cout << "Transaksi T" << x.getID() << " gagal divalidasi dengan " << "T" << 
                                order.at(k).getID() << " dan harus dirollback." << endl;
                                berhasil = false;
                            }
                            else {
                                cout << "Transaksi T" << x.getID() << " berhasil divalidasi dengan " << 
                                "T" << order.at(k).getID() << "." << endl;
                            }
                        }
                    }
                    if(berhasil) {
                        cout << "Transaksi T" << x.getID() << " berhasil dijalankan" << endl; 
                    }
                    else {
                        rollback.push_back(x);                    
                    }
                }
                ++j;
            }
            
            if(rollback.size() > 0) {
                cout << "----ROLLBACK----" << endl; 
            }

            order.clear();
            for(Transaction y: rollback) {
                order.push_back(y);
            }
            rollback.clear();
            tFinish = order.empty();
        }

    } catch(const char* msg) {
        cerr << msg << endl;
    }

    return 0;
}