# Beat707 NXT
### Open Source Arduino Compatible MIDI Sequencer

created by William (Guilherme) Kalfelz (c) 2018
Creative Development by Gert Borovčak

www.Beat707.com - www.Wusik.com - www.Kalfelz.com

--------------------------------------------------------------------------------

MIDI Groove Sequencer
---------------------

- 15 Drum Tracks (note on/off, 3 velocity values).
- 1 Accent Track. But each step can be set: Accent Track, Min or Max.
- 8 Note Tracks (note number, 3 velocity values). Steps have the option of Slide, Double and Note Off.
- 512 Patterns.
- 16 Steps on screen, plus ABCD variations for a total of 64 steps.
- Double Steps (in between steps option when editing the steps). Steps are 1/16 and Double-Steps are two 1/32.
- MIDI Processor Per Track. EG: Fade In, Fade Out, Fade InOut, Fade OutInt, Random Velocity and more.
- MIDI Echo/Delay option with the following options: track selector, number of 24 PPQ ticks, space in 24 PPQ ticks, Attach or Decay Velocity and the option to select when the echo happens: on all notes, max or lower forced velocity notes.
- Mute and Solo mode for live performance with pattern selection (settings are saved per song, not pattern).
- Pattern Chain and Repeat options.
- Simple Hardware design, based on the easy to find TM1638 IC based boards. Just search on eBay and you should be able to find several options. We used 3 boards that has 8 x buttons, 8 x LEDs and 8 x LED_Segments. The board uses a single ATmega328 running at 3.3V, one FLASH chip for storage and connections for the TM1638 boards.
- MIDI Input and Output, with Clock Send and Receive (Slave mode too).

--------------------------------------------------------------------------------

How The Software Works
----------------------

