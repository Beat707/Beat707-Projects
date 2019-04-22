/*
 * 
 * Beat707 MIDI Box
 * Created by William Kalfelz @ Beat707 (c) 2017 - http://www.Beat707.com
 * 
 */

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void loop()
{
  while (XSerial.hasSomething())
  {
    byte xbyte0 = SerialNextByte();
    //
    if (xbyte0 == 0xF0) // System Exclusive Data - SysEx =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    {
      XSerial.writeByte(xbyte0);
      while (1)
      {
        LED_Activity_On(LED_ACTIVITY_TIME);
        xbyte0 = SerialNextByte();
        XSerial.writeByte(xbyte0);
        if (xbyte0 == 0xF7) break;
      }
    }
    else if (xbyte0 == 0xF8     // MIDI Clock (96PPQ) =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
            || xbyte0 == 0xFA   // MIDI Clock Start
            || xbyte0 == 0xFB   // MIDI Clock Continue
            || xbyte0 == 0xFC   // MIDI Clock Stop
            || xbyte0 == 0xFE   // Active Sensing
            || xbyte0 == 0xF6   // Tune Request
            || xbyte0 == 0xFF)  // System Reset
    {
      if (!(allSettings.settings[kMode] == kModeIgnoreStartStop && (xbyte0 == 0xFA || xbyte0 == 0xFB || xbyte0 == 0xFC))) XSerial.writeByte(xbyte0);
      LED_Activity_On(LED_ACTIVITY_TIME);
    }
    else // Other Types // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    {
      byte nBytes = 2;
      byte xbyte1 = SerialNextByte();
      byte xbyte2 = 0;
      bool doEcho = true;
      //
      switch(xbyte0 & 0xF0)
      {
        case 0x80:
        case 0x90:
        case 0xA0:
        case 0xB0:
        case 0xF2:
          xbyte2 = SerialNextByte();
          break;
      }
      //
      switch(xbyte0 & 0xF0)
      {
        case 0x80: // Note Off =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
          {
            if (learnNewSettings != 0)
            {
              doEcho = false;
              if (learnNewSettings > learnNewSettingsSize) setupNewMode();
                else if (!errorLearning) LED_Activity_On(LED_ACTIVITY_TIME);
            }
            else
            {
              midiChannelCheck(xbyte0);
            }
            break;
          }
        //
        case 0x90: // Note On =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
          {
            if (learnNewSettings != 0)
            {
              doEcho = false;
              if (learnNewSettings <= learnNewSettingsSize)
              {
                if (learnNewSettings == 1)
                {
                  if (xbyte1 < 60 || xbyte1 >= (60 + kMode_Last))
                  {
                    learnNewSettings = 1;
                    errorLearning = true;
                  }
                  else
                  {
                    allSettings.settings[kMode] = xbyte1 - 60;
                    errorLearning = false;
                    if (allSettings.settings[kMode] == kMode_None) setupNewMode(); else learnNewSettings++;
                    //
                    switch(allSettings.settings[kMode])
                    {
                      case kMode_KeySwitching: learnNewSettingsSize = kKeyS_Set_Last; break;
                      case kMode_LowKeysToChannel: learnNewSettingsSize = kKeySM_Set_Last; break;
                      case kMode_Sequencer: learnNewSettingsSize = kSequencer_Last; break;
                    }
                  }
                }
                else 
                {
                  allSettings.settings[learnNewSettings-1] = xbyte1;
                  learnNewSettings++;
                }
              }
            }
            else
            {
              doEcho = noteOn(xbyte1, xbyte2);
              midiChannelCheck(xbyte0);
            }
            break; 
          }
        //
        // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        //
        case 0xA0: midiChannelCheck(xbyte0); break; // Poly Aftertouch
        case 0xB0: midiChannelCheck(xbyte0); break; // Control Change
        case 0xC0: nBytes = 1; break; // Program Change
        case 0xD0: midiChannelCheck(xbyte0); nBytes = 1; break; // Channel Aftertouch
        case 0xE0: midiChannelCheck(xbyte0); nBytes = 1; break; // Pitch Wheel
        case 0xF1: nBytes = 1; break; // MIDI Timing Code
        case 0xF3: nBytes = 1; break; // Song Select
        case 0xF2: break; // Song Position Pointer
      }
      //
      if (doEcho)
      {
        XSerial.writeByte(xbyte0);
        XSerial.writeByte(xbyte1);
        if (nBytes > 1) XSerial.writeByte(xbyte2);
        LED_Activity_On(LED_ACTIVITY_TIME);
      }
    }
  }
  //
  if ((PINA & (1<<PA0)) == 0)
  {
    if (buttonCheck[0] < 255) buttonCheck[0]++;
    if (buttonCheck[0] == 255)
    {
      buttonCheck[0] = 0;
      if (buttonCheck[1] < 254) buttonCheck[1]++;
      if (buttonCheck[1] == 254)
      {
        LED_STATE(HIGH);
        stopSequencer();
        //
        buttonCheck[1] = 255;
        learnNewSettings = 1;
        learnNewSettingsSize = 2;
        errorLearning = false;
        allSettings.init();
      }
    }
  }
  else
  {
    buttonCheck[0] = buttonCheck[1] = 0;
  }
  //
  if (led_On[0] > 0)
  {
    led_On[0]--;
    if (led_On[0] == 0) 
    {
      if (led_On[1] == 0) LED_STATE(LOW);
      else
      {
        led_On[0] = 250;
        led_On[1]--;
      }
    }
  }
}

