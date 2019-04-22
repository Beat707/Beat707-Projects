/*

  Created by Guilherme (William) Kalfelz
  @ Beat707 (c) 2014 - http://www.Beat707.com

*/

// ======================================================================================= //
#define TNOTE(a) last_settings.noteSteps[currentPattern][a][editNTrackPos]
#define TSNG(a) last_settings.songData[a][songEditPos]
#define TSPF(a) last_settings.perfData[a][songPerfPos]
enum { kLeft=2, kRight, kClick=0, kHold, kRelease };

// ======================================================================================= //
void setLCDCursor(uint8_t cPos, uint8_t cPosL)
{
  lcd.cursor();
  lcd.setCursor(cPos, cPosL);
}

// ======================================================================================= //
void showLCDmessage(uint8_t msgID1, uint8_t msgID2)
{
  lcd.clear();
  lcdPrint(msgID1);
  lcd.setCursor(0,1);
  lcdPrint(msgID2);
}

// ======================================================================================= //
void lcdPrint(uint8_t pos)
{
  uint8_t c;
  char* p = (char*)pgm_read_word(&(stringlist[pos]));
  while (c = pgm_read_byte(p)) { lcd.write(c); p++; }
}

// ======================================================================================= //
void lcdPrintString(char* string)
{
  uint8_t p = 0;
  while (string[p] != 0) { lcd.write(string[p]); p++; }
}

// ======================================================================================= //
void lcdPrintNumber(uint8_t number)
{
  lcd.write('0'+(number/10));
  lcd.write('0'+(number-((number/10)*10)));
}

// ======================================================================================= //
void lcdPrintNumberPN(int number)
{
  if (number >= 0) lcd.write('+'); else { lcd.write('-'); number*=-1; }
  lcd.write('0'+(number/10));
  lcd.write('0'+(number-((number/10)*10)));
}

// ======================================================================================= //
void lcdClearnLine(uint8_t line)
{
  lcd.setCursor(0,line);
  lcdSpaces(16);
  lcd.setCursor(0,line);
}

// ======================================================================================= //
void lcdPrintNumber3Dgts(uint8_t number)
{
  if (number >= 200) { lcd.write('2'); number -= 200; }
    else if (number >= 100) { lcd.write('1'); number -= 100; }
    else lcd.write('0');
  lcdPrintNumber(number);
}

// ======================================================================================= //
void lcdSpace()
{
  lcd.write(' ');
}

// ======================================================================================= //
void lcdSpaces(int spaces)
{
  while (spaces > 0)
  {
    lcd.write(' ');
    spaces--;
  }
}

// ======================================================================================= //
void waitSerialNextByte()
{
  while (Serial.available() == 0) { ; }
}

// ======================================================================================= //
void showB707()
{
  lcd.clear(); 
  lcdPrint(BEAT707_COM_V);
  lcd.setCursor(0,1);
  lcdPrint(PROCESSING);
}

// ======================================================================================= //
void timerStart()
{
  TCCR1A = TCCR1B = 0;
  bitWrite(TCCR1B, CS11, 1);
  bitWrite(TCCR1B, WGM12, 1);
  timerSetFrequency();
  bitWrite(TIMSK1, OCIE1A, 1);
}

void timerSetFrequency()
{
  // Calculates the Frequency for the Timer, used by the PPQ clock (Pulses Per Quarter Note) //
  // This uses the 16-bit Timer1, unused by the Arduino, unless you use the analogWrite or Tone functions //
  #define frequency (((last_settings.seq_bpm)*(PPQ))/60)
  OCR1A = (F_CPU/ 8) / frequency - 1;
}

void timerStop(void)
{
  bitWrite(TIMSK1, OCIE1A, 0);
  TCCR1A = TCCR1B = OCR1A = 0;
}

// ======================================================================================= //
void lateClockStop()
{
  timerStop();
  doLateClockStop = 1;
}

// ======================================================================================= //
void checkNextPattern()
{
  if (currentPattern != nextPattern)
  {
    currentPattern = nextPattern;
    Serial.write(B00100011); // Send to all Slave Units //
    Serial.write(currentPattern);
    doLCDupdate++;
    if (currentTrack >= 4 || last_settings.bMode >= kModeSong) updateNoteTrackLEDs();
  }
}

// ======================================================================================= //
void seqClockNewTime()
{
  if (seqClockRunning) timerSetFrequency();
}

// ======================================================================================= //
void seqShuffleUpdate()
{
  seqClockShuffleData[0][1] = 12+last_settings.seqClockShuffle;
  seqClockShuffleData[1][1] = 6+last_settings.seqClockShuffle;
  seqClockShuffleData[0][2] = 12-last_settings.seqClockShuffle;
  seqClockShuffleData[1][2] = 6-last_settings.seqClockShuffle;
}

