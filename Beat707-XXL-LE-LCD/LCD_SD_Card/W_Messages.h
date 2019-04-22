/*

  Created by Guilherme (William) Kalfelz
  @ Beat707 (c) 2014 - http://www.Beat707.com

*/

// ======================================================================================= //
#define UP_DOWN_REPEATE_RATE 1200
#define LEFT_RIGHT_REPEATE_RATE 14000

// ======================================================================================= //
prog_char myStrings_000[] PROGMEM  = "BPM ";
#define BPM_ 0
//
prog_char myStrings_001[] PROGMEM  = "Stps ";
#define STPS_ 1
//
prog_char myStrings_002[] PROGMEM  = "Patt ";
#define PATT_ 2
//
prog_char myStrings_003[] PROGMEM  = "To ";
#define TO_ 3
//
prog_char myStrings_004[] PROGMEM  = "Mode";
#define MODE 4
//
prog_char myStrings_005[] PROGMEM  = "Song ";
#define SONG_ 5
//
prog_char myStrings_006[] PROGMEM  = "Pattern ";
#define PATTERN_ 6
//
prog_char myStrings_007[] PROGMEM  = "Processing...   ";
#define PROCESSING 7
//
prog_char myStrings_008[] PROGMEM  = "Pos ";
#define POS_ 8
//
prog_char myStrings_009[] PROGMEM  = "Note ";
#define NOTE_ 9
//
prog_char myStrings_010[] PROGMEM  = "Type ";
#define TYPE_ 10
//
prog_char myStrings_011[] PROGMEM  = "---";
#define ___ 11
//
prog_char myStrings_012[] PROGMEM  = "Val  ";
#define VAL__ 12
//
prog_char myStrings_013[] PROGMEM  = "Empty";
#define EMPTY 13
//
prog_char myStrings_014[] PROGMEM  = "Note ";
#define NOTE_ 14
//
prog_char myStrings_015[] PROGMEM  = "Gate";
#define GATE 15
//
prog_char myStrings_016[] PROGMEM  = "Slide";
#define SLIDE 16
//
prog_char myStrings_017[] PROGMEM  = "Flam";
#define FLAM 17
//
prog_char myStrings_018[] PROGMEM  = "M.CC ";
#define M_CC_ 18
//
prog_char myStrings_019[] PROGMEM  = " C#C D#D E F#F G#G A#A B";
#define NOTENAMESLIST 19
//
prog_char myStrings_020[] PROGMEM  = "Transpose  Notes";
#define TRANSPOSE__NOTES 20
//
prog_char myStrings_021[] PROGMEM  = "ON";
#define ON 21
//
prog_char myStrings_022[] PROGMEM  = "Off";
#define OFF 22
//
prog_char myStrings_023[] PROGMEM  = "PT";
#define PT 23
//
prog_char myStrings_024[] PROGMEM  = "Cancel";
#define CANCEL 24
//
prog_char myStrings_025[] PROGMEM  = "Confirm";
#define CONFIRM 25
//
prog_char myStrings_026[] PROGMEM  = "Sel. Step Below";
#define OPTIONS_MESSAGE 26
//
prog_char myStrings_027[] PROGMEM  = "To Song Mode";
#define TO_SONG_MODE 27
//
prog_char myStrings_028[] PROGMEM  = "Copy Pattern";
#define COPY_PATTERN 28
//
prog_char myStrings_029[] PROGMEM  = "Paste Pattern";
#define PASTE_PATTERN 29
//
prog_char myStrings_030[] PROGMEM  = "Copy To Next Pat";
#define COPY_TO_NEXT_PAT 30
//
prog_char myStrings_031[] PROGMEM  = "Init Pattern";
#define INIT_PATTERN 31
//
prog_char myStrings_032[] PROGMEM  = "Init All Data";
#define INIT_ALL_DATA 32
//
prog_char myStrings_033[] PROGMEM  = "Init Cur. Track";
#define INIT_CUR_TRACK 33
//
prog_char myStrings_034[] PROGMEM  = "Beat707.com V1.0";
#define BEAT707_COM_V 34
//
prog_char myStrings_035[] PROGMEM  = "Rp ";
#define RP_ 35
//
prog_char myStrings_036[] PROGMEM  = "Sp ";
#define SP_ 36
//
prog_char myStrings_037[] PROGMEM  = "A    ";
#define A____ 37
//
prog_char myStrings_038[] PROGMEM  = "AB   ";
#define AB___ 38
//
prog_char myStrings_039[] PROGMEM  = "ABC  ";
#define ABC__ 39
//
prog_char myStrings_040[] PROGMEM  = "ABCD ";
#define ABCD_ 40
//
prog_char myStrings_041[] PROGMEM  = "--";
#define __ 41
//
prog_char myStrings_042[] PROGMEM  = "FI";
#define FI 42
//
prog_char myStrings_043[] PROGMEM  = "FO";
#define FO 43
//
prog_char myStrings_044[] PROGMEM  = "To Song Perform.";
#define TO_SONG_PERFORM 44
//
prog_char myStrings_045[] PROGMEM  = "To Pattern Mode";
#define TO_PATTERN_MODE 45
//
prog_char myStrings_046[] PROGMEM  = "  CD ";
#define __CD_ 46
//
prog_char myStrings_047[] PROGMEM  = "   D ";
#define ___D_ 47
//
prog_char myStrings_048[] PROGMEM  = "Save All Data";
#define SAVE_ALL_DATA 48
//
prog_char myStrings_049[] PROGMEM  = "Init. Save Data!";
#define INIT_SAVE_DATA_ 49
//
prog_char myStrings_050[] PROGMEM  = "Click to Confirm";
#define CLICK_TO_CONFIRM 50
//
prog_char myStrings_051[] PROGMEM  = "On";
#define _ON_ 51
//
prog_char myStrings_052[] PROGMEM  = "Loop Song";
#define LOOP_SONG 52
//
prog_char myStrings_053[] PROGMEM  = "Insert Position";
#define INSERT_POSITION 53
//
prog_char myStrings_054[] PROGMEM  = "Remove Position";
#define REMOVE_POSITION 54
//
prog_char myStrings_055[] PROGMEM  = "Init Song Data";
#define INIT_SONG_DATA 55
//
prog_char myStrings_056[] PROGMEM  = "Are you Sure?";
#define ARE_YOU_SURE 56
//
prog_char myStrings_057[] PROGMEM  = "Normal ";
#define NORMAL_ 57
//
prog_char myStrings_058[] PROGMEM  = "Stack  ";
#define STACK__ 58
//
prog_char myStrings_059[] PROGMEM  = "Start ";
#define START_ 59
//
prog_char myStrings_060[] PROGMEM  = "End ";
#define END_ 60
//
prog_char myStrings_061[] PROGMEM  = "**";
#define _AA_ 61
//
// ======================================================================================= //
PROGMEM const char *stringlist[] = {
    myStrings_000, myStrings_001, myStrings_002, myStrings_003, myStrings_004, myStrings_005, myStrings_006, myStrings_007, myStrings_008, myStrings_009, myStrings_010, 
    myStrings_011, myStrings_012, myStrings_013, myStrings_014, myStrings_015, myStrings_016, myStrings_017, myStrings_018, myStrings_019, myStrings_020, myStrings_021, 
    myStrings_022, myStrings_023, myStrings_024, myStrings_025, myStrings_026, myStrings_027, myStrings_028, myStrings_029, myStrings_030, myStrings_031, myStrings_032,
    myStrings_033, myStrings_034, myStrings_035, myStrings_036, myStrings_037, myStrings_038, myStrings_039, myStrings_040, myStrings_041, myStrings_042, myStrings_043,
    myStrings_044, myStrings_045, myStrings_046, myStrings_047, myStrings_048, myStrings_049, myStrings_050, myStrings_051, myStrings_052, myStrings_053, myStrings_054,
    myStrings_055, myStrings_056, myStrings_057, myStrings_058, myStrings_059, myStrings_060, myStrings_061
  };
