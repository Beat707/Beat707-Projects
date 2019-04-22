# Beat707 Mikro & Vox
### Open Source Arduino Compatible MIDI Sequencer / Groove Box & Sample Player

created by William (Guilherme) Kalfelz (c) 2017

www.Beat707.com - www.Wusik.com - www.Kalfelz.com

--------------------------------------------------------------------------------

MIDI Groove Sequencer, Arduino Based
------------------------------------

- The hardware uses two Arduino ATmega328 compatible chips for parallel processing. One takes care of the SD Card and Interface while the other only Sequencing.
- 16 x 2 Steps (Double Steps) and 4 Pattern Variations (ABCD) for a total of 128 Steps per pattern on both Drum and Note Tracks, 99 patterns total.
- 14 Drum Tracks, 2 Accent Tracks.
- 09 Note/CC Tracks - Note# and Velocity per Step or MIDI CC (Controller) Type and Value.
- Each step on both modes have: Off, Regular Note On, Double Note, Double Note In. On Note Tracks you also have Slide and Note Off.
- MIDI Processor Per Track. EG: Fade In, Fade Out, Chords. Including a special Custom Chord creator, which allows you to create 12 different 5 key chords. Those will not follow the played key, but only the played octave. So you can add multiple C, D or whatever you want to different keys in an octave and have those play according to the played octave.
- 99 Slots Song, with Pattern# and Repeat# per Slot.
- 99 Songs Space with Song Name support.
- Pattern and Song Modes.
- Mirror mode for editing all the 4 Pattern Variations at once.
- SD Card Support (required, as it stores program related data).
- All SD Data is stored in .dat files (windows readable format).
- LCD 16x2 Interface.
- We have released a board 2.0 which features some extra options for future updates. Such as support for the Beat707 Vox (sample player) code. The new board has Version 2.0 wrote in it. In the code, check the Definitions.h option named "#define V2_BOARD" set to 0 or 1 depending to the board you are using.

--------------------------------------------------------------------------------

Beat Vox, Sample Player
-----------------------

- 6 Voices with Stereo Flip-Flop effect
- 6 Levels of Velocity
- Realtime Pitch Option (*)
- Realtime Reverse Option (*)
- WAV2Code Converters in C++ (Windows) and Python (Cross-Platform)
- 11 Drum and FX Samples in WAV format included
- Uses the same board (V2) as Mikro. Uses ATmega328 IC #1 only.
- Audio is on Vox Audio Header. Pins 1 is GND, 2 and 3 is Audio. 4 is not used yet.
- Realtime controllers are on Vox Cntrl Header. Pin 1 is GND, 2 is A0 (Analog Input for a Pot), 3 is D8 for a button and 4 is VCC (5+).
- (*) To enable such features, set the option USE_EXTERNAL_CTRLS in the code. Check BUTTON_PIN and ANALOG_PIN for which pins the code uses, or change to your own settings.

--------------------------------------------------------------------------------

Mikro Key Shortcuts
-------------------

- When in Menu Display: Shift is Ok and Menu is Cancel
- Shift + Left/Right: Change Variation ABCD
- Shift + PageUp/PageDown: Change Track
- Shift + ValueUp/ValueDown: In the Steps editor, will change the value and jump steps according to "autoSteps" setting. In Note Step it will change the octave up/down.
- Shift + Menu: Save Pattern and Song Settings (if changes were made). It also checks for anything to be saved when Play or Stop is pressed. And on Play it starts the sequence and them saves in a background thread.
- Menu + PageUp/Down: In Note Track Step Editor will change the Step position, anywhere else will transpose all Note Tracks up and down.
- Menu + Left/Right: When editing Steps will Slide (move) the current step Left or Right.
- Menu + ValueUp/Down: In the Steps editor will edit a step without mirroring, if mirror mode is enabled, or force mirroring if mirror mode is disabled.
- Song Name Edit: Shift+ValueUp/Down cycles Space, A, a and 0.

--------------------------------------------------------------------------------

Board ERRATA
------------

- Pins 4 and 5 on the MIDI Input are inverted. You need to use a twisted MIDI Cable or cut traces on the PCB and re-route the pins.
- Next to the 4N35 IC there are two resistors: 220 and 1K. Do not solder the 220 one. Otherwise MIDI input won't work.

--------------------------------------------------------------------------------

V2 Board Main Headers
---------------------

```
Pin		AVR		Arduino		Port		Button
--------------------------------------------------------------------
01		13		D7		PD7		Stop
02		12		D6		PD6		ValueDown
03		11		D5		PD5		ValueUp
04		2IC		A5		SCL		--
05		2IC		A4		SDA		--
06		06		D4		PD4		Left
07		05		D3		PD3		Down
08		04		D2		PD2		Up
09		GND		--		--		--
10		VCC		--		--		--
11		VCC		--		--		--
12		23		A0		PC0		Shift
13		24		A1		PC1		Right
14		25		A2		PC2		Rec
15		26		A3		PC3		Menu
16		14		D8		PB0		Play
```

--------------------------------------------------------------------------------

V1 Board Main Headers
---------------------