// ======================================================================================= //
void seqClockStart(uint8_t restart)
{
  if (last_settings.bMode >= kModeSong)
  {
    if (last_settings.bMode == kModeSongPerf)
    {
      songEditPos = TSPF(0);
      songPerfPosCounter = 0;
    }
    //
    if (TSNG(0) == 0) return;
    editMode(0);
    currentPattern = nextPattern = TSNG(0)-1;
    tracksOnOff = TSNG(2) & B00011111;
    songRepeatCounter = 0;
    if (last_settings.bMode == kModeSong) editZone = kZoneSongPat;
    switch ((TSNG(2) >> 5))
    {
      case 1:
      {
        patternFade = 1;
        patternFadeCounter = 0;        
        Serial.write(16); 
      }
      break;
      
      case 2:
      {
        patternFade = -1;
        patternFadeCounter = 128;        
        Serial.write(17); 
      }
      break;      
    }
  }
  //
  seqShuffleUpdate();  
  seqClockRunning = 1;
  prevseqClockCounter = 15;
  stepsPos = seqClockShuffleCounter = 0;
  if (autoStepPos == 1) editStepsPos = 0;
  //
  if (last_settings.bMode >= kModeSong)
  {
    uint8_t stP = (TSNG(1) >> 5);
    if (stP == 4) stepsPos = 2;
    else if (stP == 5) stepsPos = 3;
  }
  //
  if (restart)
  {
    Serial.write(B00100111);
    Serial.write(tracksOnOff);    
    Serial.write(B00100011); // Send to all Slave Units //
    Serial.write(currentPattern);  
    Serial.write(1); // Start the Sequencer on all Slave Units //
    Serial.write(B00100101); // Seq Position
    Serial.write(0);        
    Serial.write(B00100100); // Steps Position
    Serial.write(stepsPos);
    updateAutoSteps();
    seqClockCounter = seqClockCounterDivider = 0;
    if (currentTrack >= 4) updateNoteTrackLEDs();
    tracksOnOffBuffer = tracksOnOff;
  }
  timerStart();
  doLCDupdate++;
}

// ======================================================================================= //
void seqClockStop()
{
  memset(songPerfPosNext, -1, sizeof(songPerfPosNext));
  seqClockRunning = stepsPos =  seq_beat_counter = seq_beat = patternFade = 0;
  if (autoStepPos == 1) editStepsPos = 0;
  timerStop();
  delay(10);
  tClock = 0;
  //
  if (prevNote > 0) sendseqNoteOff(prevNote);       
  prevNote = 0;
  //
  Serial.write(B01100100);  // Sequencer to SND Generator //
  Serial.write(B0100000 | (MIDI_CHANNEL_NOTE_TRACK & B00011111));  // Reset //
  Serial.write(0);
  Serial.write(0);
  //
  currentPattern = nextPattern;  
  Serial.write(B00100101);
  Serial.write(currentPattern);
  Serial.write(B00100101); // Seq Position
  Serial.write(0);        
  Serial.write(B00100100); // Steps Position
  Serial.write(0);  
  updateAutoSteps();
  Serial.write(2); // Stop Sequencer on all Slave Units //   
  //
  delay(50);
  if (currentTrack >= 4) updateNoteTrackLEDs();
  doLCDupdate++;
}

// ======================================================================================= //
void volatile nop(void) { asm __volatile__ ("nop"); }

// ======================================================================================= //
void printNoteName(uint8_t SynthNote)
{  
  char* p = (char*)pgm_read_word(&(stringlist[NOTENAMESLIST])) + ((SynthNote-((SynthNote > 128) ? 127 : 0)) % 12)*2;
  uint8_t c = pgm_read_byte(p);
  lcd.write(c);
  p++;
  c = pgm_read_byte(p);
  lcd.write(c);
}

// ======================================================================================= //
void updateNoteTrackLEDs()
{
  if (last_settings.bMode == kModeSongPerf && !showOptions)
  {
    Serial.write(13);
    noteTrackLEDs = 0;
    if (songPerfPosNext[0] != -1) bitSet(noteTrackLEDs, songPerfPosNext[0]);
      else bitSet(noteTrackLEDs, songPerfPos);
    //
    Serial.write(B01000010);
    Serial.write(uint8_t(noteTrackLEDs >> 8));
    Serial.write(uint8_t(noteTrackLEDs & B11111111));    
  }
  else if (last_settings.bMode == kModeSong && !showOptions)
  {
    Serial.write(13);
    noteTrackLEDs = 0;
    if (TSNG(0) > 0) bitSet(noteTrackLEDs, TSNG(0)-1);
    //
    Serial.write(B01000010);
    Serial.write(uint8_t(noteTrackLEDs >> 8));
    Serial.write(uint8_t(noteTrackLEDs & B11111111));    
  }  
  else if (currentTrack >= 4 || showOptions) 
  {
    Serial.write(13);
    //
    noteTrackLEDs = 0;
    if (showOptions)
    {
      if (showOptionsOption > 0) bitSet(noteTrackLEDs, showOptionsOption-1); 
    }
    else if (currentTrack == 5)
    {
      noteTrackLEDs = (uint16_t)tracksOnOffBuffer;
    }
    else if (currentTrack == 6)
    {
      bitSet(noteTrackLEDs, nextPattern);
    }
    else
    {
      for (int x=0; x<16; x++)
      {
        if (last_settings.noteSteps[currentPattern][0][x] > 0 
          && last_settings.noteSteps[currentPattern][1][x] > 0)
          bitSet(noteTrackLEDs, x);
      }
    }
    //
    Serial.write(B01000010);
    Serial.write(uint8_t(noteTrackLEDs >> 8));
    Serial.write(uint8_t(noteTrackLEDs & B11111111));
  }
  else Serial.write(9+currentTrack); 
}

