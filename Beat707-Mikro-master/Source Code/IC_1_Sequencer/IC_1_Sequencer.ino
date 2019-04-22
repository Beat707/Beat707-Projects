/*
 * Beat707 Mikro - created by William (Guilherme) Kalfelz (c) 2016 - www.Beat707.com - www.Wusik.com - www.William-k.com
 * 
 * Arduino Board should be based on ATmega328 but with internal 8Mhz Crystal
 * 
 */

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "WWire.h"
#define NOTE_TRACKS 9
#define DRUM_TRACKS 14
#define ACCENT_TRACKS 2
#define STEPS 16
#define VARIATIONS 4
#define DOUBLE_STEPS 2

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
enum
{
  midiNoteOn = 0x90, midiNoteOff = 0x80, midiCC = 0xB0, 
  regularNote = 0, noteOn = 0, noteOff, noteDouble, noteDoubleIn, noteSlide, // The last is when Velocity=0 and Note>0
  masterUnit = 0, slaveUnit,
  noteTrackModeNotes = 0, noteTrackModeCC,
  fadeIn = 1, fadeOut, fadeInOut, fadeOutIn, fadeInD, fadeOutD, randomVel, chordMaj, chordMin, chord57, octave1, octave12, chordCustom,
  accentTrack=DRUM_TRACKS, noteTrack=accentTrack+2, TOTAL_TRACKS=NOTE_TRACKS+DRUM_TRACKS+ACCENT_TRACKS, midiChannels=16,
  //
  slvInit=4, slvInitOk=6, slvGetTrackSettings, slvGetDrumSteps, slvGetNoteSteps1, slvGetNoteSteps2, slvStop, slvPlay, 
    slvSetMirrorMode, slvSetTrackSettings, slvSetSequencerSettings, slvSetDrumSteps, slvSetNoteSteps1, slvSetNoteSteps2, slvSetSingleStep, slvGetBeat,
    slvStartMIDIserial, slvStartUSBserial, slvSetFadeMin, slvClearPattern, slvClearTrack, slvIsPlaying, slvStreamSize, 
    slvSendStream, slvStartStream, slvSetSequencerSettingsBuffer, slvSetTrackSettingsBuffer, slvSetCustomChord, slvTransposeAll, slvSlideStep, 
    slvNextSingleStepForceMirror, slvNextSingleStepNoMirror
};

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
boolean switchSettings = false;
boolean isMirrorMode = false;
boolean nextSingleStepNoMirror = false;
boolean nextSingleStepForceMirror = false;
byte velAccent1 = 32;
byte velAccent2 = 32;
byte velAccent0 = 127-velAccent1-velAccent2;
byte prevTrackNote[TOTAL_TRACKS];
byte prevTrackVelocity[NOTE_TRACKS];
byte streamSize = 0;
byte streamPosition = 0;
byte streamVariation = 0;
byte procFadeMin = 4;
byte wireRequestCode = 0;
byte seqPlaying = 0; // 0=Stop/1=A_Variation/...
byte seqPosition = 0;
byte seqCounter = 0;
byte curTrack = 0;
byte curVariation = 0;
byte tickSequencer = 0;
byte ppq24Counter = 0;

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
boolean seqSyncOut = false;
byte seqBPM = 120;
byte midiClockBeats = 0;
byte playingVariation = 0;
byte seqType = masterUnit;
unsigned int drumSteps[VARIATIONS][DRUM_TRACKS+ACCENT_TRACKS][DOUBLE_STEPS];
byte noteSteps[VARIATIONS][NOTE_TRACKS][STEPS*DOUBLE_STEPS];
byte totalVariations = 3;
byte totalVariationsBuffer = 3;
byte trackNote[DRUM_TRACKS];
byte trackProcessorBuffer[TOTAL_TRACKS];
byte trackMidiCHBuffer[TOTAL_TRACKS];
byte trackNoteBuffer[DRUM_TRACKS];
byte noteTrackModeBuffer[NOTE_TRACKS];
byte customChords[12][5];
byte trackMidiCH[TOTAL_TRACKS]; // 0~15
byte trackProcessor[TOTAL_TRACKS];
byte noteTrackMode[NOTE_TRACKS];

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void setup() 
{
  i2cInit();
  i2cSetLocalDeviceAddr(SLAVE_ID, true);
  i2cSetSlaveReceiveHandler(receiveEvent);
  i2cSetSlaveTransmitHandler(requestEvent);
  //
  memset(customChords, 0, sizeof(customChords));
  //
  clearPattern();
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void clearPattern()
{
  memset(drumSteps, 0, sizeof(drumSteps));
  memset(noteSteps, 0, sizeof(noteSteps));  
  memset(trackProcessor, 0, sizeof(trackProcessor));
  memset(trackNote, 60, sizeof(trackNote));
  memset(trackMidiCH, 0, sizeof(trackMidiCH));
  memset(trackProcessorBuffer, 0, sizeof(trackProcessorBuffer));
  memset(trackNoteBuffer, 60, sizeof(trackNoteBuffer));
  memset(trackMidiCHBuffer, 0, sizeof(trackMidiCHBuffer));
  memset(prevTrackNote, 0, sizeof(prevTrackNote));
  memset(prevTrackVelocity, 0, sizeof(prevTrackVelocity));
  memset(noteTrackMode, 0, sizeof(noteTrackMode));
  memset(noteTrackModeBuffer, 0, sizeof(noteTrackModeBuffer));
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void loop() 
{
  doTickSequencer();
}
