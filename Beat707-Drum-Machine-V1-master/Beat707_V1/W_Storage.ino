/*

  Created by Beat707 (c) 2011 - http://www.Beat707.com
  
  Storage (external EEPROM) Init, Reading and Saving
    
*/

// ======================================================================================= //
#define prePosLS 28
#define prePos (prePosLS+DRUMTRACKS+2+DRUMTRACKS+(MAXSONGPOS*2)) // 256 bytes - should always be a multple of 64 for the EEPROM page write faster code
#define patternSizeBytes (((DRUMTRACKS+2)*4*2)+128) // First numbers are 16 bits, that's why the extra *2 - Total of 256 bytes //
#define totalSongSize prePos+(patternSizeBytes*MAXSPATTERNS)  // 23296 bytes (99 SongsPos + 90 Patterns) + 14 bytes for the song name = 23310 (6 sectors of Flash has 24576 of space)
                                                              // A special note about totalSongSize, the flash code writes in pairs of 2 bytes, (to speed up) so the song size should be a multiple of 2

/*

  Organization of the data
  
  Byte = Data
  0000 = B (Header)
  0001 = 7 (Header)
  0002 = 0 (Header)
  0003 = 7 (Header)
  0004 = (Reserved)
  0005 = (Reserved)
  0006 = MidiClockType
  0007 = Song Version #
  0008 = Time Scale (1/16 or 1/32)
  0009 = Midi Clock BPM Tempo
  0010 = Unit ID (SysEx)
  0011 = Auto Steps
  0012 = Mirror Pattern Edit
  0013 = Midi Clock Shuffle
  0014 = Number of Steps per Pattern (Global)
  0015 = Enable A/B Patterns
  0016 = Analog Input Mode (when #define ANALOG_INPUT_A0 is set)
  0017 = Midi USB Mode
  0018 = External MIDI PortSelector - used by the USB to MIDI Converter  
  0019 = Midi Clock Direction
  ....
  0028 = End of the generic header
  
*/

// ======================================================================================= //
void savePattern(uint8_t saveAccentsOnly)
{
  patternChanged = wireBufferCounter = 0;
  
  for (char q=0; q<((DRUMTRACKS+2)*4); q++) 
  { 
    if (wireBufferCounter == 0) wireBeginTransmission((prePos+(q*2)+(currentPattern*patternSizeBytes)));
    byte lowByte = (((int)dmSteps[patternBufferN][q] >> 0) & 0xFF);
    byte highByte = (((int)dmSteps[patternBufferN][q] >> 8) & 0xFF);
    if (saveAccentsOnly && q != (((DRUMTRACKS+2)*0)+DRUMTRACKS) && q != (((DRUMTRACKS+2)*1)+DRUMTRACKS) && q != (((DRUMTRACKS+2)*2)+DRUMTRACKS) && q != (((DRUMTRACKS+2)*3)+DRUMTRACKS)) lowByte = highByte = 0;
    Wire.send(lowByte);
    Wire.send(highByte);
    wireBufferCounter += 2;
    wireWrite64check(true);
  }
  wireWrite64check(false);
  
  wireBeginTransmission(prePos+(currentPattern*patternSizeBytes)+((DRUMTRACKS+2)*8));
  for (char q=0; q<64; q++) Wire.send((saveAccentsOnly) ? 0 : dmSynthTrack[0][patternBufferN][q]);
  wireEndTransmission();

  wireBeginTransmission(prePos+(currentPattern*patternSizeBytes)+64+((DRUMTRACKS+2)*8));
  for (char q=0; q<64; q++) Wire.send((saveAccentsOnly) ? 0 : dmSynthTrack[1][patternBufferN][q]);
  wireEndTransmission();
}