// ======================================================================================= //
void ignoreKeysForOneSecond(uint8_t restart)
{
  if (restart == 1) ignoreKeysCounter = 20000;
  if (ignoreKeysCounter > 0) ignoreKeysCounter--;
}

// ======================================================================================= //
void initNoteTrack()
{
  for (int x=0; x<16; x++)
  {
    last_settings.noteSteps[currentPattern][0][x] = 0;
    last_settings.noteSteps[currentPattern][1][x] = 0;
  }
  last_settings.patternsTranspose[currentPattern][0] = 
  last_settings.patternsTranspose[currentPattern][1] = 
  last_settings.patternsTranspose[currentPattern][2] = 
  last_settings.patternsTranspose[currentPattern][3] = 0;
}

// ======================================================================================= //
void copyNoteTrackToBuffer()
{
  for (int x=0; x<16; x++)
  {
    noteStepsBuffer[0][x] = last_settings.noteSteps[currentPattern][0][x];
    noteStepsBuffer[1][x] = last_settings.noteSteps[currentPattern][1][x];
  }
  patternsTransposeBuffer[0] = last_settings.patternsTranspose[currentPattern][0];
  patternsTransposeBuffer[1] = last_settings.patternsTranspose[currentPattern][1];
  patternsTransposeBuffer[2] = last_settings.patternsTranspose[currentPattern][2];
  patternsTransposeBuffer[3] = last_settings.patternsTranspose[currentPattern][3];
}

// ======================================================================================= //
void copyNoteTrackFromBuffer()
{
  for (int x=0; x<16; x++)
  {
    last_settings.noteSteps[currentPattern][0][x] = noteStepsBuffer[0][x];
    last_settings.noteSteps[currentPattern][1][x] = noteStepsBuffer[1][x];
  }
  last_settings.patternsTranspose[currentPattern][0] = patternsTransposeBuffer[0];
  last_settings.patternsTranspose[currentPattern][1] = patternsTransposeBuffer[1];
  last_settings.patternsTranspose[currentPattern][2] = patternsTransposeBuffer[2];
  last_settings.patternsTranspose[currentPattern][3] = patternsTransposeBuffer[3];
}

// ======================================================================================= //
void updateAutoSteps()
{
  if (autoStepPos == 1 && seq_mirror == 1) Serial.write(14);
  else if (autoStepPos == 1 && seq_mirror == 0) Serial.write(15);  
  else Serial.write(editStepsPos+5); // Steps ABCD - Mirror Off //  
}

// ======================================================================================= //
void editMode(uint8_t seton)
{
  if (seton)
  {
    if (last_settings.bMode == kModeSongPerf && seqClockRunning)
    {
      for (int x=0; x<32; x++) 
      { 
        if (songPerfPosNext[x] == -1 && x > 0)
        {
          songPerfPosNext[x-1] = -1;
          break;
        }
      }
      doLCDupdate++;
      return;
    }
    if (last_settings.bMode == kModeSong && seqClockRunning) editZone = kZoneSongBPM;
    editingOn = 1;
    Serial.write(3); // Show Navigation Keys //              
  }
  else
  {
    if (last_settings.bMode == kModeSong && seqClockRunning) editZone = kZoneSongPat;
    editingOn = 0;
    updateNoteTrackLEDs();
    Serial.write(4); // Hide Navigation Keys //
  }
  doLCDupdate++;
}

// ======================================================================================= //
void initSlaveData()
{
  for (int x=0; x<16; x++)
  {
    Serial.write(B00100011); // Send to all Slave Units //
    Serial.write(x);      
    Serial.write(B00101000); // Pattern Special Functions
    Serial.write(1); // Init Pattern      
    delay(100);
  }
  //
  Serial.write(B00100011); // Send to all Slave Units //
  Serial.write(currentPattern);      
}

// ======================================================================================= //
void saveDataToEEPROM()
{
  /*if (seqClockRunning) seqClockStop();
  showProcessing = 1;
  showProcessingMessage = SAVE_ALL_DATA;
  doLCDupdate++;
  updateLCD();
  Serial.write(19); // Tell the Slave Unit to Save Data to EEPROM Too //
  eeprom_write_block((const void*)&last_settings, (void*)0, sizeof(last_settings));
  delay(250);
  showProcessing = 0;
  showProcessingMessage = 0;
  doLCDupdate++;*/
}
