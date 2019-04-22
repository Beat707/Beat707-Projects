/*

  Created by Beat707 (c) 2011 - http://www.Beat707.com - Version 1.0.2 - June 24 2011 
  
  USB to MIDI converter - The code will search for any Beat707 Device that is in USB mode and start listening to any midi information it may transmit
  Serial Port Speed: 115200 bps
  
*/

import javax.sound.midi.*;
import processing.serial.*;
 
PFont myFont;
Serial serialPort = null;
String midiPortName = "";
String errorMsg;
MidiDevice MIDIout = null;
ShortMessage MIDIshortMessage = new ShortMessage();
MidiDevice.Info[] outputMIDIports;
int BeatID = 0;
int BeatSerial = -1;
int MIDIdevice = 0;
int MIDIportsN = 0;
String[] SerialList = Serial.list();
int state, data0, data1, data2, incomingByte = 0;
Receiver receiver;
int framecounter = 0;
boolean showFrameCounter = false;
PImage backImage;
PImage deviceNFound;
PImage selectMIDI;
PImage iconImage;
int startupCounter = 0;
boolean enableSerial = false;
 
// ======================================================================================= //
void setup() 
{ 
  size(600,300); 
  frame.setResizable(false);
  frame.setTitle("Beat707.com - USB to MIDI - Initializing ...");
  iconImage = loadImage("Icon.png");
  frame.setIconImage(iconImage.getImage());
  myFont = loadFont("Verdana-18.vlw"); 
  backImage = loadImage("Background.png");
  deviceNFound = loadImage("DeviceNotFound.png");
  selectMIDI = loadImage("SelectMIDIinterface.png");
  textFont(myFont);
  textAlign(CENTER);
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
  if (MIDIout != null)
  {
    MIDIout.close();
  }
}

// ======================================================================================= //
void draw() 
{ 
  image(backImage, 0, 0);
  
  if (startupCounter == 0) 
  {
    startupCounter++;
    text("Initializing ...", 94, 174, 412, 18);
  }
  else if (startupCounter == 1)
  {
    startupCounter++;
    listMidiDevices();
    findSerialPort(false);
    if (BeatSerial == -1) findSerialPort(true);
  }
  else
  {
    if (startupCounter == 2)
    {
      startupCounter++;
      noLoop(); // Stops Draw()
    }
    
    frame.setTitle("Beat707.com - USB to MIDI");
    
    if (BeatSerial != -1)
    {
      
      image(selectMIDI,37,142);
      text(midiPortName, 94, 174, 412, 18);
    }
    else
    {
      image(deviceNFound,148,138);
      if (errorMsg != null) text(errorMsg, 0, 260, 600, 18);
        else text("Click to Retry", 0, 260, 600, 18);
    }
    
    if (showFrameCounter)
    {
      framecounter++;
      text(framecounter, 10,150);
    }
  }
}

// ======================================================================================= //
void mousePressed()
{
  if (BeatSerial == -1)
  {
    startupCounter = 0;
    SerialList = Serial.list();
    loop();
  }
  else
  {
    boolean newMIDI = false;
    
    if (mouseX >= 49 && mouseX <= 80 && mouseY >= 165 && mouseY <= 198) // Left
    {
      MIDIdevice--;
      if (MIDIdevice < 0) MIDIdevice = MIDIportsN-1;
      newMIDI = true;
    }
    if (mouseX >= 518 && mouseX <= 550 && mouseY >= 165 && mouseY <= 198) // Right
    {
       MIDIdevice++;
      if (MIDIdevice >= MIDIportsN)  MIDIdevice = 0;
      newMIDI = true;
    }
    
    if (newMIDI)
    {
      enableSerial = false;
      try { Thread.sleep(250); } catch (InterruptedException e) { }
      MIDIout.close();
      openMIDI();
      enableSerial = true;
      serialPort.write(240); serialPort.write(0x01); serialPort.write(0x08); serialPort.write(0x04);
      serialPort.write(0x02); serialPort.write(0x09); serialPort.write(101); serialPort.write(BeatID); 
      serialPort.write(MIDIdevice);
      serialPort.write(247);
    }
  }
  
  redraw();
}

