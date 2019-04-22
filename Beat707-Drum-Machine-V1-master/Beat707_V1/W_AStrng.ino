/*

  Created by Beat707 (c) 2011 - http://www.Beat707.com
  
  Strings to be stored as Program (not in RAM, but in Flash)
  
*/

// GM Drum Set //
#if EXTENDED_DRUM_NAMES
  const char string_1[] PROGMEM   = "AcBass";  // 35
  const char string_2[] PROGMEM   = "Bass";    // 36
  const char string_3[] PROGMEM   = "Stick";   // 37
  const char string_4[] PROGMEM   = "Snare";   // 38
  const char string_5[] PROGMEM   = "Clap";    // 39
  const char string_6[] PROGMEM   = "Snare2";  // 40
  const char string_7[] PROGMEM   = "LFlTom";  // 41
  const char string_8[] PROGMEM   = "ClosHat"; // 42
  const char string_9[] PROGMEM   = "HFlTom";  // 43
  const char string_10[] PROGMEM  = "PedlHat"; // 44
  const char string_11[] PROGMEM  = "LowTom";  // 45
  const char string_12[] PROGMEM  = "OpenHat"; // 46
  const char string_13[] PROGMEM  = "LMTom";   // 47
  const char string_14[] PROGMEM  = "HMTom";   // 48
  const char string_15[] PROGMEM  = "Crash";   // 49
  const char string_16[] PROGMEM  = "HiTom";   // 50
  const char string_17[] PROGMEM  = "RideCmb"; // 51
  const char string_18[] PROGMEM  = "Chinese"; // 52
  const char string_19[] PROGMEM  = "RdeBell"; // 53
  const char string_20[] PROGMEM  = "Tmbrine"; // 54
  const char string_21[] PROGMEM  = "Splash";  // 55
  const char string_22[] PROGMEM  = "Cowbell"; // 56
  const char string_23[] PROGMEM  = "Crash2";  // 57
  const char string_24[] PROGMEM  = "VbrSlap"; // 58
  const char string_25[] PROGMEM  = "Ride2";   // 59
  const char string_26[] PROGMEM  = "BongoH";  // 60
  const char string_27[] PROGMEM  = "BongoL";  // 61
  const char string_28[] PROGMEM  = "CongaMt"; // 62
  const char string_29[] PROGMEM  = "CongaOp"; // 63
  const char string_30[] PROGMEM  = "CongaLw"; // 64
  const char string_31[] PROGMEM  = "TmbaleH"; // 65
  const char string_32[] PROGMEM  = "TmbaleL"; // 66
  const char string_33[] PROGMEM  = "AgogoH";  // 67
  const char string_34[] PROGMEM  = "AgogoL";  // 68
  const char string_35[] PROGMEM  = "Cabasa";  // 69
  const char string_36[] PROGMEM  = "Maracas"; // 70
#else
  const char string_2[] PROGMEM   = "Bass";
  const char string_3[] PROGMEM   = "Stick";
  const char string_4[] PROGMEM   = "Snare";
  const char string_5[] PROGMEM   = "Clap";
  const char string_8[] PROGMEM   = "ClosHat";
  const char string_12[] PROGMEM  = "OpenHat";
  const char string_15[] PROGMEM  = "Crash";
#endif

// ======================================================================================= //

const char empty_Str[] PROGMEM  = ""; 
#define EMPTY_STR 0

