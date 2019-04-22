/*

  Created by Beat707 (c) 2011 - http://www.Beat707.com - Version 1.0.0 - June 23 2011
  
  The code will search for any Beat707 Device that is in USB mode and let you Backup/Restore all machine data stored on both EEPROM and Nand Flash. (Setup, Patterns and Songs)
  Serial Port Speed: 115200 bps
  
*/

import processing.serial.*;
import java.io.File;
 
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
void setup() 
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
void dispose()
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
void draw() 
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
void mousePressed()
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
void findSerialPort(boolean pauseOnSerialPortOpen)
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
