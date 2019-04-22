/*

  Created by Guilherme (William) Kalfelz
  @ Beat707 (c) 2014 - http://www.Beat707.com
  
  Main Loop
  
*/

// ======================================================================================= //
void loop()
{   
  while (Serial.available() > 0)
  {
    byte bByte = Serial.read();
    byte nBytes = bByte >> 5;
    switch (bByte)
    {
      // ---------------------------- //
      case 0: // MIDI Clock, sent from the LCD IC //
      {
        clockTick();
      }
      break;
      
      // ---------------------------- //
      case 1: // Start the Sequencer, sent from the LCD IC //
      {
        seqClockStart(1);
      }
      break;
      
      // ---------------------------- //
      case 2: // Stop the Sequencer, sent from the LCD IC //
      {
        seqClockStop();
      }
      break;

      // ---------------------------- //
      case 3: showNavigationKeys = 1; break;
      case 4: showNavigationKeys = 0; break;    
      
      // ---------------------------- //
      case 5: // Steps Position
      case 6:
      case 7:
      case 8:
      {
        editStepsPos = bByte-5;
        autoStepPos = 0;
        mirrorStepsEdit = 0;
      }
      break;
      
      // ---------------------------- //
      case 9: // Track Selector
      case 10:
      case 11:
      case 12:
      case 13:
      {
        currentTrack = bByte-9;
      }
      break;
      
      // ---------------------------- //
      case 14: // Auto Steps Position
      case 15:
      {
        editStepsPos = stepsPos;
        autoStepPos = 1;
        if (bByte == 14) mirrorStepsEdit = 1; 
          else mirrorStepsEdit = 0;
      }
      break;
      
      // ---------------------------- //
      case 16:
      {
        patternFade = 1;
        patternFadeCounter = 0;
      }
      break;
     
      // ---------------------------- //
      case 17:
      {
        patternFade = -1;
        patternFadeCounter = 128;
      }
      break;
      
      // ---------------------------- //
      case 18:
      {
        ledsReady = 1;
        eeprom_read_block((void*)&dmSteps, (void*)0, sizeof(dmSteps));
      }
      break;
      
      // ---------------------------- //
      case 19:
      {
        extLEDblink = EXTRA_LED_BLINK_RATE+1;
        clearDisplay();
        for (uint8_t x=0; x<16; x++) { drawLEDs(); }
        eeprom_write_block((const void*)&dmSteps, (void*)0, sizeof(dmSteps));
      }
      break;
      
      // ---------------------------- //
      case B00101000: // Pattern Special Functions
      {
        waitSerialNextByte();        
        uint8_t fFunc = Serial.read();
        //
        switch (fFunc)
        {
          case 1: initPattern(currentPattern); break;
          case 2: copyPatternToBuffer(); break;
          case 3: copyBufferToPattern(); break;
          case 4: initCurrentTrack(); break;
        }
      }
      break;
      
      // ---------------------------- //
      case B00100111: // Tracks OnOff //
      {
        waitSerialNextByte();        
        uint8_t tracksOnOffBuffer = Serial.read();
        for (char xdtm=0; xdtm<DRUMTRACKS; xdtm++)
        {
          if (bitRead(tracksOnOff, xdtm) && !bitRead(tracksOnOffBuffer, xdtm)) sendseqNoteOff(dmNotes[xdtm], destChannel[xdtm]);
        }
        tracksOnOff = tracksOnOffBuffer;
      }
      break;
      
      // ---------------------------- //
      case B01000010:  // Set Note Track LEDs
      {
        waitSerialNextByte();
        uint16_t sReadA = Serial.read();
        waitSerialNextByte();
        uint16_t sReadB = Serial.read();
        noteTrackLEDs = (sReadA << 8) | sReadB;
      }
      break;
      
      // ---------------------------- //
      case B00100001: // Touch Sensor - 1 extra bytes //
      {
        waitSerialNextByte();
        byte sRead = Serial.read();
        byte xCode = sRead >> 5; 
        byte xButton = sRead & B00011111;
        if (xCode == 0) // Touch Sensor Click
        {
          if (xButton >= 4) 
          {
            byte xButton4 = xButton-4;
            if (holdingLeft == 1) // Select Track //
            {
              if (xButton4 < DRUMTRACKS) currentTrack = xButton4;
            }
            else if (holdingRight == 1)
            {
            }
            else 
            {
              buttonClick(xButton4);
            }
          }
        }
        else if (xCode == 1) // Touch Sensor Hold
        {
          if (xButton == 2) holdingLeft = 1;
          else if (xButton == 3) holdingRight = 1;
          else if (xButton >= 4 && holdingLeft == 0 && holdingRight == 0) buttonClick(xButton-4, 1);
        }
        else if (xCode == 2) // Touch Sensor Release
        {
          if (xButton == 2) holdingLeft = 0;
          else if (xButton == 3) holdingRight = 0;
        }
      }
      break;
      
      // ---------------------------- //
      case B00100011: // Change Pattern //
      {
        waitSerialNextByte();
        currentPattern = Serial.read();
      }
      break;
    
      // ---------------------------- //
      case B00100101: // Seq Position      
      {
        waitSerialNextByte();
        seqClockCounter = Serial.read();
      }
      break;
      
      // ---------------------------- //
      case B00100100: // Steps Position
      {
        waitSerialNextByte();
        stepsPos = Serial.read();
      }
      break;
      
      // ---------------------------- //
      case B01100100: // Sequencer to SND Generator -> Convert to MIDI //
      {
        waitSerialNextByte();
        byte msg1 = Serial.read();
        byte xT = msg1 >> 5; // Type
        byte xCh = msg1 & B00011111; // Channel
        switch (xT)
        {
          case 0: MSerial.write(0x90+xCh); break;
          case 1: MSerial.write(0x80+xCh); break;
          case 2: MSerial.write(0xB0+xCh); break;
          case 3: MSerial.write(0xB0+xCh); break;
        }
        //
        waitSerialNextByte();
        byte msg2 = Serial.read();
        //
        if (xT == 2) Serial.write(0x7B); else Serial.write(msg2);
        //
        waitSerialNextByte();
        byte msg3 = Serial.read();
        Serial.write(msg3);
      }
      break;  
      
      // ---------------------------- //
      default: // Other codes // ------------------------------------------- //
      {
        while (nBytes > 0)
        {
          waitSerialNextByte();
          nBytes--;
        }
      }      
    }
  }
  //
  // --------------------------------------- //
  //
  setLEDs();
  drawLEDs();
}
