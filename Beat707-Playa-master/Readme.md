# Beat707 Playa
### Open Source Arduino Compatible Audio Player

created by William (Guilherme) Kalfelz (c) 2017

www.Beat707.com - www.Wusik.com - www.Kalfelz.com

-------------------------------------------------------

Uses the flash memory: Winbond W25Q64 BVAIG - DIP (64 Megabits - 8 Megabytes)
Sound is done using PWM on pins 9 and 10.
A Serial connection is used to send audio data to the device, which them can be auto-played by the device. It uses 10 bit audio at 22038hz sample-rate.
The current code playes sound 1, wait 40 seconds them play sound 2. Once it finishes up playing sound 2 it shuts down.