// ======================================================================================= //
void findSerialPort(boolean pauseOnSerialPortOpen)
{
  for (int x=(SerialList.length-1); x>=0; x--)
  {
    serialPort = null;
    try { serialPort = new Serial(this, SerialList[x], 115200);  } catch(RuntimeException e) { errorMsg = "Port already in use: " + SerialList[x] + " - Click to Retry"; }
    if (serialPort != null)
    {
      if (pauseOnSerialPortOpen) { try { Thread.sleep(3000); } catch (InterruptedException e) { } } // This is required as some Arduino Boards in certain situations will auto-reset as soon as the computer opens the Serial port
      serialPort.clear();
      serialPort.write(0xFC); // First Stop any Clock //
      try { Thread.sleep(50); } catch (InterruptedException e) { }
      while (serialPort.available() > 0) { serialPort.read(); }
      serialPort.clear();
      // Sends MIDI SysEx to the Serial port, hoping a Beat707 unit will reply //
      serialPort.write(240); serialPort.write(0x01); serialPort.write(0x08); serialPort.write(0x04); serialPort.write(0x02); serialPort.write(0x09); serialPort.write(100); serialPort.write(BeatID); serialPort.write(247);
      try { Thread.sleep(50); } catch (InterruptedException e) { }
      if (serialPort.available() > 0 && serialPort.read() == 240 && serialPort.read() == 'B' && serialPort.read() == '7' && serialPort.read() == '0' && serialPort.read() == '7')
      {
        MIDIdevice = serialPort.read();
        if (MIDIdevice < 0 || MIDIdevice >= MIDIportsN)  MIDIdevice = 0;
        if (serialPort.read() == 247)
        {
          BeatSerial = x;
          serialPort.clear();
          openMIDI();
          serialPort.buffer(1);
          enableSerial = true;
          break;
        }
        else
        {
          MIDIdevice = 0;
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

// ======================================================================================= //
void listMidiDevices()
{
  MidiDevice.Info[] MIDIports = MidiSystem.getMidiDeviceInfo(); // This lists ALL Midi Devices found on the system (Input and Output)
  Vector<MidiDevice.Info> MIDIportsList = new Vector<MidiDevice.Info>();
  for(int i = 0;i < MIDIports.length;i++) 
  {
    try { if (MidiSystem.getMidiDevice(MIDIports[i]).getMaxReceivers() != 0) { MIDIportsList.add(MIDIports[i]); MIDIportsN++; } } // getMaxReceivers: obtains the maximum number receiver connections (that would be MIDI Outputs)
      catch(MidiUnavailableException e) { }
  }
  outputMIDIports = new MidiDevice.Info[MIDIportsList.size()];
  MIDIportsList.toArray(outputMIDIports);
}

// ======================================================================================= //
void openMIDI()
{
  try
  {
      midiPortName = outputMIDIports[MIDIdevice].toString();
      MIDIout = MidiSystem.getMidiDevice(outputMIDIports[MIDIdevice]);
      MIDIout.open();
      receiver = MIDIout.getReceiver();
  } catch(MidiUnavailableException e) { midiPortName = ""; }
}

// ======================================================================================= //
void sendMIDIdata()
{
  try { if (data1 == -1) MIDIshortMessage.setMessage(data0); else MIDIshortMessage.setMessage(data0,data1,data2); } catch(InvalidMidiDataException e) {}
  receiver.send(MIDIshortMessage,0);
}

// ======================================================================================= //
void serialEvent(Serial p)
{  
  if (enableSerial && BeatSerial != -1)
  {
    while (serialPort.available() > 0)
    {
      incomingByte = p.read();
      
      switch (state)
      {
        case 0:
          if (incomingByte >= 128) 
          { 
            data0 = incomingByte; 
            state = 1; 
          } 
          else 
          { 
            data0 = incomingByte; 
            data1 = -1;
            data2 = state = 0; 
            sendMIDIdata(); 
          }
          break;
          
        case 1: 
          if (incomingByte < 128)  
          { 
            data1 = incomingByte; 
            state = 2;
            
            if (data0 == 192 || data0 == 208)
            {
              state = data2 = 0;
              sendMIDIdata();
            }
          } 
          break;
          
        case 2: 
          if (incomingByte < 128)  
          { 
            data2 = incomingByte; 
            state = 0; 
            sendMIDIdata(); 
          } 
          break;
      }      
    }
  }
}
