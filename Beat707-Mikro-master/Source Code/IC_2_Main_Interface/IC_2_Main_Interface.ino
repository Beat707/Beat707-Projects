/*
 * Beat707 Mikro - created by William (Guilherme) Kalfelz (c) 2016 - www.Beat707.com - www.Wusik.com - www.William-k.com
 * 
 */

#include "Definitions.h"

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
enum
{
  patternMode = 0, songMode, 
  entirePattern = 0, currentTrack, 
  entireScreen = 0, lastLineOnly, firstLineOnly, 
  buttonUp = 0, buttonDown, buttonLeft, buttonRight, buttonPageUp, buttonPageDown, buttonShift, buttonMenu, buttonPlay, buttonStop, buttonRecord, buttonLast,
  buttonEventNone = 0, buttonEventClick, buttonEventHold, buttonEventRelease,
  regularNote = 0, noteOff, noteDouble, noteDoubleIn, noteSlide, // The last is when Velocity=0 and Note>0
  noteTrackModeNotes = 0, noteTrackModeCC,
  masterUnit = 0, slaveUnit,
  fileSong = 0, filePattern, filePatternSettings, filePatternCopy, filePatternSettingsCopy, fileDrumTrackCopy, fileNoteTrackCopy, fileChordCustom,
  fileWrite = 0, fileRead,
  stringAccent=25, stringError=33, stringOn=41, stringOff=45, stringSpacesOff=49, stringMaster=58, stringSlave=66, stringPattern=74, stringTrack=82, stringDotDotDot=88, stringMNotes=92, stringMidiCC=99,
  accentTrack=DRUM_TRACKS, noteTrack=accentTrack+2, TOTAL_TRACKS=NOTE_TRACKS+DRUM_TRACKS+ACCENT_TRACKS, lastTrack=TOTAL_TRACKS, midiChannels=16,
  //
  // ------------------------------------------------------- //
  //
  areaMain = 0, areaNoteTrackEdit, areaBPMnMIDI, areaTrackSettings, areaTrackProcessor, areaAutoStepnPatternType, areaVariations, areaSong,
    areaMenusStart, areaMenuCopyPaste, areaNewPatternGoToSong, areaCustomChord, areaSavingMessage, areaCopyingMessage, areaPastingMessage, areaNewPatternCloneCur, areaNewPatternCloneTmplt, areaMenusEnd,
  areaSongMain,
  //
  // ------------------------------------------------------- //
  //
  slvInit=4, slvInitOk=6, slvGetTrackSettings, slvGetDrumSteps, slvGetNoteSteps1, slvGetNoteSteps2, slvStop, slvPlay, 
    slvSetMirrorMode, slvSetTrackSettings, slvSetSequencerSettings, slvSetDrumSteps, slvSetNoteSteps1, slvSetNoteSteps2, slvSetSingleStep, slvGetBeat,
    slvStartMIDIserial, slvStartUSBserial, slvSetFadeMin, slvClearPattern, slvClearTrack, slvIsPlaying, slvStreamSize, 
    slvSendStream, slvStartStream, slvSetSequencerSettingsBuffer, slvSetTrackSettingsBuffer, slvSetCustomChord, slvTransposeAll, slvSlideStep, 
    slvNextSingleStepForceMirror, slvNextSingleStepNoMirror
};

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint16_t buttonsShiftHoldCounter = 4; // This is read from the SD Card during setup //
uint16_t buttonsHoldCounter = 428;
uint16_t buttonsRepeatCounter = 148;
uint16_t nextPatternCC = 2200;
boolean beatCheckLateRedraw = false;
boolean somethingChanged = false;
boolean lateRedrawDisplay = false;
boolean redrawDisplay = true;
boolean reloadScreen = true;
boolean isMirrorMode = false;
boolean hideCursor = false;
boolean holdDownPressedAnything = false;
boolean streamPatternGo = false;
boolean songSettingsChanged = false;
boolean customChordChanged = false;
boolean blinkBeat = true;
char changeVar = 0;
byte midiCClast[2] = {1, 127};
byte sdCardFileOpenRetry = 0;
byte curPlayingVariation = 0;
byte customChords[12][5];
byte customChordsZone1 = 0;
byte loadingIcon = '^';
byte wBuf[2] = {0,0};
byte patternStreamBuffer[(DRUM_TRACKS*DOUBLE_STEPS)+4];
byte copyMode = entirePattern;
byte currentMode = patternMode;
byte buttonsClickCounter = 4;
byte streamCheck = 0;
byte buttonsDecay = 2;
byte streamPattern = 0;
byte streamPosition = 0;
byte streamVariation = 0;
char stepNoteOffChar = 'x';
char regularNoteChar = '.';
char stepSlideChar = '/';
byte maxTrackProcessor = 0;
byte maxPatternNames = 0;
byte maxTrackDrumNames = 0;
byte maxTrackNoteNames = 0;
byte slaveCheckBeatCounter = 0;
char curTrackName[10];
char curPatternName[10];
byte dButtons2[buttonLast];
byte dButtonsEvent[buttonLast];
byte dArea = areaMain;
byte prevArea = dArea;
byte dZone1 = 0; // First Line
byte dZone1Max = 2;
byte dZone2 = 0; // Second Line
byte dZone2Max = 15;
byte dLine = 0;
byte curPattern = 1;
byte nextPattern = curPattern;
int nextPatternCounter = 0;
byte curTrack = 0;
byte curStep = 0;
byte curStepZoneBuffer = 0;
byte lastNote = 60;
byte lastVelocity = 127;
byte initPing = 1;
int  dButtons1[buttonLast];
int  dButtonsCounter[buttonLast];
File tmpFile;

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
byte autoSteps = 1;
boolean seqSyncOut = false;
byte seqBPM = 120;
byte curVariation = 0;
byte totalVariations = 3; // 0~3 //
byte seqType = masterUnit;
byte curPatternNameValue = 0;
int  drumSteps[DOUBLE_STEPS] = {0,0};
byte noteSteps[STEPS*DOUBLE_STEPS];
byte trackProcessor = 0;
byte trackMidiCH = 0;
byte trackNote = 0;
byte noteTrackMode[NOTE_TRACKS];
byte trackName[TOTAL_TRACKS];
byte songLoopPosition = 0;
char songName[17];
byte curSong = 1;

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void setup() 
{
  displayInit();
  SDInit();
  //
  maxTrackProcessor = (SDFileSize(1) / (12+NEW_LINE_SIZE)) - 1;
  maxPatternNames = (SDFileSize(4) / (9+NEW_LINE_SIZE)) - 1;
  maxTrackDrumNames = (SDFileSize(2) / (9+NEW_LINE_SIZE)) - 1;
  maxTrackNoteNames = (SDFileSize(3) / (9+NEW_LINE_SIZE)) - 1;
  //
  memset(noteSteps,0,sizeof(noteSteps));
  memset(trackName,0,sizeof(trackName));
  memset(customChords, 0, sizeof(customChords));
  memset(noteTrackMode, 0, sizeof(noteTrackMode));
  //
  SDReadCustomLCDChars(0);
  slaveInit();
  SDReadConfigurations();
  setupButtons();
  SDGetCurrentSong();
  //
  loadSong();
  displayClear();
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void loop() 
{
  checkButtons();
  checkPlayStopMenu();
  if (currentMode == patternMode) 
  {
    patternInterface(); 
    updateDisplayPattern();
    checkNextPattern();
  }
  else
  {
    songInterface();
    updateDisplaySong();
  }
  streamDataCheck();
  slaveCheckBeat();
  //
  if (lateRedrawDisplay)
  {
    lateRedrawDisplay = false;
    redrawDisplay = reloadScreen = true;
  }
}
