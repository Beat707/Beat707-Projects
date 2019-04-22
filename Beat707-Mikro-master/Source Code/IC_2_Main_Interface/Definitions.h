/*
 * Beat707 Mikro - created by William (Guilherme) Kalfelz (c) 2016 - www.Beat707.com - www.Wusik.com - www.William-k.com
 * 
 */

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define V2_BOARD 1
#define NOTE_TRACKS 9
#define DRUM_TRACKS 14
#define ACCENT_TRACKS 2
#define STEPS 16
#define VARIATIONS 4
#define DOUBLE_STEPS 2
#define SDCARDSS 10
#define BLOCKING_ERROR_MSGS 1

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define LCD_I2C_MODE_ADDRESS 0x27
#define SLAVE_ID 37

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include <SdFat.h>
#include <Wire.h>
#include "LCD_Lib.h"
SdFat SD;

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define NEW_LINE_SIZE 2
#define SD_CARD_ERROR 0
#define SD_CARD_ERRORSC 1
#define WIRE_ERROR 2
#define STEP_OFF 0
#define STEP_ON 1
#define STEP_DOUBLE 2
#define STEP_DOUBLE_IN 3
#define LCD_VARIATION 4
#define MAX_BPM 250
#define MIN_BPM 50
#define SD_PATTERN_VARIATION_SIZE (2+(((DRUM_TRACKS+ACCENT_TRACKS)*(STEPS+1))+NEW_LINE_SIZE+(NOTE_TRACKS*((7*STEPS)+NEW_LINE_SIZE))+NEW_LINE_SIZE))
#if BLOCKING_ERROR_MSGS
  #define ERROR_HALT_CODE while (1) { ; }
#else
  #define ERROR_HALT_CODE unsigned int xc = 0; while (xc < 60000) { xc++; }
#endif
#define CHECK_BEAT_COUNTER 24
#define CHECK_STREAM_COUNTER 12

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
LiquidCrystal_I2C lcd(LCD_I2C_MODE_ADDRESS,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void changeTrack(char val, boolean checkShift = false);

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define fW(a) tmpFile.write(a)
#define fR() tmpFile.read()
char fileG[]  = "/B707mk/00/xxx.dat";
char fileGT[] = "/B707mk/00/xxx.tmp";
char fileS[]  = "/B707mk/s0.dat";

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define SECOND_LINE(value) (value | 0x10)