- The first TM1638 board is the main interface. Where it shows the Tempo in BPM and the current Pattern. The top leds, first 4 leds show the beat position and the last 4 leds shows the ABCD pattern variation. Also, beats will blink to bottom dot of the 4th digit from left to right.
- Still in the first board, the first button is Play, second button is Stop. 3rd is - and 4th is +. 5th, when hold, let you force a step to Max, Min or Accent. 6th button, if you press and release, will alternate mirror mode, where all ABCD variations will be edited at once, or not, when disabled. You can also hold it and select the first 4 buttons on the second board to select ABCD variations directly. The 7th button, if you press and release, will go to the next pattern. If you hold, it will show the current selected pattern on the two other boards LEDs. The final button, the 8th one, is for selecting the track been edited. If you select a track twice, it will go to the Note Track, instead of Drum Track.
- Holding button #5 from the first board and selecting the button #7, will open the pattern menu: copy, paste, init. Or selecting button #8 will open the global settings menu.
- Adding steps is easy, just click to turn On or Off. By default when you turn a step On it will use the Accent track (last track). Or you can hold button #5 from the left board and turn a step On: Max, Min and Accent. Holding a step will turn the Double-Step On/Off. The Double-Step uses the same velocity as the regular step it is on. Double-Steps are 1/32 steps.
- On Note Tracks, when editing the steps (not editing a single step) you can hold one (or multiple) step(s) and use the +/- buttons to raise or lower the step(s) pitch. When you hold a step the BPM display on the left will change to P000 and show if you are adding or subtracting the pitch and go back to regular BPM display when you release the buttons.  While holding a step you can also press the track selector (button #8 from the left) to toggle between selected step(s) or ALL steps.
- The Slide option for the notes work like this: regular notes will turn the previous note Off before turning the new step note On. When Slide is activated for the step, the new Step note On will come Before the previous Step Note Off. So if the target MIDI device supports Glide/Slide it will do just that.

--------------------------------------------------------------------------------

Hold Button #5 and Press Button #8, from the first board on the left.

```
  1 2 3 4 5 6 7 8 
  ---------------
          +_____+
             Global Options Menu.
```

--------------------------------------------------------------------------------

Global Options Menu
-------------------
- Button #7 is Previous Item and Button #8 is Next Item. 
- Buttons #3 and #4 are the usual - and +.
- Step buttons will select which track you are editing parameters for (when applicable).
- To exit click button #5 again.

First Board (from the left).

```
  1 2 3 4 5 6 7 8 
  ---------------
      - + E   < >
          x
          i
          t
```

Options
-------

- MIDI Channel
- MIDI Note Number (for Drum Tracks)
- * Enable Recording for this Track (* can be disabled)
- MIDI Accent Value 1
- MIDI Accent Value 2
- MIDI Accent Value 3
- MIDI Processor for the current track
- ECHO Processor (6 options)
- Pattern Variations: ABCD (each variation has 16 steps)
- MIDI Sync Out: On/Off
- MIDI Input Channel to Pattern Selection
- Sequencer Clock Type: Internal or External
- Current Song
- Initialize: None, Current Song or ALL (WARNING!) - hit any STEP key to start
- * To Song Mode - hit + or - to go (* this option is disabled by default)

--------------------------------------------------------------------------------

Hold Button #5 and Press Button #7, from the first board on the left.

```
  1 2 3 4 5 6 7 8
  ---------------  
          +___+
            Pattern Menu: Copy, Paste and Init.
```

--------------------------------------------------------------------------------

Pattern Menu
------------
- Just click the button next to the option you want: Copy, Paste and Init. Or hit the button #5 again to exit.

--------------------------------------------------------------------------------

Hold Button #5 and Press Button #6, from the first board on the left.

```
  1 2 3 4 5 6 7 8
  ---------------  
          +_+
           Solo Mute Mode
```

--------------------------------------------------------------------------------

Solo Mute Interface Overview 
----------------------------

```
  1 2 3 4 5 6 7 8    1 2 3 4 5 6 7 8  1 2 3 4 5 6 7 8
  ---------------    ---------------  ---------------
  { Note Tracks }    {         Drum Tracks         }Click Goes to Pattern Menu
```

- Press buttons to Mute/UnMute and Hold for Solo. The last button on the right goes to Pattern Menu (see below).

--------------------------------------------------------------------------------

Solo Mute Pattern Menu
----------------------

```
  1 2 3 4 5 6 7 8    1 2 3 4 5 6 7 8  1 2 3 4 5 6 7 8
  ---------------    ---------------  ---------------
  { Pattern Bank}    {Pattern Numbr}  P   N   Close B
                                      r   e         a
                                      e   x         c
                                      v   t         k (goes back to the Solo Mute Interface above).
```

--------------------------------------------------------------------------------

Interface Overview (Drum Tracks - hold a step for Double-Step)
--------------------------------------------------------------

```
  1 2 3 4 5 6 7 8    1 2 3 4 5 6 7 8  1 2 3 4 5 6 7 8
  ---------------    ---------------  ---------------
  P S - + M A P T    S S S S S S S S  S S S S S S S S
  l t     e B a r    t t t t t t t t  t t t t t t t t
  a o     n C t a    e e e e e e e e  e e e e e e e e
  y p     u D t c    p p p p p p p p  p p p p p p p p
              e k  
          A V r      0 0 0 0 0 0 0 0  0 1 1 1 1 1 1 1
          c a n      1 2 3 4 5 6 7 8  9 0 1 2 3 4 5 6
          c r
          e i
          n a
          t t
            i
            o
            n
            s
```

--------------------------------------------------------------------------------

Interface Overview (Note Tracks - to enter this mode, just hold a step for one second and release)
Holding a step and clicking +/- transposes the selected step, or click Bt#8 (Track) to enable/disable Transpose-All-Steps.
-----------------------------------------------------------------------

```
  1 2 3 4 5 6 7 8    1 2 3 4 5 6 7 8  1 2 3 4 5 6 7 8
  ---------------    ---------------  ---------------
  P S - + M A P T    C C D D E F F G  G A A B S V O C
  l t     e B a r      #   #     #    #   #   l e c l
  a o     n C t a                             i l t o
  y p     u D t c                             d o a s
              e k                             e c v e
          A V r                               / i e
          c a n                               D t
          c r                                 o y
          e i                                 u
          n a                                 b
          t t                                 l
            i                                 e
            o
            n
            s
```

--------------------------------------------------------------------------------

List of required components in order to build your unit
-------------------------------------------------------

- The Main PCB produced by us. You can order on the www.Beat707.com site.
- USB Power for the Main PCB.
- The components required by the board above. Specially the ATmega328 IC and the Flash Memory IC. The ATmega328 runs with the internal Oscillator at 8Mhz and 3.3 VCC operation. See complete list below.
- 3 x TM1638 boards such as this one: https://github.com/Beat707/Beat707-NXT/blob/master/Hardware/TM1638.jpg or from eBay: http://www.ebay.com/itm/400985462447
- A MIDI Cable and a MIDI compatible unit to produce some Audio. EG: a synth keyboard or a sound module. As the Beat707 NXT unit itself only produces MIDI, NOT Audio.
- To program the ATmega328 you will also need some sort of ISP programmer. Ordering an UNO based ATmega328 won't do, as it requires 5V and 16Mhz external oscillator to run. The easy way is to order an Arduino UNO and the USB Tiny ISP programmer to program the ATmega328 on the UNO, changing it to run on 3.3V and internal 8Mhz Oscillator. Read more below.
- The Flash memory is the Winbond W25Q64 BVAIG - DIP

--------------------------------------------------------------------------------

Connecting the boards
---------------------

- From the Main PCB you can see the Main Header, which starts from the top: 5V+, Ground, STB (Strobe), CLK (Clock), Data 1, data 2 and Data 3.
- Now, the first 4 pins are shared on all the 3 x TM1638 boards. While Data 1 is for the first board (on the left) DIO pin, data 2 is for the middle board DIO pin and finally Data 3 for the last (on the right) board DIO pin.
- The best thing is to remove the J1 headers from the TM1638 boards and just pass wires from behind the board.

Main Header (top to bottom)
---------------------------

- 5V+
- GND (Ground)
- STB (Strobe)
- CLK (Clock)
- Data 1 (DIO pin from the board on the left)
- Data 2 (DIO pin from the middle board)
- Data 3 (DIO pin from the board on the right)

--------------------------------------------------------------------------------

BOM (Build Of Materials) List for the Main PCB
----------------------------------------------
- 1x ATmega328P-PU DIP-28 - 8-Bit Microcontroller (using the internal 8Mhz Oscillator at 3.3V)
- 1x Socket for the ATmega328 IC. DIP-28.
- 1x W25Q64 DIP - 8Mbit Flash Memory Chip (3.3V)
- 1x 4N35 DIP - Octopler
- 1x 1k Ohm Resistor
- 1x 10k Ohm Resistor
- 1x 200 Ohm Resistor
- 1x 10 Ohm Resistor
- 1x 33 Ohm Resistor
- 1x 1N4748 Diode
- 2x Female Din-5 - MIDI Connector
- 1x 47uf Electrolitic Capacitor
- 4x 100nF Ceramic Capacitor
- 1x LD33V - 3.3V Voltage Regulator
- 1x Female USB-B Connector
- 3x Male Jumpers (for power, midi in and midi out. if you plan on leaving those always on, just short-solder the connections instead)
- 6x Male Headers for the FTDI Connection

--------------------------------------------------------------------------------

Programming the ATmega328 IC
----------------------------

- The board requires an ATmega328 IC that has been set to use its internal 8Mhz oscillator. So you can't just get one that has the UNO Bootloader. You will need to use an ISP programmer and connect to the ATmega328 IC and set the correct FUSES so it uses the internal 8Mhz Oscillator in order to run at 3.3V.
- One way to do this is to use the cheap USB Tiny ISP programmer and connect it to an Arduino Board which uses a DIP ATmega328, not the SMD ones.
- To set the right fuses, use the following command: avrdude -p ATMEGA328P -c usbtiny -U lfuse:w:0xe2:m -U hfuse:w:0xda:m -U efuse:w:0x05:m -C "Arduino IDE PATH\hardware\tools\avr\etc\avrdude.conf"
- Be sure to change "Arduino IDE PATH" to the correct location. In the command above I used the usbtiny programmer. Change the name for the one you are using. You can do a search for "avrdude" to check where it is located (install the Arduino IDE first).
- This must be done only one time. Now to upload to the board you can select "Arduino Pro or Pro Mini" and set to "ATmega328 8Mhz 3.3V" in the options. Only worry about the Bootloader if you plan to use the board FTDI connection.
- If you don't see the Pro or Pro Mini option in the boards you will need to use the Boards Manager and add the MiniCore. Google on how to add the following link to the boards manager: https://mcudude.github.io/MiniCore/package_MCUdude_MiniCore_index.json
- Here's a tutorial on how to do the Boards Manager: https://github.com/MCUdude/MiniCore#boards-manager-installation
- When using MiniCore, you need to select MiniCore->ATmega328, Clock->8 Mhz Internal, the rest leave unchanged.
- After programming the IC, just remove and place on the Beat707 NXT board.
- Why running the board on 8Mhz? So we can power it with 3.3V and talk directly to the Flash IC without extra components.

--------------------------------------------------------------------------------

List Of Changes

```
 * Version 1.0.8 - October 26 2018
 * 
 *    Added external MIDI clock code.
 *    Better sequencer timing using extra MIDI buffer.
 *    New option for editing Note Tracks: when editing the steps (not editing a single step) you can hold one (or multiple) steps and use the +/- buttons to raise or lower the note(s) pitch. When you hold a step the BPM display on the left will change to P000 and show if you are adding or subtracting the pitch. While holding a step you can also press the track selector (button #8 from the left) to toggle between selected step(s) or ALL steps.
 *    New option to enable MIDI input to pattern selection MIDI_IN_TO_PATTERN.
 *    Added new option to record MIDI input RECORD_ENABLED - when enabled an extra option will appear in the global menu REC ON or OFF for the selected track. If NO tracks are selected to record, it will record the current selected track, otherwise it will record on multiple selected tracks and not the current selected one (if not selected to record on the global menu).
 *    Added separated button debounce values for when the sequencer is idle or playing (check the TM1638 file).
 *    Added ECHOS_ENABLED option to disable the MIDI Echo Delay FX.
 *    Changed how Drum and Note Tracks output MIDI information, doing a ping-pong from one type of track to another as they are output. EG: Drum1, Note1, Drum2, Note2, Drum3, Note3, ...
 *    Removed the Song Editor to give more room to other better functions for Live usage.
 *    Instead of Song# we now have Pattern Bank#.
 *    Reorganized the global menu to show things that are not global first, which will show all leds on when they are trully global.
 *    When editing notes if you turn it off now it will set velocity=0 and keep the note number, so if you turn a previously off-note it will remember which note is was.
 *    Added Echo/Delay FX to Note Tracks too.
 *    Added TRACK_DEBUG_MIDI, when set the code will store the last 4 bytes of MIDI Input Message and show those values when you hold the track selector button (Bt#8 on the left).
 *    Added SPI code into the folder and optimized a few things for shorter code.
 *    Removed all delay() code with a shorter custom waitMs() code, this also frees another timer.
 *    Replace all pinMode(), digitalRead() and digitalWrite() with direct port manipulation, this frees a lot of programming memory but also hardcodes all pins.
 *    Added another global option, Protect (Memory Protect), when On it will not save patters and any options. Use the TEMPORARY_MESSAGE_TIME value to adjust the message it shows if the memory is protected (max of 255).
 *    Added Pattern Plays, Next Pattern and Chain Plays. You can find those options in the Global Menu, and those values are saved per-pattern. The Chain Plays is separated from the Pattern Plays. So when the chain play is not OFF and INF, it will count how many times this pattern played and instead of going to the Next Pattern value it will go to the actual next pattern instead. See example below.
 *        If you want the following example to play: (P01 P02 P02 P03) (P01 P02 P02 P03) P04 and stay on 4. You set:
 *        Pattern 01 - Next Pattern 02  - Plays 1 - Chain Plays Off
 *        Pattern 02 - Next Pattern 03  - Plays 2 - Chain Plays Off
 *        Pattern 03 - Next Pattern 01  - Plays 1 - Chain Plays 002 (this sets this step as the end of a chain, and Next Step is the start of a chain)
 *        Pattern 04 - Next Pattern Off - Plays 1 - Chain Plays Off
 *    Removed the Solo option in the Mute page.
 *    To stop the sequencer you need to hold the stop button. Pressing stop without holding is ignored.
 *    By default all patterns will repeat until you select another one. You can press play while the sequencer is already playing to check the current repeat mode, press again to change the mode while the message is still been displayed. You have 3 options: Normal, Repeat Chain or Repeat Pattern. In normal mode it will play each pattern and repeat many times depending on the Pattern Plays amount. Once that's finished it will go to the next patter or to the pattern set in Pattern Next.
 *    Small cosmetic changes to the global menu display.
 *    Optimized and organized the code a bit better in a more correct C style.
 *    Several bug fixes.
 * 
 * Version 1.0.0 - Sep 28 2017
```