// ======================================================================================= //
void loadPattern(uint8_t mergePattern)
{
  wireBeginTransmission(prePos+(currentPattern*patternSizeBytes));
  Wire.endTransmission();
  Wire.requestFrom(0x50,((DRUMTRACKS+2)*8));
  for (char q=0; q<((DRUMTRACKS+2)*4); q++)
  {
    if (mergePattern) dmSteps[!patternBufferN][q] |= ((Wire.receive() << 0) & 0xFF) + ((Wire.receive() << 8) & 0xFF00);
      else dmSteps[!patternBufferN][q] = ((Wire.receive() << 0) & 0xFF) + ((Wire.receive() << 8) & 0xFF00);
  }

  wireBeginTransmission(prePos+(currentPattern*patternSizeBytes)+((DRUMTRACKS+2)*8));
  Wire.endTransmission();
  Wire.requestFrom(0x50,128);
  for (char q=0; q<64; q++) { if (mergePattern) dmSynthTrack[0][!patternBufferN][q] |= Wire.receive(); else dmSynthTrack[0][!patternBufferN][q] = Wire.receive(); }
  for (char q=0; q<64; q++) { if (mergePattern) dmSynthTrack[1][!patternBufferN][q] |= Wire.receive(); else dmSynthTrack[1][!patternBufferN][q] = Wire.receive(); }
}

#if EXTRA_MIDI_IN_H_2
  // ======================================================================================= //
  void loadPatternPartial()
  {
    wireBeginTransmission(prePos+(currentPattern*patternSizeBytes));
    Wire.endTransmission();
    Wire.requestFrom(0x50,((DRUMTRACKS+2)*2));
    for (char q=0; q<(DRUMTRACKS+2); q++)
    {
      dmSteps[!patternBufferN][q] = ((Wire.receive() << 0) & 0xFF) + ((Wire.receive() << 8) & 0xFF00);
    }
  
    wireBeginTransmission(prePos+(currentPattern*patternSizeBytes)+((DRUMTRACKS+2)*8));
    Wire.endTransmission();
    Wire.requestFrom(0x50,1);
    dmSynthTrack[0][!patternBufferN][0] = Wire.receive();
    wireBeginTransmission(prePos+64+(currentPattern*patternSizeBytes)+((DRUMTRACKS+2)*8));
    Wire.endTransmission();
    Wire.requestFrom(0x50,1);
    dmSynthTrack[1][!patternBufferN][0] = Wire.receive();
  }
#endif

// ======================================================================================= //
void loadSongPosition()
{
  if (curSongPosition < 0) curSongPosition = 0;
  
  patternSong = EEPROM_READ((prePosLS+(DRUMTRACKS*2)+2)+curSongPosition);
  patternSongRepeat = EEPROM_READ((prePosLS+(DRUMTRACKS*2)+2+MAXSONGPOS)+curSongPosition);
}

// ======================================================================================= //
void loadSongNextPosition()
{
  if ((curSongPosition+1) < MAXSONGPOS) patternSongNext = EEPROM_READ((prePosLS+(DRUMTRACKS*2)+2)+(curSongPosition+1)); else patternSongNext = patternSong;
    
  if (patternSongNext == 1) 
  {
    patternSongNext = EEPROM_READ((prePosLS+(DRUMTRACKS*2)+2)); 
    patternSongRepeatNext = EEPROM_READ((prePosLS+(DRUMTRACKS*2)+2+MAXSONGPOS));
    songLoopPos = curSongPosition;
    curSongPosition = -1; 
  }
  else
  {
    patternSongRepeatNext = EEPROM_READ((prePosLS+(DRUMTRACKS*2)+2+MAXSONGPOS)+curSongPosition+1);
  }
}

// ======================================================================================= //
void saveSongPosition()
{
  songChanged = 0;
  
  EEPROM_WRITE((prePosLS+(DRUMTRACKS*2)+2)+curSongPosition,patternSong);
  EEPROM_WRITE((prePosLS+(DRUMTRACKS*2)+2+MAXSONGPOS)+curSongPosition,patternSongRepeat);
}

