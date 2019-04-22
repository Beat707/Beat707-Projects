/*

  Created by Beat707 (c) 2011 - http://www.Beat707.com - July 21 2011 - Version 1.0.0

  Important: this code is for the ATtiny85 (or ATtiny45) chip - it requires to set its configuration fuses before uploading this code, so it uses its internal Oscilator.
  You may program the ATtiny series with an ISP programmer or using the Arduino as an ISP programmer. (in the provided pictures we are using the USBtinyISP)
  For more information, visit the following link: http://hlt.media.mit.edu/wiki/pmwiki.php?n=Main.ArduinoATtiny4585 - (keep in mind that we did not use the library from the link above, we used another lib which is already included in the Library.h file)
  To add ATtiny support to the Arduino IDE, open the "_Arduino_Extras" folder and read the Readme.txt file.

*/

#include "Library.h"

// ======================================================================================= //
char displayNumber[4];
uint8_t dots, dataOut = 0;
uint8_t beats = 8;
uint8_t beatAB = 0;
byte byteI2CReceived[2] = {0,0};
uint8_t showDots = 1;
uint8_t initDisplay;
uint8_t mode = 0; // 0=MMB* / 1=MMMB / 2=MMMM / M=Measure - B=Beat(1,2,3,4) //
char x, q, w = 0;
uint8_t numbers[11][7] = { { 1,1,1,1,1,1,0 }, { 0,1,1,0,0,0,0 }, { 1,1,0,1,1,0,1 }, { 1,1,1,1,0,0,1 }, { 0,1,1,0,0,1,1 }, { 1,0,1,1,0,1,1 }, { 0,0,1,1,1,1,1 }, { 1,1,1,0,0,0,0 }, { 1,1,1,1,1,1,1 }, { 1,1,1,0,0,1,1 },  { 0,0,0,0,0,0,0 } };

// ======================================================================================= //
void reset()
{
  initDisplay = 0;
  beatAB = 0;
  dots = 0;
  displayNumber[0] = 1;
  if (mode != 2) displayNumber[1] = 1; else displayNumber[1] = 10;
  displayNumber[2] = 10;
  displayNumber[3] = 10;
}

// ======================================================================================= //
void setup() 
{
  reset();
  TinyWireS.begin(TWI_SLAVE_ADDR);
  pinMode(CLOCK_PIN,OUTPUT);  pinMode(DATA_PIN,OUTPUT);  pinMode(LATCH,OUTPUT);
  digitalWrite(CLOCK_PIN, LOW);  digitalWrite(DATA_PIN, LOW);  digitalWrite(LATCH, LOW);
  initDisplay = 1;
}

// ======================================================================================= //
#define rawtick()\
  for (x=7; x>=0; x--)\
  {\
    setClockLow();\
    if (bitRead(dataOut,x) == 0) setDataLow(); else setDataHigh();\
    setClockHigh();\
  }\
  digitalWrite(LATCH, HIGH);\
  digitalWrite(LATCH, LOW);

// ======================================================================================= //
#define tickNumber(number,digit)\
  for (q=0; q<7; q++) \
  {\
    if (numbers[number][q] == 1) dataOut = q; else dataOut = B1111;\
    bitSet(dataOut,7-digit);\
    rawtick();  \
  }

// ======================================================================================= //
void loop() 
{
  if (TinyWireS.available())
  {
    byteI2CReceived[0] = TinyWireS.receive();
    switch (byteI2CReceived[0])
    {
      case 0: // Beat //
        initDisplay = 0;
        dots = 0;
        displayNumber[0]++;
        beatAB++;
        if (beatAB >= beats) beatAB = 0;
        if ((mode != 2 && displayNumber[0] > 4) || (mode == 2 && displayNumber[0] > 9))
        {
          if (mode == 2) displayNumber[0] = 0; else displayNumber[0] = 1;
          if (displayNumber[1] == 10) displayNumber[1] = 0;
          displayNumber[1]++;
          if (displayNumber[1] > 9)
          {
            displayNumber[1] = 0;
            if (displayNumber[2] == 10) displayNumber[2] = 0;
            displayNumber[2]++;
            if (mode == 1)
            {
              if (displayNumber[2] > 9)
              {
                displayNumber[2] = 0;
                if (displayNumber[3] == 10) displayNumber[3] = 0;
                displayNumber[3]++;
                if (displayNumber[3] > 9) displayNumber[3] = 0;
              }
            }
            else
            {
              if (displayNumber[2] > 9) displayNumber[2] = 0;
            }
          }
        }
        break;
      
      case 1: // Reset //
        reset();
        break;
        
      case 2: // Special Commands //
        while (!TinyWireS.available()) { ; } // Wait for the command //
        byteI2CReceived[1] = TinyWireS.receive();
        switch(byteI2CReceived[1])
        {
          case 0: // Set Beats/Measure Position //
            while (!TinyWireS.available()) { ; } // Wait for the extra byte //
            byteI2CReceived[1] = TinyWireS.receive();
            reset();          
            break;
          
          case 1: beats = 8; reset(); break;
          case 2: beats = 4; reset(); break;
          case 3: showDots = 1; break;
          case 4: showDots = 0; break;
          case 5: mode = 0; reset(); break; // MMB*
          case 6: mode = 1; reset(); break; // MMMB
          case 7: mode = 2; reset(); break; // MMMM
        }
        break;
    }
  }
  
  // ----------------------------------------------- //
  
  #if SHOW_INIT_DISPLAY
    if (initDisplay == 1)
    {
      dataOut = B10000110; rawtick();
      dataOut = B01000110; rawtick();
      dataOut = B00100110; rawtick();
      dataOut = B00010110; rawtick();
      dataOut = B00000000;
      for (w=0; w<40; w++) { rawtick(); }
      return;
    }
  #endif
  
  if (showDots == 0 || dots > 40)
  {
    tickNumber(displayNumber[0], 1-mode);
    tickNumber(displayNumber[1], 2-mode);
    tickNumber(displayNumber[2], 3-mode);
    if (mode != 0)
    {
      tickNumber(displayNumber[3], 3);
    }
    else
    {
      if (beatAB < 4)
      {
        if (beatAB != 0) dataOut = B10000000; rawtick();
        if (beatAB != 1) dataOut = B10000001; rawtick();
        if (beatAB != 2) dataOut = B10000110; rawtick();
        if (beatAB != 3) dataOut = B10000101; rawtick();
      }
      else
      {
        if (beatAB != 4) dataOut = B10000110; rawtick();
        if (beatAB != 5) dataOut = B10000010; rawtick();
        if (beatAB != 6) dataOut = B10000011; rawtick();
        if (beatAB != 7) dataOut = B10000100; rawtick();
      }
    }
  }
  
  if (showDots == 1 && dots < 50)
  {
    dots++;
    dataOut = B10000111; rawtick();
    dataOut = B01000111; rawtick();
  }
}
