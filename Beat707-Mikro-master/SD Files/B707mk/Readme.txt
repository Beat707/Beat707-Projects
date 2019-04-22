Beat707 Mikro - created by William (Guilherme) Kalfelz (c) 2016 - www.Beat707.com - www.Wusik.com - www.Kalfelz.com

All the following files are required in order for the Beat707 Mikro code to run.

ss.dat - Global Settings
sg.dat - Last loaded song. A Number from 01 to 99. By default the last loaded song is 01. 2 chars.
s0.dat - Internal menus and screens, 16 chars per line, 2 lines per screen
s1.dat - Extra Pattern Options, such as Patern Fade In/Out. 12 chars, single line
s2.dat - Drum Track Names, such as Kick, Snare, Clap. 9 chars, single line
s3.dat - Note Track Names, such as Bass, Synth, ARP. 9 chars, single line
s4.dat - Pattern Name, such as Intro, Chorus, Ending. 9 chars, single line
s5.dat - Custom LCD Chars. 5 x 8 lines, extra empty line per custom char. Step Off, Step On, Step Double, Note Off, Variations A B C D.
s6.dat - Custom LCD Chars when in Mirror mode.

For each Song there must be a subfolder named from 01 to 99, inside it there will be the sng.dat file with the Song Settings and all the pattern files named p**.dat and the pattern settings is stored on s**.dat There's also a TP folder for the song template, and a CP folder for copying/pasting patterns and tracks. Song positions (pattern itens) are stored in g**.dat files. Custom Chords are stored in a chd.dat file.

Files must be saved in Windows Text format (2 bytes for new lines).