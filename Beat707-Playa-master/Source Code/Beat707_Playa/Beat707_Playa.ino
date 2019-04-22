/*
 * 
 * Beat707 Playa
 * Created by William Kalfelz @ Beat707 (c) 2017 - http://www.Beat707.com
 * 
 */

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define SS 8
#include <SPIFlash.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <EEPROM.h>

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
struct SOUND
{
  SOUND::SOUND() { init(); }
  void init() { soundAddress = soundSize = 0; }
  uint32_t soundAddress;
  uint32_t soundSize;
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define MAX_SOUNDS 4
#define SAMPLE_RATE 22038 // For reference only.
#define BUFFER_SIZE 32
#define HIGH_SPEED_SERIAL 1 // 0 = 9600 - 1 = 115200 (usually you use 1 for programming the flash, once done, you can use 0 for better coms
#define AUTO_PLAY 1
#define AUTO_PLAY_SND 0
#define USE_AUTO_ENGINE 1 // when set to 1 it will fade in from 0 to 363, play the sound, fade out from 363 to 0 and stop the timer

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
enum
{
  kAntiPopUp, kAntiPopDown, kPlayingSound, kIdle
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool shutDownEngine = false;
byte currentMode = kIdle;
SPIFlash flash;
SOUND listOfSounds[MAX_SOUNDS];
uint16_t soundOut[2][BUFFER_SIZE];
byte soundOutPos1 = 0;
byte soundOutPos2 = 0;
uint32_t soundPosition = 0;
float antiPopOnStart = 0;
float antiPopOnStartRate = 363.0f / (float(SAMPLE_RATE) * 2.0f);
float antiPopEnd = 363.0f;
bool doAntiPopEnd = false;
int32_t soundLen = 0;
char soundPlaying = -1;
bool readNextSample = false;
bool playLooped = false;

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void setup() 
{
  pinMode(9,OUTPUT); // PWM Output
  pinMode(10,OUTPUT); // PWM Output
  //
  #if HIGH_SPEED_SERIAL
    Serial.begin(115200);
  #else
    Serial.begin(9600);
  #endif
  Serial.println("Starting...");
  //
  memset(soundOut, 363, sizeof(soundOut));
  flashInit();
  readSoundList();
  //
  printMenu();
  //
  #if !USE_AUTO_ENGINE
    antiPopOnStart = 0.0f;
    currentMode = kAntiPopUp;
  #endif
  //
  #if AUTO_PLAY
    playSound(AUTO_PLAY_SND);
  #endif
  //
  timerStart();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void playSound(byte xsound)
{
  if (listOfSounds[xsound].soundSize > 0)
  {
    Serial.print("Play Sound: ");
    Serial.println(xsound, DEC);   
    //    
    soundPlaying = -1;
    delay(10);
    shutDownEngine = false;
    soundPosition = listOfSounds[xsound].soundAddress;
    soundLen = listOfSounds[xsound].soundSize;
    antiPopOnStart = 0.0f;
    soundOutPos1 = soundOutPos2 = 0;
    readNextSample = false;
    #if USE_AUTO_ENGINE
      currentMode = kAntiPopUp;
    #else
      currentMode = kPlayingSound;
    #endif
    //
    flash.readAnything(soundPosition, soundOut[0]);
    soundPosition += BUFFER_SIZE * 2;
    soundLen -= BUFFER_SIZE * 2;
    //
    soundPlaying = xsound;
  }
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void tickNextSamnple()
{
  if (readNextSample)
  {
    if (soundPlaying != -1)
    {
      byte xPos = 0;
      if (soundOutPos2 == 0) xPos = 1;
      flash.readAnything(soundPosition, soundOut[xPos]);
      soundPosition += BUFFER_SIZE * 2;
    }
    readNextSample = false;
  } 
  //
  if (shutDownEngine)
  {
    shutDownEngine = false;
    Serial.println("Engine Stop");
  }  
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void loop()
{
  tickNextSamnple();
  //
  if (Serial.available() > 0)
  {
    byte code = Serial.read();
    if (code == 'P')
    {
      while (Serial.available() == 0) { ; }
      code = Serial.read();
      //
      switch (code - '1' + 1)
      {
        case 4:
          currentMode = kIdle;
          timerStop();
          chipErase();
          timerStart();
        break;
        //
        case 2:
          listSounds();
        break;
        //
        case 3:
          currentMode = kIdle;
          timerStop();
          Serial.println("This appends a new sound to the end of the list.");
          Serial.println("Data Format: 3 x 0~F HEX - min is 000 and max is 2D6 - To end transmission send XXX - All UPPERCASE - First 6 bytes is the number of samples");
          receiveData();
          timerStart();
        break;
      }
    }
    else if (code == 'S')
    {
      shutDownEngine = true;
      soundPlaying = -1;
      playLooped = false;
      #if USE_AUTO_ENGINE
        antiPopOnStart = 363.0f;
        currentMode = kAntiPopDown;
      #endif
    }
    else if (code == 'L' || code == 'N')
    {
      playLooped = false;
      if (code == 'L') playLooped = true;
      //
      while (Serial.available() == 0) { ; }
      code = Serial.read();
      playSound(code - '0');
    }
    //
    printMenu();
  }
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void chipErase()
{
  Serial.print("Erasing Flash... ");
  if (flash.eraseChip()) Serial.println("Ok"); else Serial.println("Error");
  flash.eraseSector(0);
  Serial.print("Erasing EEPROM... ");
  for (byte x=0; x<MAX_SOUNDS; x++) { listOfSounds[x].init(); }
  writeSoundList();
  Serial.println(" Ok");  
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void printMenu()
{
  Serial.println("Options: (P4) cleans up the memory - (P3) setup new sound - (P2) list sounds - (N0~9) Plays the Sound (non looped) - (L0~9) Loops The Sound - (S) Stops the Sound");
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void readSoundList()
{
  EEPROM.get(0, listOfSounds);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void writeSoundList()
{
  EEPROM.put(0, listOfSounds);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void listSounds()
{
  uint32_t totalSize = 0;
  Serial.println("List of Sounds");
  for (byte x=0; x<MAX_SOUNDS; x++) 
  { 
    Serial.print("S");
    Serial.print(x, DEC);
    if (listOfSounds[x].soundSize == 0)
    {
      Serial.println(" - Empty");
    }
    else
    {
      Serial.print(" - Address: ");
      Serial.print(listOfSounds[x].soundAddress, DEC);
      Serial.print(" - Size: ");
      Serial.println(listOfSounds[x].soundSize, DEC);
      totalSize += listOfSounds[x].soundSize;
    }
  }
  //
  if (totalSize > 0)
  {
    Serial.print("Total Size: ");
    Serial.print(totalSize, DEC);
    Serial.print(" bytes - ");
    Serial.print(float(totalSize/1024.0f)/1024.0f);
    Serial.print(" megabytes / Space Left: ");
    Serial.print((8*1024*1024) - totalSize, DEC);
    Serial.print(" bytes - ");
    Serial.print(8.0f - float(totalSize/1024.0f)/1024.0f);   
    Serial.println(" megabytes");
  }
}

