/*

  Created by Guilherme (William) Kalfelz
  @ Beat707 (c) 2014 - http://www.Beat707.com
  
  Configuration File
  
*/

// ======================================================================================= //
#define STEPS_ABCD 4 // number of multi steps (1 to 4) //
#define USE_GM_NOTES 1 // If set to 0 it will use note 60, 61, 62, 63, 64, ... //
#define IS_LAST_UNIT 1 // Set to 0 if you have multiple units side-chainned and this is not the last one //
#define IS_MASTER_UNIT 1 // Set to 0 if this is a Slave Unit //
#define HAS_SLAVE_UNITS 0 // Set to 1 if this is a Master Unit and you have Slave Units on the Chain //
#define BLINK_TRANSPORT_MODE 2 // 0 = Normal, 1 = Beats only, 2 = Off //

// ======================================================================================= //
#define DRUMTRACKS 8 // + 2 accent tracks //
#define PATTERNS 6
#define ACCENT1 DRUMTRACKS
#define ACCENT2 DRUMTRACKS+1
#define INIT_EEPROM 0 // This will clear the EEPROM data //
#define TRANSPORT_BLINK_RATE 1500
#define EXTRA_LED_BLINK_RATE 1800
#define UP_DOWN_REPEATE_RATE 3200
#define MSerial Serial