```
Pin		AVR		Arduino		Port		Button
--------------------------------------------------------------------
01		13		D7		PD7		Stop
02		12		D6		PD6		ValueDown
03		11		D5		PD5		ValueUp
04		10		--		PB7		Shift
05		09		--		PB6		Right
06		06		D4		PD4		Left
07		05		D3		PD3		Down
08		04		D2		PD2		Up
09		GND		--		--		--
10		VCC		--		--		--
11		15		D9		PB1		--
12		23		A0		PC0		--
13		24		A1		PC1		--
14		25		A2		PC2		Rec
15		26		A3		PC3		Menu
16		14		D8		PB0		Play
```

--------------------------------------------------------------------------------

Looking at the PCB, with the SD conection on TOP and MIDI+USB on the LEFT, the Main Headers pins should read

```
+----------___----------+
|08 07 06 05 04 03 02 01|
|09 10 11 12 13 14 15 16|
+-----------------------+
```

For the SD Card we use the one found on the picture: Docs/SD Arduino Board.jpg

--------------------------------------------------------------------------------

SD Card Settings File (ss.dat)
------------------------------

- Midi Over USB? - When set to 001 it will use Serial.begin(38400) so some Serial to MIDI Application run on a computer can translate data. Otherwise it will use Serial.begin(31250).
- Fade InOut Min - The lowest midi velocity value to be used in the Fade Processes.
- Buttons Click Counter - In the button's code, when a button is pressed a counter is added 1 until it reaches over this value and a click is to be detected once the value falls to zero using the decay value below: Buttons Decay.
- Buttons Hold Counter - As above, the threshold where a hold will be detected.
- Buttons Repeat Counter - As above, the delay between button repeats.
- Buttons Decay - When a button is released the button counter variable is subtracted by this amount.
- Buttons Shift Counter - The Shift button can have a different click threshold value.
- Loading Counter - Hold long it takes to load a new selected pattern.
- Step Note Off - Step Off character when in a Note Track. Here you can put a regular character like this 'A' or use a special value from 0 to 255. See the examples on the file itself to understand the required format.
- Regular Note - Same as above, for the Regular Notes.
- Step Slide - Same as above, for Slide Notes.
- Loading Icon - Here you can set what shows in the Variation ABCD area when something is been loaded.
- Song Mode Default? - Should the Song Mode be the default mode when the unit is powered up?
- Blink on Beat? - When playing it will blink the cursor and also ABCD playing variation next to the Variation selector (only on the main page).

--------------------------------------------------------------------------------

Pattern Mode Areas
------------------

```
1) Main Area
 01x N1 xxxxxxxxx
 ................
 ----------------

 01 = Pattern Number
 x = Variation ABCD or the Loading Icon when a new pattern is been loaded or is waiting for the "Loading Counter" threshold.
 N1 = Track Number: 01~14 Drum Tracks (regular On/Off steps) / A1~A2 Accent Tracks / N1~N9 Note Tracks (each step has a Velocity and Note Number)
 xxxxxxxxx = Track Name: HiHat, Bass, ...
 The last line are the 16 Steps for the selected Variation ABCD.

 * When in a Note Track you can push PageDown again to see a Step Editor as shown below.
```

```
2) Note Track Step Editor
 01x N1 xxxxxxxxx
 Step01  127  .5C#
 -----------------

 Step01 = Step 01~16 Selector.
 127 = Velocity.
 . = Regular Note, Double Note, Double In Note, Slide or Note Off selector.
 5C# = The Note Octave and Name, from 000~127.
```

```
3) BPM and SyncOut
 Master   120 BPM
 MidiSyncOut  Off
 ----------------

 Master = Master or Slave mode, if it should use its internal timer or an external MIDI timer.
 120 BPM = The tempo in BPM (Beats Per Minutes).
 MidiSyncOut = If it should send MIDI Timer Information for other devices to sync.
```

```
4) Track Types
 T01    MidiCh 01
 N# 060 Kick
 ----------------

 T01 = Track Number.
 MidiCh = Midi Channel for the selected track.
 N# = Note Number for the selected track.
 Kick = The name of this Track.

 * Accent Tracks will show a blank page.
 * Note Tracks shows the Mode instead of the N#. Mode can be Notes (regular notes) or CC (controller values, such as volume, mod wheel, ...).
```

```
5) Track Processor
 T01  Fade In
 -----------------

 T01 = Track Number.
 Fade In = The type of processor: Fade In, Out, InOut, ...
```

```
6) Auto Steps and Pattern Type
 Auto Step     01
 PtType Intro
 ----------------

 Auto Step = When you press Shift + ValueUp/Down it will go to the next step, when editing steps, and skip the steps set in this value.
 PtType = Pattern Type.
```

```
7) Variations, where you can select if this track should use all ABCD variations or just the ones selected.
```

```
8) Song Selector and Song Name Editor.
```

--------------------------------------------------------------------------------

Menus in Pattern Mode
---------------------

- Copy, Paste and Clear: Current Pattern or just the Current Track.
- Add New Pattern and Go To Song Mode.
- Custom Chords. From notes 01 to 12 (starting on the C note and up) and in the last line the 5 keys for the selected chord. The played note will them selected a chord from the 01~12 list and use its key octave to play the chord and will ignore the played base key.

--------------------------------------------------------------------------------
