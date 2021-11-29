class Transaction():
  def __init__(self, strTxn: str):
    self.txnType = strTxn[0]
    self.txnNumber = ""
    self.txnRecord = ""
    
    if self.txnType == 'C':
      self.txnNumber = int(strTxn[1:])
    else :
      strTxn = strTxn[1:]
      while True:
        if (strTxn[0] != '('):
          self.txnNumber += strTxn[0]
          strTxn = strTxn[1:]
        else:
          strTxn = strTxn[1:]
          break
      self.txnNumber = int(self.txnNumber)
      self.txnRecord = strTxn[:-1]
    # print(f"input check: {self.txnType}{self.txnNumber}({self.txnFile})")
  
  def isCommit(self):
    if self.txnType == 'C':
      return True
    else:
      return False