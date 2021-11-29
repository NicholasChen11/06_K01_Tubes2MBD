from Database import Database
from Transaction import Transaction


class LockManager():
  # Implementing wait-die strategy
  def __init__(self, database: Database):
    self.lockKeeper = {}
    self.lockReqQueue = []
    
    for record in database.database:
      self.lockKeeper[record] = None
      
  def getLockKeeper(self, record: str):
    return self.lockKeeper.get(record)
  
  def hasLock(self, txn: Transaction):
    if (self.lockKeeper.get(txn.txnRecord) == txn.txnNumber):
      return True
    else:
      return False

  def reqLock(self, txn: Transaction):
    if self.lockKeeper.get(txn.txnRecord) == None or self.lockKeeper.get(txn.txnRecord) == txn.txnNumber:
      self.lockKeeper[txn.txnRecord] = txn.txnNumber
      print(f"XL{txn.txnNumber}({txn.txnRecord})")
      return True
    else:
      if self.lockKeeper.get(txn.txnRecord) > txn.txnNumber:
        self.lockReqQueue.append(txn)
      return False

  def unlockLock(self, txn: Transaction):
    #return semua txn yg recordnya sama dari self.lockReqQueue
    self.lockKeeper[txn.txnRecord] = None
    print(f"UL{txn.txnNumber}({txn.txnRecord})")
    copyLockReqQueue = self.lockReqQueue
    listReturn = []
    while copyLockReqQueue:
      trans = copyLockReqQueue.pop(0)
      if trans.txnRecord == txn.txnRecord:
        listReturn.append(trans)
    return listReturn
  
  def canWait(self, txn: Transaction):
    if self.lockKeeper.get(txn.txnRecord) > txn.txnNumber:
      print(f"{txn.txnType}{txn.txnNumber}({txn.txnRecord}) is waiting for lock {txn.txnRecord}.")
      return True
    else:
      return False