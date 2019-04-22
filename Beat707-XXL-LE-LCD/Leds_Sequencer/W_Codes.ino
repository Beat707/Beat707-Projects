/*

  Created by Guilherme (William) Kalfelz
  @ Beat707 (c) 2014 - http://www.Beat707.com
  
  Beat707 XXL Serial Codes
  On LE units, not all code works
  
---------------------------------------------------------------------------------------------

1 Byte Message (8 bits)
First 3 bits		Next 5 bits
Extra Bytes		Main Code

---------------------------------------------------------------------------------------------

= Main Codes =

Extra Bytes	Code		Description
-----------	-----		-----------
000	(0)	00000	(00)	MIDI Clock, sent from the LCD IC
                        (01)    Start the Sequencer, sent from the LCD IC
                        (02)    Stop the Sequencer, sent from the LCD IC
                        (03)    Show Navigation Keys
                        (04)    Hide Navigation Keys
                        (05)    Edit Steps A (Mirror Off)
                        (06)    Edit Steps B (Mirror Off)
                        (07)    Edit Steps C (Mirror Off)
                        (08)    Edit Steps D (Mirror Off)
                        (09)    Select Drum Track 1
                        (10)    Select Drum Track 2
                        (11)    Select Drum Track 3
                        (12)    Select Drum Track 4
                        (13)    Select Note/Special Track
                        (14)    Edit Steps Auto (Mirror On)
                        (15)    Edit Steps Auto (Mirror Off)
                        (16)    Pattern Fade In
                        (17)    Pattern Fade Out
                        (18)    Ready to display the LEDs and to read from the EEPROM
                        (19)    Save Data to EEPROM

001	(1)	00001	(01)	Touch Sensor / Next byte: 3 bits = type, 5 bits = button# / types: 0 click, 1 hold, 2 release  (see table below).
010	(2)	00010	(02) 	Set Note Track LEDs / Next 2 bytes = the 16 bits = 16 steps = 16 leds
001	(1)	00011	(03)	Change Pattern / Next byte = Pattern #
011	(3)	00100	(04)	Sequencer to MIDI / Next byte: 3 bits = type (see table below), 5 bits = MIDI Channel, plus another two bytes.
001	(1)	00101	(05)	Seq Position / Next byte = position
001	(1)	00110	(06)	Steps Position / Next byte = position
001     (1)     00111   (07)    Tracks On / Next byte = tracks to turn on/off (each bit is one track - 5 bits)
001     (1)     01000   (08)    Pattern Special Functions. Next byte: 0 = none, 1 = init pattern, 2 = copy pattern to buffer, 3 = paste buffer, 4 = init current track

---------------------------------------------------------------------------------------------

(01) Touch Sensor (note, the left and right buttons will send a hold right after you touch it, and only send a click if nothing was selected while holding those buttons)

Type		Button #
----		-------
(0)		*****		Click
(1)		*****		Hold
(2)		*****		Release

---------------------------------------------------------------------------------------------

(04) Sequencer to MIDI (3 bytes)

Type		MIDI Ch		Description	Byte 2		Byte 3
----		-------		-----------	------		------
(0)		*****		Note On		note # 		velocity
(1)		*****		Note Off	note # 		velocity
(2)		*****		Reset		0		0
(3)		*****		CC		type            value

---------------------------------------------------------------------------------------------

*/
