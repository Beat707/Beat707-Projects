/*
 * 
 * Created by William Kalfelz @ Beat707 (c) 2017 - http://www.Beat707.com
 * 
 * 
 */

#define DEBUG_OUT 0
#include "MIDIUSB.h"
uint8_t midiPacket[4] = { 0, 0, 0, 0 };

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void setup() 
{
  Serial1.begin(31250);
  Serial1.flush();
  //
  #if DEBUG_OUT
    Serial.begin(9600);
    Serial.println("Startup");
  #endif
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void loop() 
{
  if (Serial1.available() > 0)
  {
    uint8_t xInput = SerialNextByte(false);
    memset(midiPacket, 0, sizeof(midiPacket));
    if (xInput == 0xF0) // System Exclusive Data - SysEx //
    {
      byte xBuffer = 2;
      midiPacket[0] = 0x4;
      midiPacket[1] = 0xF0;
      //
      while (1)
      {
        xInput = SerialNextByte(true);
        if (xInput == 0xF7) 
        {
          midiPacket[xBuffer] = 0xF7; 
          if (xBuffer == 1) { midiPacket[0] = 0x5; midiPacket[xBuffer + 1] = 0; midiPacket[xBuffer + 2] = 0; }
          if (xBuffer == 2) { midiPacket[0] = 0x6; midiPacket[xBuffer + 1] = 0; }
          if (xBuffer == 3) { midiPacket[0] = 0x7; }
          sendData();
          break;
        }
        else
        {
          midiPacket[xBuffer] = xInput;
        }
        //
        xBuffer++;
        if (xBuffer >= 4)
        {
          xBuffer = 1;
          sendData();
        }       
      }
    }
    else if (xInput == 0xF8     // MIDI Clock (96PPQ)
            || xInput == 0xFA   // MIDI Clock Start
            || xInput == 0xFB   // MIDI Clock Continue
            || xInput == 0xFC   // MIDI Clock Stop
            || xInput == 0xFE   // Active Sensing
            || xInput == 0xF6   // Tune Request
            || xInput == 0xFF)  // System Reset
    {
      midiPacket[0] = 0xF;
      midiPacket[1] = xInput;
      sendData();
    }
    else // Other Types //
    {
      byte nBytes = 2;
      midiPacket[0] = 0x09;
      switch(xInput & 0xF0)
      {
        case 0x80: midiPacket[0] = 0x08; break; // Note Off
        case 0x90: midiPacket[0] = 0x09; break; // Note On
        case 0xA0: midiPacket[0] = 0x0A; break;
        case 0xB0: midiPacket[0] = 0x0B; break;
        case 0xC0: midiPacket[0] = 0x0C; nBytes = 1; break;
        case 0xD0: midiPacket[0] = 0x0D; nBytes = 1; break;
        case 0xE0: midiPacket[0] = 0x0E; nBytes = 1; break;
        case 0xF1: midiPacket[0] = 0x0F; nBytes = 1; break; // MIDI Timing Code
        case 0xF3: midiPacket[0] = 0x0F; nBytes = 1; break; // Song Select
        case 0xF2: midiPacket[0] = 0x02; break; // Song Position Pointer
        
      }
      //
      midiPacket[1] = xInput;
      midiPacket[2] = SerialNextByte(true);
      if (nBytes == 2) midiPacket[3] = SerialNextByte(true);
      //
      sendData();
    }
  }
  //
  midiEventPacket_t rx = MidiUSB.read();
  if (true)//MidiUSB.available() > 0)
  {
    //midiEventPacket_t rx = MidiUSB.read();
    if (rx.header != 0)
    {
      #if DEBUG_OUT
        Serial.print(rx.header, DEC);
        Serial.print(" - ");
        Serial.print(rx.byte1, DEC);
        Serial.print(" - ");
        Serial.print(rx.byte2, DEC);
        Serial.print(" - ");
        Serial.println(rx.byte3, DEC);
      #endif
      //
      if (rx.header == 0xF) // Single Byte
      {
        Serial1.write(rx.byte1);
      }
      else if (rx.header == 0x04) // SysEx Start or Continue
      {
        Serial1.write(rx.byte1);
        Serial1.write(rx.byte2);
        Serial1.write(rx.byte3);
      }
      else if (rx.header == 0x05) // SysEx Ends 1 Byte
      {
        Serial1.write(rx.byte1);
        Serial1.write(0xF7);
      }
      else if (rx.header == 0x06) // SysEx Ends 2 Bytes
      {
        Serial1.write(rx.byte1);
        Serial1.write(rx.byte2);
        Serial1.write(0xF7);
      }
      else if (rx.header == 0x07) // SysEx Ends 3 Bytes
      {
        Serial1.write(rx.byte1);
        Serial1.write(rx.byte2);
        Serial1.write(rx.byte2);
        Serial1.write(0xF7);
      }
    }
  }
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
inline void sendData()
{
  MidiUSB.write(midiPacket, 4);
  MidiUSB.flush(); 
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
inline byte SerialNextByte(bool checkTime)
{
  //unsigned int xtime1 = millis();
  while (Serial1.available() == 0) 
  { 
    ;
    //if (checkTime && (millis() - xtime1) > 2000) return 0;
  }
  return Serial1.read();
}
