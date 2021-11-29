#include <iostream>
#include <string>
#include <vector>
#include "Action.hpp"

using namespace std;


int main() {
    try{
        /* 
        Tuliskan transaksi-transaksi anda, one(1) berarti
        Transaksi bernama one dengan id: 1 (id dan nama harus unik)
        */
        Transaction one(1);
        Transaction two(2);

        /* 
        Action adalah bagian-bagian dari schedule, terdapat 5 jenis aksi:
        start, read, write, validate, dan finish. read dan write menerima objek terkait 
        dalam bentuk char, sementara aksi lain tidak memedulikan argumen char(dapat diisi apa saja).
        
        Act1(one, "read", 'b') berarti aksi milik dari transaksi "one", dengan metode "read", dengan 
        objek yang dibaca adalah b. Perhatikan penulisan char menggunakan kutip satu '' agar tidak salah.
        Pengisian selain dengan metode-metode yang diperbolehkan("read", "write", "start", "validate", dan "finish")
        akan menghasilkan error.
        */
        Action act1(one, "start", '-');
        Action act2(one, "read", 'b');
        Action act3(two, "start", '-');
        Action act4(two, "read", 'b');
        Action act5(two, "read", 'a');
        Action act6(one, "read", 'a');
        Action act7(one, "validate", '-');
        Action act8(one, "finish", 'b');
        Action act9(two, "validate", 'c');
        Action act10(two, "write", 'b');
        Action act11(two, "write", 'a');
        Action act12(two, "finish", '-');

        vector<Action> actions;

        /* 
        Masukkan seluruh aksi ke dalam actions dengan prosedur push_back().
        Perhatikan urutan push_back() mengubah urutan schedule.
        */
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
        
        /* Mulai dari bawah ini jangan diubah */

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

        /* 
        Tambahkan perintah di bawah untuk melihat anatomi transaksi.
        Perintah tidak mengubah hasil program.
        */
        one.printData();
        cout << "\n";
        two.printData();
        cout << "\n";
        
        /* Mulai dari bawah ini jangan diubah */

        /* Algoritma utama, maap ga elegan */
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
