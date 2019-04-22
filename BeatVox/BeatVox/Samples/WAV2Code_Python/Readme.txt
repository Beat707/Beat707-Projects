Storing Sounds in FLASH

Sounds are stored either in the Arduino’s on-chip FLASH memory (that is, compiled in as part of your sketch) or in the SPI FLASH on the BeatVox shield. Here we look at the steps needed to take a standard audio file and convert it to the format needed to play on a BeatVox.

    Get a WAV file

        Whatever format your sound is in, you will need to convert it to a monophonic WAV file at a sampling rate of 22039 Hz (22050 Hz is OK too). You can play stereo sounds but each sound must be a separate monophonic WAV file and stored separately in FLASH. You can then play just the left channel, just the right channel, or both channels together.

        Our favorite program for sound file manipulation is SoX. It is a free, cross-platform program that can do just about anything to your audio files. Here is an example command-line call to SoX to convert just about any type of WAV file to the format needed by BeatVox:

            sox InputFile.wav --channels 1 --rate 22039 OutputFile.wav

        SoX will also convert from other file formats to the WAV file format -- just give it an input audio file and it will auto-detect the format.
    Get more WAV files

        After you’ve converted all the WAV files you want to play to be monophonic and 22039 Hz, put them all in one directory.
    Generate the Sounds.h header file

        To compile the sounds into your sketch, you will process all of the audio files through our free voxheader.py Python program. Chances are good you already have Python installed, but if not then it’s a simple and free installation (works on all platforms) from http://www.python.org.

        Assuming you’ve got all the WAV files in the current directory you just type:

            python voxheader.py *.wav

        This will generate a Sounds.h file that you can then put into your sketch directory. This file is included in your PDE sketch file and the sounds then become part of your program.
