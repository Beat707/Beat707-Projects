import processing.core.*; 
import processing.xml.*; 

import processing.serial.*; 
import java.io.File; 
import java.awt.FileDialog; 

import java.applet.*; 
import java.awt.Dimension; 
import java.awt.Frame; 
import java.awt.event.MouseEvent; 
import java.awt.event.KeyEvent; 
import java.awt.event.FocusEvent; 
import java.awt.Image; 
import java.io.*; 
import java.net.*; 
import java.text.*; 
import java.util.*; 
import java.util.zip.*; 
import java.util.regex.*; 

public class Beat707_Manager extends PApplet {

/*

  Created by Beat707 (c) 2011 - http://www.Beat707.com - Version 1.0.0 - June 23 2011
  
  The code will search for any Beat707 Device that is in USB mode and let you Backup/Restore all machine data stored on both EEPROM and Nand Flash. (Setup, Patterns and Songs)
  Serial Port Speed: 115200 bps
  
*/



 
PFont myFont;
Serial serialPort = null;
String showMessage;
int BeatID = 0;
int BeatSerial = -1;
String[] SerialList = Serial.list();
int state, data0, data1, data2, incomingByte = 0;
int headerSize = 32;
PImage backImage;
PImage deviceNFoundImage;
PImage iconImage;
PImage menuImage;
PImage includePatSngsImage;
PImage includePatSngsImageOff;
boolean includePatSngs = true;
boolean isProcessing = false;
boolean clickToContinue = false;
byte numerOfSongsOnFlashMemory = 0;
 
// ======================================================================================= //
public void setup() 
{ 
  size(600,300); 
  frame.setResizable(false);
  frame.setTitle("Beat707.com Manager - Initializing");
  iconImage = loadImage("Icon.png");
  frame.setIconImage(iconImage.getImage());
  myFont = loadFont("Verdana-18.vlw"); 
  backImage = loadImage("Background.png");
  deviceNFoundImage = loadImage("DeviceNotFound.png");
  menuImage = loadImage("Menu.png");
  includePatSngsImage = loadImage("IncludePatSngs.png");
  includePatSngsImageOff = loadImage("IncludePatSngsOff.png");
  textFont(myFont);
  textAlign(CENTER);
  showMessage = "Initializing";    
  myThread.start();
} 

// ======================================================================================= //
public void dispose()
{
  if (serialPort != null)
  {
    serialPort.stop();
    serialPort.clear();
    try { Thread.sleep(150); } catch (InterruptedException e) { }
    serialPort.dispose();
    try { Thread.sleep(150); } catch (InterruptedException e) { }
  }
  
  mainThread.doStop();
}

// ======================================================================================= //
public void draw() 
{ 
  image(backImage, 0, 0);
  if (mainThread.bNotFound) image(deviceNFoundImage,148,138);
  
  if (mainThread.bReady && !isProcessing && !clickToContinue)
  {
    image(menuImage,33,112);
    if (includePatSngs) image(includePatSngsImage,170,252); else image(includePatSngsImageOff,170,252);
  }
  
  if (showMessage != null) text(showMessage, 0, 260, 600, 18);
}

// ======================================================================================= //
public void mousePressed()
{
  if (isProcessing) return;
  if (clickToContinue) { clickToContinue = false; showMessage = ""; return; }
  
  if (mainThread.bNotFound)
  {
    showMessage = "Initializing";
    SerialList = Serial.list(); 
    mainThread.bRetry = true;
    return;
  }
  
  if (BeatSerial == -1) return;

  if (mouseX >= 173 && mouseX <= 420 && mouseY >= 260 && mouseY <= 288)
  {
    includePatSngs = !includePatSngs;
  }
  else if (mouseX >= 41 && mouseX <= 290 && mouseY >= 121 && mouseY <= 237) // Backup
  {
    mainThread.backup();
  }
  else if (mouseX >= 308 && mouseX <= 552 && mouseY >= 121 && mouseY <= 237) // Restore
  {
    mainThread.restore();
  }
}

// ======================================================================================= //
public void findSerialPort(boolean pauseOnSerialPortOpen)
{
  for (int x=(SerialList.length-1); x>= 0; x--)
  {
    if (pauseOnSerialPortOpen) showMessage = "Opening port: " + SerialList[x];
    serialPort = null;
    try { serialPort = new Serial(this, SerialList[x], 115200);  } catch(RuntimeException e) { showMessage = "Port already in use: " + SerialList[x]; } // 115200
    if (serialPort != null)
    {
      if (pauseOnSerialPortOpen) { try { Thread.sleep(3000); } catch (InterruptedException e) { } }
      serialPort.clear();
      serialPort.write(0xFC); // First Stop any Clock //
      try { Thread.sleep(50); } catch (InterruptedException e) { }
      while (serialPort.available() > 0) { serialPort.read(); }
      serialPort.clear();
      // Sends MIDI SysEx to the Serial port, hoping a Beat707 unit will reply //
      serialPort.write(240); serialPort.write(0x01); serialPort.write(0x08); serialPort.write(0x04); serialPort.write(0x02); serialPort.write(0x09); serialPort.write(102); serialPort.write(BeatID); serialPort.write(247);
      try { Thread.sleep(50); } catch (InterruptedException e) { }
      if (serialPort.available() > 0 && serialPort.read() == 240 && serialPort.read() == 'B' && serialPort.read() == '7' && serialPort.read() == '0' && serialPort.read() == '7')
      {
        numerOfSongsOnFlashMemory = (byte)(serialPort.read() & 0xff);
        if (serialPort.read() == 247)
        {
          showMessage = "Found on port: " + SerialList[x];
          BeatSerial = x;
          serialPort.clear();
          break;
        }
        else
        {
          serialPort.stop();
          serialPort.dispose();          
        }
      }
      else
      {
        serialPort.stop();
        serialPort.dispose();
      }
    }
  }
}
/*

  Created by Beat707 (c) 2011 - http://www.Beat707.com

*/

public void startBackup()
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
/*

  Created by Beat707 (c) 2011 - http://www.Beat707.com

*/



public String selectOutputPro(String prompt, String filestring) {
  return selectFileImplPro(prompt, filestring, FileDialog.SAVE);
}

public String selectInputPro(String prompt, String filestring) {
  return selectFileImplPro(prompt, filestring, FileDialog.LOAD);
}


protected String selectFileImplPro(final String prompt, final String filestring, final int mode) {
  this.checkParentFrame();

  try {
    javax.swing.SwingUtilities.invokeAndWait(new Runnable() {
	public void run() {
	  FileDialog fileDialog =
	    new FileDialog(parentFrame, prompt, mode);
	  fileDialog.setFile(filestring);
	  fileDialog.setVisible(true);
	  String directory = fileDialog.getDirectory();
	  String filename = fileDialog.getFile();
	  selectedFile =
	    (filename == null) ? null : new File(directory, filename);
	}
    }
    );
    return (selectedFile == null) ? null : selectedFile.getAbsolutePath();

  } 
  catch (Exception e) {
    e.printStackTrace();
    return null;
  }
}
/*

  Created by Beat707 (c) 2011 - http://www.Beat707.com

*/

public void startRestore()
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
  static public void main(String args[]) {
    PApplet.main(new String[] { "--bgcolor=#ECE9D8", "Beat707_Manager" });
  }
}