const char myStrings_001[] PROGMEM  = "ReceivingSysEx";
#define RECEIVING_SYSEX 40
const char myStrings_002[] PROGMEM  = "Shift to Confirm"; 
#define SHIFT_TO_CONFIRM 41
const char myStrings_003[] PROGMEM  = "Pattern"; 
#define PATTERN 42
const char myStrings_004[] PROGMEM  = "Next"; 
#define NEXT 43
const char myStrings_005[] PROGMEM  = "Init Storage"; 
#define INIT_STORAGE 44
const char myStrings_006[] PROGMEM  = "Are you Sure?"; 
#define ARE_YOU_SUREQ 45
const char myStrings_007[] PROGMEM  = "Init Starting..."; 
#define INIT_STARTING 46
const char myStrings_008[] PROGMEM  = "Change Mode?"; 
#define CHANGE_MODE 47
const char myStrings_009[] PROGMEM  = "1/"; 
#define N1_SLASH 48
const char myStrings_010[] PROGMEM  = "16"; 
#define N16 49
const char myStrings_011[] PROGMEM  = "32";
#define N32 50
const char myStrings_012[] PROGMEM  = "64";
#define N64 51
const char myStrings_013[] PROGMEM  = "Steps Mode";
#define STEPS_MODE 52
const char myStrings_014[] PROGMEM  = "Init Pattern?";
#define INIT_PATTERNQ 53
const char myStrings_015[] PROGMEM  = "Paste Pattern?";
#define PASTE_PATTERNQ 54
const char myStrings_016[] PROGMEM  = "Shift Mode";
#define SHIFT_MODE 55
const char myStrings_017[] PROGMEM  = "Track Selection";
#define TRACK_SELECTION 56
const char myStrings_018[] PROGMEM  = "Mute Tracks";
#define MUTE_TRACKS 57
const char myStrings_019[] PROGMEM  = "Solo Tracks";
#define SOLO_TRACKS 58
const char myStrings_020[] PROGMEM  = "Init Pattern";
#define INIT_PATTERN 59
const char myStrings_021[] PROGMEM  = "Copy Pattern";
#define COPY_PATTERN 60
const char myStrings_022[] PROGMEM  = "CopyPtr.Accents";
#define COPY_PTR_ACCENTS 61
const char myStrings_023[] PROGMEM  = "Paste Pattern";
#define PASTE_PATTERN 62
const char myStrings_024[] PROGMEM  = "Pst.MergePattrn";
#define PST_MERGEPATTERN 63
const char myStrings_025[] PROGMEM  = "Patt";
#define PATT 64
const char myStrings_026[] PROGMEM  = "BPM";
#define BPM 65
const char myStrings_027[] PROGMEM  = "Speed Mode";
#define SPEED_MODE 66
const char myStrings_028[] PROGMEM  = "GoTo";
#define GOTO 67
const char myStrings_029[] PROGMEM  = "Accent";
#define ACCENT 68
const char myStrings_030[] PROGMEM  = "MidiChan";
#define MIDICHAN 69
const char myStrings_031[] PROGMEM  = "Pattrn.Mode";
#define PATTRN_MODE 70
const char myStrings_032[] PROGMEM  = "B707";
#define B707 71
const char myStrings_033[] PROGMEM  = "SPos";
#define POSP 72
const char myStrings_034[] PROGMEM  = "End";
#define END 73
const char myStrings_035[] PROGMEM  = "Song Mode";
#define SONG_MODE 74
const char myStrings_036[] PROGMEM  = "Lop";
#define LOP 75
const char myStrings_037[] PROGMEM  = "SLV";
#define SLV 76
const char myStrings_038[] PROGMEM  = "A1";
#define A1 77
const char myStrings_039[] PROGMEM  = "A2";
#define A2 78
const char myStrings_040[] PROGMEM  = "S1";
#define S1 79
const char myStrings_041[] PROGMEM  = "S2";
#define S2 80
const char myStrings_042[] PROGMEM  = "Master";
#define MASTER 81
const char myStrings_043[] PROGMEM  = "Sync Mode";
#define SYNC_MODE 82
const char myStrings_044[] PROGMEM  = "None";
#define NONE 83
const char myStrings_045[] PROGMEM  = "Slave";
#define SLAVE 84
const char myStrings_046[] PROGMEM  = "System Init";
#define SYSTEM_INIT 85
const char myStrings_047[] PROGMEM  = "None";
#define NONE 86
const char myStrings_048[] PROGMEM  = "Note-Off";
#define NOTE_OFF 87
const char myStrings_049[] PROGMEM  = "AutoStepsEdt";
#define AUTOSTEPSEDT 88
const char myStrings_050[] PROGMEM  = "Song Init";
#define SONGINIT 89
const char myStrings_051[] PROGMEM  = "File Mode";
#define FILE_MODE 90
const char myStrings_052[] PROGMEM  = "Song";
#define SONG 91
const char myStrings_053[] PROGMEM  = "Load";
#define LOAD 92
const char myStrings_054[] PROGMEM  = "Save";
#define SAVE 93
const char myStrings_055[] PROGMEM  = "Dump";
#define DUMP 94
const char myStrings_056[] PROGMEM  = "Erase";
#define ERASE 95
const char myStrings_057[] PROGMEM  = "SysEx MIDI ID";
#define SYSEX_MIDI_ID 96
const char myStrings_058[] PROGMEM  = "Processing";
#define PROCESSING 97
const char myStrings_059[] PROGMEM  = "! Empty Song !";
#define E_EMPTY_SONG_E 98
const char myStrings_060[] PROGMEM  = "C C#D D#E F F#G G#A A#B ";
#define NOTENAMESLIST 99
const char myStrings_061[] PROGMEM  = "On";
#define ON_ 100
const char myStrings_062[] PROGMEM  = "Off";
#define OFF_ 101
const char myStrings_063[] PROGMEM  = "Sld";
#define SLD 102
const char myStrings_064[] PROGMEM  = "MirrorEdMode";
#define MIRROREDMODE 103
const char myStrings_065[] PROGMEM  = "ClockShuffle";
#define CLOCKSHUFFLE 104
const char myStrings_066[] PROGMEM  = "Patt.Steps #";
#define N_OF_STEPS 105
const char myStrings_067[] PROGMEM  = "Pattern AB";
#define PATTERN_AB 106
const char myStrings_068[] PROGMEM  = "USB MODE";
#define USB_MODE 107
const char myStrings_069[] PROGMEM  = "USB TO MIDI OK";
#define USB_TO_MIDI_OK 108
const char myStrings_070[] PROGMEM  = "SeqDirection";
#define SEQDIRECTION 109
const char myStrings_071[] PROGMEM  = "Nrm";
#define NRM 110
const char myStrings_072[] PROGMEM  = "Rev";
#define REV 111
const char myStrings_073[] PROGMEM  = "Rnd";
#define RND 112