// ======================================================================================= //
void saveSetup()
{
  setupChanged = 0;
  
  wireBeginTransmission(0);
  Wire.send('B');
  Wire.send('7');
  Wire.send('0');
  Wire.send('7');
  for (char q=4; q<prePosLS; q++)
  {
    uint8_t value = 0;
    if (q == 6) value = midiClockType;
      else if (q == 7) value = SONG_VERSION;
      else if (q == 8) value = timeScale;
      else if (q == 9) value = midiClockBPM;
      else if (q == 10) value = sysMIDI_ID;
      else if (q == 11) value = autoSteps;
      else if (q == 12) value = mirrorPatternEdit;
      else if (q == 13) value = midiClockShuffle;
      else if (q == 14) value = numberOfSteps;
      else if (q == 15) value = enableABpattern;
      #if ANALOG_INPUT_A0
      else if (q == 16) value = analogInputMode;
      #endif
      else if (q == 17) value = midiUSBmode;
      else if (q == 18) value = externalMIDIportSelector; // slot 18 is used for an external variable: MIDI Out Port - for the USB to MIDI Converter  
      else if (q == 19) value = midiClockDirection;
      #if BEAT707_BIG_TIME
        // 20 = Beats (4, 8) 
        // 21 = showDots (0, 1)
        // 22 = Mode (0, 1, 2)
      #endif
      
    Wire.send(value);
  }
  for (char x=0; x<DRUMTRACKS; x++) Wire.send(dmNotes[x]); 
  for (char x=0; x<DRUMTRACKS+2; x++) Wire.send(dmChannel[x]); 
  wireEndTransmission();
}

// ======================================================================================= //
void loadSetup()
{
  midiClockType = EEPROM_READ(6);
  timeScale = EEPROM_READ(8);
  midiClockBPM = EEPROM_READ(9);
  sysMIDI_ID = EEPROM_READ(10);
  autoSteps = EEPROM_READ(11);
  mirrorPatternEdit = EEPROM_READ(12);
  midiClockShuffle = EEPROM_READ(13);
  numberOfSteps = EEPROM_READ(14);
  if (numberOfSteps == 0 || numberOfSteps > 16) numberOfSteps = 16;
  enableABpattern = EEPROM_READ(15);
  #if ANALOG_INPUT_A0
    analogInputMode = EEPROM_READ(16);
  #endif
  midiUSBmode = EEPROM_READ(17);
  externalMIDIportSelector = EEPROM_READ(18); // slot 18 is used for an external variable: MIDI Out Port - for the USB to MIDI Converter  
  midiClockDirection = EEPROM_READ(19);
  
  wireBeginTransmission(prePosLS);
  Wire.endTransmission();
  Wire.requestFrom(0x50,((DRUMTRACKS*2)+2));
  for (char x=0; x<DRUMTRACKS; x++) dmNotes[x] = Wire.receive();
  for (char x=0; x<DRUMTRACKS+2; x++) dmChannel[x] = Wire.receive();
}

// ======================================================================================= //
boolean checkStorageHeader()
{
  #if DISABLE_STORAGE_CHK
    return true;
  #endif
  
  if (EEPROM_READ(7) != SONG_VERSION || EEPROM_READ(0) != 'B' || EEPROM_READ(1) != '7' || EEPROM_READ(2) != '0' || EEPROM_READ(3) != '7') return false;
  return true;
}

