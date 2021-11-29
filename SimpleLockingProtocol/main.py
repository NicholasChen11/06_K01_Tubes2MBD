from typing import List
from Transaction import Transaction
from Database import Database
from LockManager import LockManager

def main():
  def recieveTransactionInput():
    """
    Function for recieving transaction input from user 
    
    return: List of Transaction (list)
    """
    
    N = int(input("Enter the number of transaction you want to input: "))
    
    print("")
    print(f"Input {N} transaction one by one with following format.")
    print("R - Read")
    print("W - Write")
    print("e.x: R1(X), W3(Z), C1, C3")
    print("")
    
    listTransaction = []
    for i in range(N):
      inputTransaction = Transaction(str(input(f"Enter transaction {i+1}: ")))
      listTransaction.append(inputTransaction)
    
    return listTransaction
  
  def processSchedule(database: Database, listTransaction):
    lockManager = LockManager(database)
    schedule = []
    
    while listTransaction:  #while list still not empty
      txn = listTransaction.pop(0)
      if txn.isCommit():
        print(f"{txn.txnType}{txn.txnNumber}")
        schedule.append(txn)
      else:
        if lockManager.hasLock(txn):
          schedule.append(txn)
          database.process(txn)
        else:
          acquiredLock = lockManager.reqLock(txn)
          if acquiredLock:
            schedule.append(txn)
            database.process(txn)
          else:
            if lockManager.canWait(txn):  # check wait-die
              continue
            else:  #abort
              listTransaction = removeTxnFromList(listTransaction, txn)
              database, schedule = abort(schedule, txn, database)
        
        unlock = True
        for nextTxn in listTransaction:
          if (not nextTxn.isCommit()) and nextTxn.txnNumber == txn.txnNumber and nextTxn.txnRecord == txn.txnRecord:
            unlock = False
        if unlock:
          listTxnQueue = lockManager.unlockLock(txn)
          listTransaction = listTxnQueue + listTransaction
    return database
  
  def removeTxnFromList(listTransaction, txn: Transaction):
    listReturn = []
    for trans in listTransaction:
      if trans.txnNumber != txn.txnNumber:
        listReturn.append(trans)
    return listReturn
  
  def abort(schedule: List, txn: Transaction, database: Database):
    print(f"A{txn.txnNumber}")
    listRollback = []  #berisi txn yg kena dampak cascading rollback
    i = len(schedule) - 1
    while i > -1:
      trans = schedule[i]
      if trans.txnNumber == txn.txnNumber:
        if trans.isCommit():
          print(f"Recoverability error! cannot rollback transaction T{txn.txnNumber} because already commited.")
          exit()
        if trans.txnType == 'W':
          # cek apakah ada transaksi lain yg terdampak cascading rollback
          # dicek dengan mencari kasus dirty read ke record yg ingin di rollback
          j = i + 1
          while j < len(schedule):
            if schedule[j].txnRecord == trans.txnRecord:
              if schedule[j].txnType == 'W':
                break
              else:
                listRollback.append(schedule[j])
            j += 1
        schedule.pop(i)
        database.rollback(txn, schedule)
      i -= 1
    
    while listRollback:
      txnRollback = listRollback[0]
      database, schedule = abort(schedule, txnRollback, database)
    
    return database, schedule
        
  
  # MAIN PROGRAM
  print("")
  print("==================================================================")
  print("This is a Simple Locking Concurrency Protocol Simulation Program")
  print("(exclusive locks only) made to complete task for IF3140 Database")
  print("Management Class, Institut Teknologi Bandung (ITB).")
  print("")
  print("This simulation use 'Wait-die' protocol for deadlock prevention.")
  print("Because of the nature of simple Locking Protocol doesn't handle")
  print("rollback recovery cases, when such cases occur, error message will")
  print("appear and force stop the simulation.")
  print("==================================================================")
  print("")
  listTransaction = recieveTransactionInput()
  database = Database(listTransaction)
  print("")
  print("Here are the process:")
  database = processSchedule(database, listTransaction)
  print("")
  database.printDatabase()
  

if __name__ == '__main__':
    main()