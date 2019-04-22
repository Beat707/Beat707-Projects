/*

  Created by Beat707 (c) 2011 - http://www.Beat707.com

*/

// ======================================================================================= //
class BasicThread2 implements Runnable 
{
  boolean bStop = false;
  boolean bDone = false;
  boolean bNotFound = false;
  boolean bRetry = true;
  boolean bReady = false;
  boolean doBackup = false;
  boolean doRestore = false;
  
  // -------------------------------------------- //
  public void run() 
  {
    // Search Devices //    
    while (!bStop) 
    {
      if (BeatSerial != -1) break;
      if (BeatSerial == -1 && bRetry)
      {
        bRetry = false;
        bNotFound = false;
        findSerialPort(false);
        if (BeatSerial == -1) findSerialPort(true);        
        if (BeatSerial == -1)
        {
          bNotFound = true;
          showMessage = "Click to retry";
        }
      }
      
      try { Thread.sleep(250); } catch (InterruptedException e) { }
    }
    
    // Main Thread Run //
    showMessage = "";
    bReady = true;
    while (!bStop)
    {
      if (doBackup) { doBackup = false; startBackup(); }
      if (doRestore) { doRestore = false; startRestore(); }
      try { Thread.sleep(250); } catch (InterruptedException e) { }
    }
    
    bDone = true;
  }
  
  // -------------------------------------------- //
  public void doStop()
  {
    bStop = true;
    
    while (!bDone) { try { Thread.sleep(150); } catch (InterruptedException e) { } }
  }
  
  // -------------------------------------------- //
  public void backup() { if (!isProcessing) doBackup = true; }  
  public void restore() { if (!isProcessing) doRestore = true; }  
}

// ======================================================================================= //
BasicThread2 mainThread = new BasicThread2();
Thread myThread = new Thread(mainThread);