// ======================================================================================= //

char * const stringlist[] PROGMEM = { empty_Str, 
#if EXTENDED_DRUM_NAMES
  string_1, string_1, string_1, string_1, string_2, string_3, string_4, string_5, string_6, string_7, string_8, string_9, string_10, string_11, string_12, string_13, string_14, string_15, 
  string_16, string_17, string_18, string_19, string_20, string_21, string_22, string_23, string_24, string_25, string_26, string_27, string_28, string_29, string_30, string_31, string_32, string_33, string_34, string_35, string_36, 
#else
  empty_Str, empty_Str, empty_Str, empty_Str, string_2, string_3, string_4, string_5, empty_Str, empty_Str, string_8, empty_Str, empty_Str, empty_Str, string_12, empty_Str, empty_Str, string_15, 
  empty_Str, empty_Str, empty_Str, empty_Str, empty_Str, empty_Str, empty_Str, empty_Str, empty_Str, empty_Str, empty_Str, empty_Str, empty_Str, empty_Str, empty_Str, empty_Str, empty_Str, empty_Str, empty_Str, empty_Str, empty_Str, 
#endif
  myStrings_001, myStrings_002, myStrings_003, myStrings_004, myStrings_005, myStrings_006, myStrings_007, myStrings_008, myStrings_009, myStrings_010, myStrings_011,
  myStrings_012, myStrings_013, myStrings_014, myStrings_015, myStrings_016, myStrings_017, myStrings_018, myStrings_019, myStrings_020, myStrings_021, myStrings_022,
  myStrings_023, myStrings_024, myStrings_025, myStrings_026, myStrings_027, myStrings_028, myStrings_029, myStrings_030, myStrings_031, myStrings_032, myStrings_033,
  myStrings_034, myStrings_035, myStrings_036, myStrings_037, myStrings_038, myStrings_039, myStrings_040, myStrings_041, myStrings_042, myStrings_043, myStrings_044,
  myStrings_045, myStrings_046, myStrings_047, myStrings_048, myStrings_049, myStrings_050, myStrings_051, myStrings_052, myStrings_053, myStrings_054, myStrings_055,
  myStrings_056, myStrings_057, myStrings_058, myStrings_059, myStrings_060, myStrings_061, myStrings_062, myStrings_063, myStrings_064, myStrings_065, myStrings_066, 
  myStrings_067, myStrings_068, myStrings_069, myStrings_070, myStrings_071, myStrings_072, myStrings_073};
