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
000	(0)	00000	(00)	Sequence Trigger If a Slave Sequencer Unit receives this, the unit should start the internal clock from zero. It will play until it reaches the end of all steps, stop the timer, and wait for another Sequence Trigger message.
001	(1)	00001	(01)	Touch Sensor / Next byte: 3 bits = type, 5 bits = button# / types: 0 click, 1 hold, 2 release  (see table below).
001	(1)	00010	(02) 	Steps Selector / Next byte: 0 = Steps A,  1 = Steps B,  2 = Steps C,  3 = Steps D, 4 = Steps Auto
001	(1)	00011	(03)	Change Tempo / Next byte = BPM 0~255
011	(3)	00100	(04)	Sequencer to SND Generator / Next byte: 3 bits = type (see table below), 5 bits = destination SND Generator unit ID, plus another two bytes.
010     (2)     00101   (05)    Master to All Sequencers / Next bytes: code and data (see table below).

001	(1)	11000	(24)	Master or Slave Sequencers To Slave or Master Sequencer / Next byte: 4 bits = code (see table below), 4 bits = data.
001	(1)	11001	(25)	Count SND Generator Units / Next bytes: counter (same as the code below).
001	(1)	11010	(26)	Count Sequencer Units / Next byte: counter. The Master will send this when it boots and each Sequencer Unit will use the counter for its Unit ID, do a counter++ and echo the code so the next unit can continue it until the Master receives it again.
001	(1)	11011	(27)	Slave Sends Data / Next bytes: data.
001	(1)	11100	(28)	Slave Responds and Requests / Next bytes: code (see table below).
001	(1)	11101	(29)	Master Sends Data / Next byte: data.
001	(1)	11110	(30)	Master Requests / Next byte: 3 bits = type, 5 bits = unit ID (see table below).
000	(0)	11111	(31)	Extended Message: the next byte code should be added by 31. This message can be sent multiple times before a new code. EG: if 2 messages of this code are received, the next message code should be # + 31 + 31.

111             11111   0xFF + 'B' + 'T' Serial Init Bytes (3): this is sent by the Master IC #3 to all units to start the serial talk. Anything received before this is ignored and this is not sent again. The Master IC #3 will wait 3 seconds to check if the Serial Loop is working correctly, otherwise it will display an error message. Nothing will work until the Serial Loop is correctly setup.

---------------------------------------------------------------------------------------------

(01) Touch Sensor (note, the left and right buttons will send a hold right after you touch it, and only send a click if nothing was selected while holding those buttons)

Type		Button #
----		-------
(0)		*****		Click
(1)		*****		Hold
(2)		*****		Release

---------------------------------------------------------------------------------------------

(04) Sequencer to SND Generator (3 bytes)

Type		Unit ID		Description	Byte 2		Byte 3
----		-------		-----------	------		------
(0)		*****		Note On		note # 		velocity
(1)		*****		Note Off	note # 		velocity
(2)		*****		Reset		0		0

Master with MIDI can assign unit IDs to MIDI Channel Outputs. EG: Unit ID # 31 = MIDI Channel 16 output.

---------------------------------------------------------------------------------------------

(05)    Master to All Sequencers (2 bytes: code and data).

Code            Description
----            -----------
(0)             Set BPM Tempo
(1)             Set Number of Steps

---------------------------------------------------------------------------------------------

(24) Master or Slave Sequencers To Slave or Master Sequencer (4 bits code and 4 bits data)

Code
----
(0)	Slave to Master: Start/Stop Sequencer.
(1)	Slave to Master: Mirror Mode Toggle.
(2)	
(3)	Slave to Master: Steps Selection Change. Next 4 bits is the selection: A, B, C, D and Auto = 0, 1, 2, 3 and 4.
(4)	Master to Slave: Stop the Sequencer.
(5)	Master to Slave: Mirror Mode (next 4 bits = 1 on or 0 off).
(6)	
(7)	Master to Slave: Steps Selection Change.

---------------------------------------------------------------------------------------------

(28) Slave Responds and Requests

Code
----
(0)	End of Data Dump.
(1)	Slave Requests Next Pattern Data (for the Song Mode, only the first unit does this).

---------------------------------------------------------------------------------------------

(30) Master Requests

Type		Unit ID
----		-------
(0)		*****		Request All Data.
(1)		*****		Next Data (from the above).
(2)		*****		Sending Next Pattern Data (for the Song Mode).
(3)		*****		Sending All Sequencer Data.
(4)		*****		Finished Sending Data (for the two codes above).

---------------------------------------------------------------------------------------------

*/