// ======================================================================================= //
void storageInit(uint8_t forceInit)
{
  if (STORAGE_FORCE_INIT || !checkStorageHeader() || forceInit)
  {
    lcd.clear();
    lcd.setCursor(2,0);
    lcdPrint(INIT_STORAGE);
    lcd.setCursor(3,1);
    if (!hitShiftToConfirm()) return;

    lcd.clear();
    lcd.setCursor(1,0);        
    lcdPrint(ARE_YOU_SUREQ);
    if (!hitShiftToConfirm()) return; // Just to be sure an accidental INIT doesn't happen //
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcdPrint(INIT_STARTING);
    lcd.setCursor(7,1); lcd.write('3');
    
    // Init Variables //
    sysInit();

    // Save Setup //
    saveSetup();
    for (int q=0; q<MAXSONGPOS; q++)
    {
      EEPROM_WRITE((prePosLS+(DRUMTRACKS*2)+2)+q,0);
      EEPROM_WRITE((prePosLS+(DRUMTRACKS*2)+2+MAXSONGPOS)+q,0);
    }
    EEPROM_WRITE((prePosLS+(DRUMTRACKS*2)+2),2); // So the song is not empty //
    
    lcd.setCursor(7,1); lcd.write('2');
    
    for (char x=0; x<=MAXSPATTERNS; x++) // We go above MAXSPATTERNS as the last is for the Copy/Paste code
    {   
      currentPattern = x; 
      savePattern(0);
    }
    currentPattern = 0;
    
    lcd.setCursor(7,1); lcd.write('1');

    flashTotalErase();
    delayNI(1000);
    
    #if INIT_EMPTY_SONG
      lcd.setCursor(7,1); lcd.write('0');
      fileSelected = 0;
      strcpy(fileSongName,"Init Song");
      delayNI(1000);
      songSave();
    #endif

    lcdOK();
    delayNI(2000);
  }
}

// ======================================================================================= //
// ======================================================================================= //
// ======================================================================================= //
// ======================================================================================= //

/*

  Flash 4Mbit = 4 * 1024 * 1024 = 4194304 bits / 8 = 524288 bytes = 512 KBytes
  The address byte requires 20 bits = 10000000000000000000 = 524288 (the chip requires a 24 bit address number)
  4Kbyte Sectors = 4 * 1024 = 4096 bytes = 128 sectors for the whole chip
  Each song, using 99 SongsPos and 90 Patterns (+14 char for the song name) = 23310 bytes = 6 sectors = 21 Songs on the chip  
  Plus, the Flash Write code saves data in pairs of 2 bytes, therefore, song size should be a multiple of 2.

*/

void loadSongName(void)
{
  memset(fileSongName,0,sizeof(fileSongName));
  
  // Check if the current selected song slot is not empty //
  if (checkSong())
  {
    flashReadInit(fileSelected*6);
    for (int q=0; q<14; q++) { fileSongName[q] = flashReadNext();  }
    flashReadFinish();
  }
  else
  {    
    strcpy(fileSongName,"Empty Song");
  }
}

// ======================================================================================= //

uint8_t checkSong(void)
{   
  uint8_t chSng = true;

  flashReadInit(fileSelected*6);
  for (int x=0; x<14; x++) flashReadNext();
  if (flashReadNext() != 'B') chSng = false;
  if (flashReadNext() != '7') chSng = false;
  if (flashReadNext() != '0') chSng = false;
  if (flashReadNext() != '7') chSng = false;
  flashReadFinish();
  
  return chSng;
}

// ======================================================================================= //
void songErase()
{
  flashSectorsErase(fileSelected*6,6);
}

// ======================================================================================= //

void songLoad()
{
  wireBufferCounter = 0;
  flashReadInit(fileSelected*6); 
  for (int q=0; q<14; q++) { fileSongName[q] = flashReadNext();  }     
  for (int q=0; q<totalSongSize; q++) 
  {     
    if (wireBufferCounter == 0) wireBeginTransmission(q);
    Wire.send(flashReadNext());
    wireBufferCounter++;
    wireWrite64check(true);
  }
  wireWrite64check(false);
  flashReadFinish();
  loadSetup();  
}

// ======================================================================================= //

void songSave()
{
  if (checkSong()) songErase();
  if (!strcmp(fileSongName,"Empty Song")) sprintf(fileSongName, "Song %02d",fileSelected+1);
  flashPageWriteInit(fileSelected*6, fileSongName[0], fileSongName[1]);
  for (int q=2; q<14; q += 2) { flashPageWriteNext(fileSongName[q], fileSongName[q+1]); }
  for (int q=0; q<totalSongSize; q += 2) { flashPageWriteNext(EEPROM_READ(q), EEPROM_READ(q+1)); }
  flashPageWriteFinish();
}

