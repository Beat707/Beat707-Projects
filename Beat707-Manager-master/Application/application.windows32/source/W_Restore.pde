/*

  Created by Beat707 (c) 2011 - http://www.Beat707.com

*/

void startRestore()
{
  isProcessing = true;

  String EXT = "bnk1";
  if (includePatSngs) EXT = "bnk2";  
  String loadPath = selectInputPro("Enter the Filename you want data to be restored from ...", "Beat707." + EXT);
  if (loadPath == null) { isProcessing = false; return; }
  File f = new File(loadPath);
  if (!f.exists()) { isProcessing = false; return; }
  byte b[] = loadBytes(loadPath);
  
  serialPort.clear();
  serialPort.write(0xFC); // First Stop any Clock //
  try { Thread.sleep(50); } catch (InterruptedException e) { }
  while (serialPort.available() > 0) { serialPort.read(); }
  serialPort.clear();
  // Request Complete Serial Dump //
  serialPort.write(240); serialPort.write(0x01); serialPort.write(0x08); serialPort.write(0x04); serialPort.write(0x02); serialPort.write(0x09); serialPort.write(104); serialPort.write(BeatID);
  if (includePatSngs) serialPort.write(0x01); else serialPort.write(0x00);
  try { Thread.sleep(50); } catch (InterruptedException e) { }
  float percentDone = 0;
  float percentTotal = b.length-headerSize;
  boolean hasError = false;  
  int counter = 0;
  if (serialPort.available() > 0 && serialPort.read() == 240)
  {
    serialPort.clear();
    byte buffer64[] = new byte[64];
    while (true)
    {
      int checkData = 0;
      for (int x=0; x<64; x++)
      {
        buffer64[x] = (byte)b[headerSize+counter+x];
        checkData += (buffer64[x] & 0xff);
      }
      serialPort.write(buffer64);
      showMessage = "Sending ... " + (int)((100.0f/percentTotal)*percentDone) + " %";
      percentDone += 64;
      
      while (serialPort.available() == 0) { ; }
      if ((serialPort.read() & 0xff) != (checkData/64))
      {
        hasError = true;
        break;
      }

      counter += 64;
      if (counter == 23296) break;
    } 
    
    if (!hasError && includePatSngs)
    {
      if (counter < (b.length-headerSize))
      {
        serialPort.write(0x00);
        while (serialPort.available() == 0) { ; }
        serialPort.read();
               
        byte buffer14[] = new byte[14];
        while (true)
        {
          int checkData = 0;
          int xCounter = 0;
          for (int x=0; x<14; x++) { buffer14[x] = (byte)b[headerSize+counter+x]; }
          counter += 14;
          percentDone += 14;
          serialPort.write(buffer14);
          while (serialPort.available() == 0) { ; }
          serialPort.read();
                    
          while (true)
          {
            checkData = 0;
            for (int x=0; x<64; x++)
            {
              buffer64[x] = (byte)b[headerSize+counter+x];
              checkData += (buffer64[x] & 0xff);
            }
            serialPort.write(buffer64);
            showMessage = "Sending ... " + (int)((100.0f/percentTotal)*percentDone) + " %";
            percentDone += 64;
            counter += 64;
            xCounter += 64;
            
            while (serialPort.available() == 0) { ; }
            byte tempData = (byte)(serialPort.read() & 0xff);
            if (tempData != (checkData/64))
            {
              hasError = true;
              break;
            }
      
            if (xCounter == 23296) break;
          }
          
          if (hasError) break;
          
          if ((counter+headerSize) >= b.length)
          {
            serialPort.write(0xff);
            break;
          } else serialPort.write(0x00);
        }
      } else serialPort.write(0xff);  
    }

    if (!hasError)
    {
      while (serialPort.available() == 0) { try { Thread.sleep(1); } catch (InterruptedException e) { } }               
      if (serialPort.read() != 247) hasError = true;
    }

    if (!hasError)
    {
      showMessage = "Done, click to continue";
      clickToContinue = true;
    }
  } else hasError = true;
  
  if (hasError)
  {
    showMessage = "Error, click to continue";
    clickToContinue = true;
  }  
  
  isProcessing = false;
}
