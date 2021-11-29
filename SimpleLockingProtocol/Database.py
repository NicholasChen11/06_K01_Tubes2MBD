from Transaction import Transaction


class Database():
  def __init__(self, listTransaction):
    self.database = {}
    self.numOfRecord = 0
    
    for transaction in listTransaction:
      if not transaction.isCommit():
        if not self.isRecordExist(transaction.txnRecord):
          self.numOfRecord += 1
          self.database[transaction.txnRecord] = 0;
      
  def isRecordExist(self, record: str):
    if not self.database:  #database empty
      return False
    else:
      for rec in self.database:
        if self.database.get(rec) == record:
          return True
      return False
  
  def printDatabase(self):
    print("Database Content:")
    for record in self.database:
      print(f"{record}: {self.database.get(record)}")
    
  def process(self, txn: Transaction):
    if txn.txnType == 'W':
      self.database[txn.txnRecord] = txn.txnNumber
    print(f"{txn.txnType}{txn.txnNumber}({txn.txnRecord})")
  
  def rollback(self, txn: Transaction, schedule: list):
    exist = False
    for i, trans in reversed(list(enumerate(schedule))):
      if trans.txnRecord == txn.txnRecord and trans.txnType == 'W':
        self.database[trans.txnRecord] = trans.txnNumber
        exist = True
    if not exist:
      self.database[txn.txnRecord] = 0
      