// ======================================================================================= //
  
void songDumpReceive(void)
{
  uint32_t address = 0;
  wireBufferCounter = 0;
    
  if (midiClockRunning) goto sysExEnd;
  
  // First check Manufacturer's ID bytes 1 to 6 and User's ID //
  if (midiInput() != 0x01) goto sysExEnd;
  if (midiInput() != 0x08) goto sysExEnd;
  if (midiInput() != 0x04) goto sysExEnd;
  if (midiInput() != 0x02) goto sysExEnd;
  if (midiInput() != 0x09) goto sysExEnd;
  
  if (midiInput() == 100)
  {
    if (midiInput() != sysMIDI_ID) goto sysExEnd;
    // Special USB Check for the USB to MIDI Program //
    lcd.clear();
    lcd.setCursor(1,0);
    lcdPrint(USB_TO_MIDI_OK);
    #if !MIDIECHO
      MSerial.write(240);
    #endif
    MSerial.write('B'); MSerial.write('7'); MSerial.write('0'); MSerial.write('7'); MSerial.write(EEPROM_READ(18));
    #if !MIDIECHO
      MSerial.write(247);
    #endif
    delayNI(2000);
    goto sysExEnd;
  }
  else if (incomingByte == 101)
  {
    // Stores external selected MIDI Output Port from the USB to MIDI converter program
    if (midiInput() != sysMIDI_ID) goto sysExEnd;
    externalMIDIportSelector = midiInput();
    EEPROM_WRITE(18,externalMIDIportSelector);
    goto sysExEnd;
  }
  #if MANAGER_DUMP_RECV
    else if (incomingByte == 102)
    {
      if (midiInput() != sysMIDI_ID) goto sysExEnd;
      #if !MIDIECHO
        MSerial.write(240);
      #endif
      MSerial.write('B'); MSerial.write('7'); MSerial.write('0'); MSerial.write('7'); MSerial.write(numerOfSongsOnFlashMemory);
      #if !MIDIECHO
        MSerial.write(247);
      #endif
      delayNI(50);
      goto sysExEnd;
    }  
    else if (incomingByte == 103)
    {
      if (midiInput() != sysMIDI_ID) goto sysExEnd;
      uint8_t procSongsToo = 0;
      if (midiInput() == 0x01) procSongsToo = 1;
      lcd.clear();
      lcd.setCursor(2,0);
      lcdPrint(PROCESSING);
      stepLEDs[0] = stepLEDs[1] = stepLEDs[2] = 0;
      // Selects Beat707 Manager Dump Mode - Dumps via Serial all Machine Data for Backup on a Computer (both EEPROM and Nand Flash)
      #if !MIDIECHO
        MSerial.write(240);
      #endif
      delayNI(10);
      rawMIDImode = 1;
      uint16_t checkData = 0;
      uint8_t tempData = 0;
      // Send EEPROM Data First //
      for (unsigned int q=0; q<23296; q += 64) 
      { 
        checkData = 0;
        wireBeginTransmission(q);
        Wire.endTransmission();
        Wire.requestFrom(0x50,64);
        for (char q=0; q<64; q++) 
        { 
          tempData = Wire.receive();
          MSerial.write(tempData);
          checkData += tempData;
        }
        MSerial.write((checkData/64));
        if (midiInput() != 0x00)
        {
          MSerial.write(247);
          rawMIDImode = 0;
          doLCDupdate = 1;
          return;            
        }
      }
      
      // Now, if asked for, send Flash Data //
      if (procSongsToo == 1)
      {
        for (char xs=0; xs<MAXSONGSFILE; xs++)
        {
          fileSelected = xs;
          if (checkSong())
          {
            flashReadInit(fileSelected*6);
            MSerial.write(0xFF); // New Song Data
            midiInput(); // Wait
            for (char x=0; x<14; x++) { MSerial.write(flashReadNext()); }
            midiInput(); // Wait
            for (unsigned long q=0; q<23296; q += 64)
            {
              checkData = 0;
              for (char x=0; x<64; x++)
              {
                tempData = flashReadNext();
                MSerial.write(tempData);
                checkData += tempData;
              }
              MSerial.write((checkData/64));
              if (midiInput() != 0x00)
              {
                MSerial.write(247);
                rawMIDImode = 0;
                doLCDupdate = 1;
                return;            
              }              
            }
            flashReadFinish();
          }
        }
        fileSelected = 0;
        MSerial.write(0x25); // End Songs
      }
      
      rawMIDImode = 0;
      MSerial.write(247);
      doLCDupdate = 1;
      return;
    }
    else if (incomingByte == 104)
    {
      if (midiInput() != sysMIDI_ID) goto sysExEnd;
      uint8_t procSongsToo = 0;
      if (midiInput() == 0x01) procSongsToo = 1;
      rawMIDImode = 1;
      lcd.clear();
      lcd.setCursor(2,0);
      lcdPrint(PROCESSING);
      stepLEDs[0] = stepLEDs[1] = stepLEDs[2] = 0;
      #if !MIDIECHO
        MSerial.write(240);
      #endif
      // Selects Beat707 Manager Receive Mode - Receives via Serial all Machine Data for Backup from a Computer (both EEPROM and Nand Flash)
      uint16_t checkData = 0;
      while (address < 23296)
      {
        // First EEPROM //
        midiInput();
        if (wireBufferCounter == 0) wireBeginTransmission(address);
        Wire.send(incomingByte);
        wireBufferCounter++;
        checkData += incomingByte;
        if (wireBufferCounter == 64)
        {
          wireWrite64check(true);
          MSerial.write(checkData/64);
          checkData = 0;
        }
        address++;
      }
      wireWrite64check(false);
            
      if (procSongsToo == 1)
      {
        wireBufferCounter = 0;
        uint8_t startFirst = 1;
        uint8_t fileCounter = 0;
        checkData = 0;
        
        if (midiInput() == 0x00)
        {
          flashTotalErase();
          delayNI(1000);
          MSerial.write((byte)0x00);
          
          while (1)
          {
            fileSelected = fileCounter;
            address = 0;
            wireBufferCounter = 0;
            while (1)
            {
              TwoWire::rxBuffer[wireBufferCounter] = midiInput();
              wireBufferCounter++;
              if (wireBufferCounter == 14)
              {
                flashPageWriteInit(fileSelected*6, TwoWire::rxBuffer[0], TwoWire::rxBuffer[1]);
                for (char x=0; x<12; x+=2) flashPageWriteNext(TwoWire::rxBuffer[x+2], TwoWire::rxBuffer[x+3]);
                MSerial.write((byte)0x00);
                break;
              }
            }
            wireBufferCounter = 0;
            while (1)
            {
              TwoWire::rxBuffer[wireBufferCounter] = midiInput(); // here we re-used the 2Wire buffer to keep memory usage lower
              checkData += incomingByte;
              wireBufferCounter++;
              if (wireBufferCounter == 64)
              {
                for (char x=0; x<64; x+=2) { flashPageWriteNext(TwoWire::rxBuffer[x], TwoWire::rxBuffer[x+1]); }
                MSerial.write(checkData/64);
                checkData = 0;
                address += 64;
                wireBufferCounter = 0;
                if (address == 23296) break;
              }
            }
            flashPageWriteFinish();
            fileCounter++;
            if (midiInput() == 0xFF) break;
          }
          fileSelected = 0;
        }
      }
      
      // End //
      MSerial.write(247);
      rawMIDImode = 0;
      currentPattern = 0;
      loadPattern(false);
      patternBufferN = !patternBufferN;      
      doLCDupdate = 1;
      return;
    }
  #endif
  else if (incomingByte != sysMIDI_ID) goto sysExEnd;
    
  sysExEnd:  
  while (incomingByte != 247) { midiInput(); }
  doLCDupdate = 1;
}
