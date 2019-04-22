/*

  Created by Beat707 (c) 2011 - http://www.Beat707.com

*/

void startBackup()
{
  isProcessing = true;
  
  String EXT = "bnk1";
  if (includePatSngs) EXT = "bnk2";
  String savePath = selectOutputPro("Enter the Filename you want data to be stored on...", "Beat707." + EXT);
  if (savePath == null) { isProcessing = false; return; }
  
  File f = new File(savePath);
  if (f.exists()) f.delete();

  byte[] largeBuffer;
  if (includePatSngs) largeBuffer = new byte [23296+(numerOfSongsOnFlashMemory*(23296+14))+headerSize]; else largeBuffer = new byte [23296+headerSize];
  
  largeBuffer[0] = 'B'; largeBuffer[1] = 'e'; largeBuffer[2] = 'a'; largeBuffer[3] = 't'; largeBuffer[4] = '7'; largeBuffer[5] = '0';
  largeBuffer[6] = '7'; largeBuffer[7] = '.'; largeBuffer[8] = 'c'; largeBuffer[9] = 'o'; largeBuffer[10] = 'm'; largeBuffer[11] = ' ';
  largeBuffer[12] = 'D'; largeBuffer[13] = 'M'; largeBuffer[14] = ' '; largeBuffer[15] = 'V'; largeBuffer[16] = '0'; largeBuffer[17] = '2'; largeBuffer[18] = 'F';
  if (includePatSngs) largeBuffer[19] = 'Y'; else largeBuffer[19] = 'N';
  
  serialPort.clear();
  serialPort.write(0xFC); // First Stop any Clock //
  try { Thread.sleep(50); } catch (InterruptedException e) { }
  while (serialPort.available() > 0) { serialPort.read(); }
  serialPort.clear();
  // Request Complete Serial Dump //
  serialPort.write(240); serialPort.write(0x01); serialPort.write(0x08); serialPort.write(0x04); serialPort.write(0x02); serialPort.write(0x09); serialPort.write(103); serialPort.write(BeatID);
  if (includePatSngs) serialPort.write(0x01); else serialPort.write(0x00);
  while (serialPort.available() == 0) { try { Thread.sleep(1); } catch (InterruptedException e) { } }
  float percentDone = 0;
  float percentTotal = 23296;
  if (includePatSngs) percentTotal += (numerOfSongsOnFlashMemory*23296);
  boolean hasError = false;
  if (serialPort.available() > 0 && serialPort.read() == 240)
  {
    // First the complete EEPROM Chip #1 //
    int counter = 0;
    int checkData = 0;
    byte buffer65[] = new byte[65];
    while (true)
    {
      if (serialPort.available() == 65)
      {
        serialPort.readBytes(buffer65);
        checkData = 0;
        for (int x=0; x<64; x++) 
        {
          largeBuffer[headerSize+counter+x] = buffer65[x];
          checkData += (buffer65[x] & 0xff);
        }
        if ((buffer65[64] & 0xff) != (checkData/64))
        {
          serialPort.write(0x01);
          hasError = true;
          break;
        } else serialPort.write(0x00);
        counter += 64;   
        if (counter == 23296) break;        
        showMessage = "Receiving ... " + (int)((100.0f/percentTotal)*percentDone) + " %";
        percentDone += 64;
      }
    }
                    
    if (includePatSngs)
    {
      // Finally the Nand Flash Chip //
      while (true)
      {
        if (hasError) break;
        while (serialPort.available() == 0) { try { Thread.sleep(1); } catch (InterruptedException e) { } }
        if ((serialPort.read() & 0xff) == 0x25) break;
        serialPort.write(0x00);
        while (serialPort.available() != 14) { try { Thread.sleep(1); } catch (InterruptedException e) { } }
        for (int x=0; x<14; x++) { largeBuffer[headerSize+counter+x] = (byte)serialPort.read(); }
        counter += 14;
        serialPort.write(0x00);
        for (int x=0; x<23296; x+=64)
        {
          while (serialPort.available() != 65) { try { Thread.sleep(1); } catch (InterruptedException e) { } }
          serialPort.readBytes(buffer65);
          checkData = 0;
          for (int xq=0; xq<64; xq++)
          { 
            largeBuffer[headerSize+counter+xq] = buffer65[xq]; 
            checkData += (buffer65[xq] & 0xff);
          }
          if ((buffer65[64] & 0xff) != (checkData/64))
          {
            serialPort.write(0x01);
            hasError = true;
            break;
          } else serialPort.write(0x00);          
          showMessage = "Receiving ... " + (int)((100.0f/percentTotal)*percentDone) + " %";
          counter += 64;
          percentDone += 64;
        }
      }
    }
    
    while (serialPort.available() == 0) { try { Thread.sleep(1); } catch (InterruptedException e) { } }               
    if (serialPort.read() != 247) hasError = true;

    if (!hasError)
    {
      saveBytes(savePath, largeBuffer);
      showMessage = "Done, click to continue";
      clickToContinue = true;
    }
  }
  else hasError = true;
  
  if (hasError)
  {
    showMessage = "Error, click to continue";
    clickToContinue = true;
  }
  
  isProcessing = false;
} 
