# Beat707 MPU
### Open Source Arduino Compatible MIDI Processing Unit

https://github.com/Beat707/Beat707-MPU

created by William (Guilherme) Kalfelz (c) 2017

www.Beat707.com - www.Wusik.com - www.Kalfelz.com

Support us on Patreon: https://patreon.com/williamk

--------------------------------------------------------------------------------

Unit Details
------------

- Pins are Hardwired: Pin 2 is LED Activity and Pin 3 is Button Input (using pull-up resistors). Connect Button to Ground. ATtiny2313 Used.
- To setup, hold the button until the LED stays on, release the button. 
- The next key setup the MODE, from the following list. It won't exit until you select a key below, once you do, the LED will go off. Them you need to press the keys required by the selected Mode. The LED will them go On and stay On once all keys where pressed. The Mode and Settings will them be saved in the internal EEPROM and loaded when you power up the unit again.
 
MIDI Processing Unit Modes
--------------------------

- #60 - Middle C = No Mode, all keys will just echo from input to output.
- #61 - Key Switching. Now you need to press 4 keys: Low Key Zone, High Key Zone, Key Root 1 and Root 2.
- #62 - Low Key to MIDI Channel change. Now you need to press 2 keys: Low Key Zone, High Key Zone. Low Key is MIDI Channel 1 and so on forward.
- #63 - Ignore MIDI Clock's Start and Stop Messages.
- #64 - Sequencer Clock. Keys you need to press: Key Zone Low, High, Sequencer Start, Stop, Speed Up and Down. Speed goes from 50 to 200, with a 10 increment. Default is 120.

--------------------------------------------------------------------------------

Parts List
----------

- ATtiny2313 DIP 20 (THT)
- 100nf ceramic capacitor, SMD 0805
- 1k resistor, SMD 0805
- 10k resistor, SMD 0805
- LED (Red works best), SMD 0805
- Push button (THT)
- (Optional) 3 x 3 Male Headers (or whatever you want to use to connect the MIDI Cable wires into the board. If you decide to connect directly, no need for the male headers)

--------------------------------------------------------------------------------

Programming the ATtiny2313
--------------------------

- If you are doing everything yourself, you will need to connect the ATtiny2313 to a programmer. I uses the USB Tiny ISP programmer and it works great.
- First: in the Arduino IDE, File->Preferences->Additional Boards Manager, add: http://drazzy.com/package_drazzy.com_index.json.
- In Tools->Board->Boards Manager look for the ATtinycore, install it.
- Now in boards select ATtiny2313, set to 8Mhz. Try to upload the bootloader. It doesn't upload one, but set the right fuses.
- You are ready to upload the sketch to the ATtiny2313.

--------------------------------------------------------------------------------

Connecting the MIDI Cable
-------------------------

- The board has two 3 pin headers, one receiver (left of the board) and one transmitter (right of the board).
- On both, GND is the middle pin on the cable. Also labeled as pin 2.
- On the Receiver (left of the board), pin 5 is VCC and pin 4 is RXD.
- On the Transmitter (right of the board), pin 5 is VCC and pin 4 is RXD.
- If the Beat707 MPU board does not power up from the MIDI power, check if your MIDI device has internally set GND to pin 2 of the MIDI connector. You may need to manually solder a wire from GND to pin 2 so the MPU board works.
- If you want to power the unit directly, you can just feed 5V into VCC, but add a 220 ohm resistor, and don't connect pin 5 from the midi cable into the left side of the board headers.