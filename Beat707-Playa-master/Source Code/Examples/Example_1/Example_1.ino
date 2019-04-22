/*
 * 
 * Beat707 Playa
 * Created by William Kalfelz @ Beat707 (c) 2017 - http://www.Beat707.com
 * 
 * In this example I have a setup to play sounds when I power up my computer, put into sleep mode and also a button to power down/up some external lights that I have.
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
//
#define LIGHTS_RELAY 7 // Connected to an external 220V Relay which turns on/off all my desk lights.
#define BUTTON_RELAY 6 // Turns on/off the desk lights using the relay above.
#define IS_SLEEPING 4 // Check if the computer went to sleep, reads the input of the 12V voltage that the ATX unit outputs. I use two resistors to drop the voltage down to 5V.
#define SPEAKERS_ON 3 // Connected to my speakers ground, so when powering up the amp it doesn't do a strange sound it does, every time. This is connected to the last part of the headers.
#define STATUS_LED 2 // Blinks when the computer is in SLEEP mode, stays on when the lights are off but the computer is ON, stays off if everything is on.

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
enum
{
  kAntiPopUp, kAntiPopDown, kPlayingSound, kIdle
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool shutDownEngine = false;
byte currentMode = kIdle;
bool lightsRelay = true;
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
byte soundSequence = 0;
bool isSleeping = false;
byte sleepingLedBlink = 0;

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void setup() 
{
  pinMode(LIGHTS_RELAY, OUTPUT);
  digitalWrite(LIGHTS_RELAY, HIGH);
  pinMode(SPEAKERS_ON, OUTPUT);
  digitalWrite(SPEAKERS_ON, LOW);
  pinMode(9,OUTPUT); // PWM Output
  pinMode(10,OUTPUT); // PWM Output
  pinMode(STATUS_LED, OUTPUT);
  digitalWrite(STATUS_LED, HIGH);
  //
  Serial.begin(115200);
  Serial.println("Starting...");
  //
  pinMode(BUTTON_RELAY, INPUT);
  digitalWrite(BUTTON_RELAY, HIGH);
  pinMode(IS_SLEEPING, INPUT);
  //
  memset(soundOut, 0, sizeof(soundOut));
  flashInit();
  readSoundList();
  digitalWrite(STATUS_LED, LOW);
  delay(500);
  digitalWrite(STATUS_LED, HIGH);
  digitalWrite(SPEAKERS_ON, HIGH);
  delay(1000);
  timerStart();
  //
  if (listOfSounds[0].soundSize > 0)
  {
    if (digitalRead(BUTTON_RELAY) == LOW)
    {
      playSound(2, true);
      while (digitalRead(BUTTON_RELAY) == LOW) { ; }
    }
    else
    {
      playSound(0, true);
      //
      for (byte xx=0; xx<58; xx++)
      {
        digitalWrite(STATUS_LED, LOW);
        delay(200);
        digitalWrite(STATUS_LED, HIGH);
        delay(800);
        Serial.print(".");
      }
      //
      Serial.println(" -> Next Sound");
      playSound(1, true);
    }
  }
  //
  digitalWrite(STATUS_LED, LOW);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void playSound(byte xsound, bool waitUntilFinished)
{
  if (listOfSounds[xsound].soundSize > 0)
  {
    currentMode = kIdle;
    soundPlaying = -1;
    shutDownEngine = false;
    soundPosition = listOfSounds[xsound].soundAddress;
    soundLen = listOfSounds[xsound].soundSize;
    //    
    antiPopOnStart = 0.0f;
    soundOutPos1 = soundOutPos2 = 0;
    readNextSample = false;
    currentMode = kAntiPopUp;
    //
    flash.readAnything(soundPosition, soundOut[0]);
    soundPosition += BUFFER_SIZE * 2;
    soundLen -= BUFFER_SIZE * 2;
    //    
    soundPlaying = xsound;
    Serial.print("Play Sound: ");
    Serial.println(xsound, DEC);   
    //
    if (waitUntilFinished)
    {
      while (currentMode != kIdle)
      {
        tickNextSamnple();
      }
    }
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
  if (digitalRead(BUTTON_RELAY) == LOW)
  {
    delay(20);
    if (digitalRead(BUTTON_RELAY) == LOW)
    {
      if (lightsRelay) 
      {
        digitalWrite(STATUS_LED, HIGH);
        playSound(3, true);
        digitalWrite(STATUS_LED, LOW);
        digitalWrite(LIGHTS_RELAY, LOW);
      }
      else digitalWrite(LIGHTS_RELAY, HIGH);
      lightsRelay = !lightsRelay;
      delay(100);
      while (digitalRead(BUTTON_RELAY) == LOW) { delay(100); }
      if (lightsRelay) 
      {
        digitalWrite(STATUS_LED, HIGH);
        delay(2000);
        digitalWrite(SPEAKERS_ON, HIGH);
        delay(1000);
        playSound(2, false);
        digitalWrite(STATUS_LED, LOW);
      }
      else
      {
        digitalWrite(SPEAKERS_ON, LOW);
        digitalWrite(STATUS_LED, HIGH);
      }
    }
  }
  //
  if (digitalRead(IS_SLEEPING) == LOW)
  {
    if (!isSleeping) 
    {
      isSleeping = true;
      sleepingLedBlink = 0;
      digitalWrite(STATUS_LED, HIGH);
      playSound(3, true);
      digitalWrite(SPEAKERS_ON, LOW);
      digitalWrite(LIGHTS_RELAY, LOW);
      delay(100);
      Serial.println("Went to Sleep");
      digitalWrite(STATUS_LED, LOW);
    }
    else
    {
      sleepingLedBlink++;
      if (sleepingLedBlink == 1)
      {
        digitalWrite(STATUS_LED, HIGH);
      }
      else if (sleepingLedBlink == 4)
      {
        digitalWrite(STATUS_LED, LOW);
      }
      else if (sleepingLedBlink >= 20)
      {
        sleepingLedBlink = 0;
      }
      delay(100);
    }
  }
  else
  {
    if (isSleeping)
    {
      digitalWrite(STATUS_LED, HIGH);
      digitalWrite(LIGHTS_RELAY, HIGH);
      Serial.println("Wake Up!");
      isSleeping = false;
      delay(2000);
      digitalWrite(SPEAKERS_ON, HIGH);
      delay(1000);
      playSound(2, false);
      digitalWrite(STATUS_LED, LOW);
    }
  }
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
