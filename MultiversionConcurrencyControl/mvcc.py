def sortTuple(tup): 
    tup.sort(key = lambda x: x[1]) 
    return tup


class Transaction:
    def __init__(self,name,ts) -> None:
        self.name = name
        self.ts = ts
        self.fin = False
    
class Item:
    def __init__(self,name) -> None:
        self.name = name
        self.version = [(0,0)]

    def checkVersionW(self,ts,name):
        length = len(self.version)
        for i in range(length-1, -1, -1):
            if(self.version[i][1] <= ts):
                if(ts < self.version[i][0]):
                    print(f"Rollback transaksi {name}")
                    return False
                elif(ts == self.version[i][1]):
                    print(f"Overwrite isi item {self.name} versi {name}")
                    return True
                else:
                    self.version.append((ts,ts))
                    self.version = sortTuple(self.version)
                    print(f"Dibuat item {self.name} versi {name}")
                    return True
        return False
    
    def checkVersionR(self,ts,name):
        length = len(self.version)
        for i in range(length-1, -1, -1):
            if(self.version[i][1] <= ts):
                print(f"Read isi item {self.name} versi {self.version[i][1]}")
                if(self.version[i][0] < ts):
                    self.version[i] = (ts,self.version[i][1])
                break
        return True
    



class Action:
    def __init__(self,action,transaction,item) -> None:
        self.action = action
        self.transaction = transaction
        self.item = item


if __name__ == "__main__":
    isAbort = False
    n = int(input("Masukkan jumlah data item: "))
    items = []
    itemsI = []
    for i in range(n):
        input1 = str(input("Masukkan nama data item " + str(i+1) + ": "))
        item = Item(input1)
        items.append(item)
        itemsI.append(input1)
    n = int(input("Masukkan jumlah transaksi: "))
    transactions = []
    transactionsI = []
    print("Asumsikan timestamp transaksi sesuai dengan urutan masukan nama transaksi mulai dari input pertama sebagai timestamp terkecil")
    for i in range(n):
        input1 = str(input("Masukkan nama transaksi " + str(i+1) + ": "))
        transaction = Transaction(input1,i+1)
        transactions.append(transaction)
        transactionsI.append(input1)
    
    
    print("Masukkan isi jadwal dalam satu baris (dipisahkan menggunakan semicolon), contoh: R1(X);R2(Y);W1(X)")
    tempSchedule = str(input()).split(";")
    schedule = []
    for i in tempSchedule:
        i = i.replace("(","").replace(")","")
        action = i[0]
        if(action != "C" and action != "c"):
            item = i[len(i) - 1]
            transaction = i[1:len(i) - 1]
            schedule.append(Action(action,transaction,item))
        else:
            transaction = i[1:len(i)]
            schedule.append(Action(action,transaction,""))
    
    while(len(schedule) != 0):
        currA = schedule.pop(0)
        print(f"{currA.action}{currA.transaction}({currA.item})")
        location1 = transactionsI.index(currA.transaction)
        currT = transactions[location1]
        if(currA.action != "C" and currA.action != "c"):
            location2 = itemsI.index(currA.item)
            currI = items[location2]
        if(currA.action == "W" or currA.action == "w"):
            result = currI.checkVersionW(currT.ts,currT.name)
            isAbort = not result
        elif(currA.action == "R" or currA.action == "r"):
            result = currI.checkVersionR(currT.ts,currT.name)
            isAbort = not result
        elif(currA.action == "C" or currA.action == "c"):
            currT.fin = True
            print(f"Commit Transaksi {currT.name}")
        if(isAbort):
            print("Terdapat transaksi yang di-abort")
            break
    if(not(isAbort)):
        print("Schedule berhasil dijalankan sehingga serializable")
            
        
        



