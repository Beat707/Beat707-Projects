/*

  Created by Beat707 (c) 2011 - http://www.Beat707.com
  
  WAV to C code converter

  10 Bits version (in groups of 40 bits - 4 samples)
  Saved as 8 Bits unsigned int arrays
  Values goes from 0 to 725 (22068hz)
  
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <windows.h>
#include <bitset>
using namespace std;

typedef struct 
{
  long	chunkSize;
  long	dwManufacturer;
  long	dwProduct;
  long	dwSamplePeriod;
  long	dwMIDIUnityNote;
  long	dwMIDIPitchFraction;
  long	dwSMPTEFormat;
  long	dwSMPTEOffset;
  long	cSampleLoops;
  long	cbSamplerData;
  long  dwIdentifier;
  long  dwType;
  long  dwStart;
  long  dwEnd;
  long  dwFraction;
  long  dwPlayCount;
} SamplerChunk;


struct Header
{
	char sChunk[4];
	long Size;
	long VelocityLow;
	long VelocityHigh;
	long KeyLow;
	long KeyHigh;
	long loopStart;
	long loopEnd;
	long Root;
	long Samplerate;
	long Channel;
};

inline signed int byte_swap(signed int v)
{
	return (
      (v >> 24) & 0xFF 
      | (v >> 8) & 0xFF00 
      | (v << 8) & 0xFF0000 
      | (v << 24) & 0xFF000000 
   );
};

void loadWAV(char *filePath);
void swap16(unsigned char *ptr);
void swap32(unsigned char *ptr);
void swap64(unsigned char *ptr);

int counter = 0;
char allLens[9049] = {0};
int totalSize = 0;

// ---------------------------------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
	memset(allLens,0,sizeof(allLens));

	printf("\n");
	printf(" +----------------------------------------------+\n");
	printf(" | WAV To Code V1.0 (8-bits unsigned int)       |\n");
	printf(" |                                              |\n");
	printf(" | Reads all .WAV files in the current folder   |\n");
	printf(" | and output as 8-bit code to PCM_Sound.h      |\n");
	printf(" |                                              |\n");
	printf(" | http://www.Beat707.com                       |\n");
	printf(" +----------------------------------------------+\n\n");

	// Clear .c file first //
	FILE *fdwrite;
	fdwrite = fopen("PCM_Sound.h","wb");
	//fprintf(fdwrite,"#include <binary.h>\n\n");
	fclose(fdwrite);

	// check for Files //
    char   psBuffer[128];
    FILE   *sDirectory;
	char sDir[9096];
	memset(sDir,0,sizeof(sDir));
    char readDir[1024];
    memset(readDir,0,sizeof(readDir));
    strcpy(readDir, "dir ");
    strcat(readDir, "*.wav /b");
	if( (sDirectory = _popen("cd", "rt" )) == NULL ) exit( 1 );
	fgets( psBuffer, 128, sDirectory );
	strncpy(sDir,psBuffer,strlen(psBuffer)-1);
	strcat(sDir,"\\");
    if( (sDirectory = _popen(readDir, "rt" )) == NULL ) exit( 1 );

    // Read Directory
    while( !feof( sDirectory ) )
    {
		if( fgets( psBuffer, 128, sDirectory ) != NULL ) 
		{
			printf(" *      Converting: ");
			psBuffer[strlen(psBuffer)-1] = 0;
			printf(psBuffer);
			loadWAV(psBuffer);
			printf("\n");
		}
    }

    // Close pipe sDirectory. */
    if (_pclose( sDirectory ) == 1) 
	{
		printf("\n");
	}

	// Final Steps //
	fdwrite = fopen("PCM_Sound.h","ab");
	fprintf(fdwrite,"// unsigned int totalBytes = %d; // = %.2f Kbytes\n",totalSize, float(totalSize)/1024);

	fprintf(fdwrite,"// char* PCMname[%d] = {",counter);
	for (int q=0; q<counter; q++) 
	{ 
		if (q > 0) fprintf(fdwrite,", "); 
		fprintf(fdwrite,"_PCMname_%d",q+1); 
	}
	fprintf(fdwrite,"};\n");

	fprintf(fdwrite,"// int* PCMrate[%d] = {",counter);
	for (q=0; q<counter; q++) 
	{ 
		if (q > 0) fprintf(fdwrite,", "); 
		fprintf(fdwrite,"_PCMrate_%d",q+1); 
	}
	fprintf(fdwrite,"};\n");


	fprintf(fdwrite,"unsigned int PCMlen[%d] = {%s};\n",counter,allLens);
	
	fprintf(fdwrite,"unsigned char* PCMdata[%d] = {",counter);
	for (q=0; q<counter; q++) 
	{ 
		if (q > 0) fprintf(fdwrite,", "); 
		fprintf(fdwrite,"_PCMdata_%d",q+1); 
	}
	fprintf(fdwrite,"};\n");
	
	fprintf(fdwrite,"unsigned char PCMtotal = %d;",counter);
	fclose(fdwrite);

	printf("\n    Done.");
	printf("\n");
	printf("\n");
	printf("\n");
	system("PAUSE");

}

// ---------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------

typedef unsigned long BEAT707_FORMAT;
static const BEAT707_FORMAT BEAT707_SINT8 = 1;   /*!< -128 to +127 */
static const BEAT707_FORMAT BEAT707_SINT16 = 2;  /*!< -32768 to +32767 */
static const BEAT707_FORMAT BEAT707_SINT24 = 8;

typedef signed short SINT16;
typedef signed int SINT24;
typedef signed int SINT32;

//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------

void loadWAV(char *filePath)
{
	// INIT //
	int ii = 0;
	unsigned long samples;
    char header[12];
	char id[4];
	int dataType;
	float max;
    long i, length;
    bool endfile;
	long chunkPointer = 0;
	long dataOffset;
    bool byteswap = false;
    long fileSize;
    long bufferSize;
	long fileRate;
	int channels;
	fpos_t pos = 1;
	int Jobs = 0;
	SINT16 temp;
	float* data = 0;

	// ----------------------------------------------------------

	if (strlen(filePath) <= 1) goto error3;

	// Open Output File //
	FILE *fdwrite;
	fdwrite = fopen("PCM_Sound.h","ab");

    // Try to open the file.

	FILE *fd;
	fd = fopen(filePath, "rb");
	if (!fd) 
	{
		printf("Error: Could not open File: %s",filePath);
		goto error2;
	}

	if ( fread(&header, 4, 3, fd) != 3 ) { printf("Could not read header of the File."); goto error2; }
	if (!( !strncmp(header, "RIFF", 4) && !strncmp(&header[8], "WAVE", 4) )) { printf("Not a valid WAV file - no header information."); goto error2; };

	while (1) 
	{

		if ( fread(&id, 4, 1, fd) != 1 ) break;

		// --------======-------------============-----------==================---------------================
		if (!strncmp(id, "fmt ", 4)) {
			// Format CHUNK //
			SINT32 chunkSize;		  
			fread(&chunkSize, 4, 1, fd);
			#ifndef __LITTLE_ENDIAN__
				swap32((unsigned char *)&chunkSize);
			#endif

			// Check that the data is not compressed.
			SINT16 format_tag;
			//if ( fseek(fd, 4, SEEK_CUR) == -1 ) { printf("Could not read format."); goto error2; }
			if ( fread(&format_tag, 2, 1, fd) != 1 ) { printf("Could not read format (2)."); goto error2; };
			#ifndef __LITTLE_ENDIAN__
				swap16((unsigned char *)&format_tag);
			#endif
			if (format_tag != 1 && format_tag != 3 ) { // PCM = 1, FLOAT = 3
				printf("Not a Valid WAV/PCM format!"); goto error2;
			}

			// Get number of channels from the header.
			if ( fread(&temp, 2, 1, fd) != 1 ) { printf("Could not read number of channels."); goto error2; };
			#ifndef __LITTLE_ENDIAN__
				swap16((unsigned char *)&temp);
			#endif
			channels = (unsigned int ) temp; 

			// Get file sample rate from the header.
			SINT32 srate;
			if ( fread(&srate, 4, 1, fd) != 1 ) { printf("Could not read SampleRate."); goto error2; };
			#ifndef __LITTLE_ENDIAN__
				swap32((unsigned char *)&srate);
			#endif
			fileRate = (float) srate;

			// Determine the data type. ----------------------------------- //
			dataType = 0;
			if ( fseek(fd, 6, SEEK_CUR) == -1 ) { printf("Could not read data format."); goto error2; }
			if ( fread(&temp, 2, 1, fd) != 1 ) { printf("Could not read data format (2)."); goto error2; }
			#ifndef __LITTLE_ENDIAN__
				swap16((unsigned char *)&temp);
			#endif
			if ( format_tag == 1 )
			{
				if (temp == 8)	dataType = BEAT707_SINT8;
				if (temp == 16)	dataType = BEAT707_SINT16;
				if (temp == 24)	dataType = BEAT707_SINT24;
			}
			else
			{
				printf("Unsuported file format."); 
				goto error2; 
			};
		}
		// --------======-------------============-----------==================---------------================
		else if (!strncmp(id, "data", 4)) 
		{
			// Get length ----------------------------------- //
			SINT32 bytes;
			if ( fread(&bytes, 4, 1, fd) != 1 ) { printf("Could not find lenght header."); goto error2; };;
			if (bytes == 0) {
				Jobs--;
				goto zerobyte;
			}
			#ifndef __LITTLE_ENDIAN__
				swap32((unsigned char *)&bytes);
			#endif
			fileSize = 8 * bytes / temp / channels;// sample frames
			bufferSize = fileSize;
			dataOffset = ftell(fd);
			byteswap = false;
			#ifndef __LITTLE_ENDIAN__
				byteswap = true;
			#endif

			// Read Data ----------------------------------- //
			samples = (bufferSize+1)*channels;
			data = (float *) new float[samples+100];
			memset(data,0,sizeof(data));
			chunkPointer = 0;

			length = bufferSize;
			endfile = (chunkPointer+bufferSize == fileSize);
			if ( !endfile ) length += 1;

			// Read samples into data[].Use float data structure
			// to store samples.
			if ( dataType == BEAT707_SINT16 )
			{
				SINT16 *buf = (SINT16 *)data;
				if (fseek(fd, dataOffset+(long)(chunkPointer*channels*2), SEEK_SET) == -1) goto error;
				if (fread(buf, length*channels, 2, fd) != 2 ) { printf("Error while reading the file."); goto error2; };;
				if ( byteswap ) {
				SINT16 *ptr = buf;
				for (i=length*channels-1; i>=0; i--)
					swap16((unsigned char *)(ptr++));
				}
				for (i=length*channels-1; i>=0; i--) data[i] = buf[i] / 32768.0f;
			}
			else if ( dataType == BEAT707_SINT24 )
			{
				if (fseek(fd, dataOffset+(long)(chunkPointer*channels*3), SEEK_SET) == -1) goto error;

				char temp_buffer[3]; 

				for (i=0; i<length*channels; i++)
				{ 
					fread(temp_buffer, 3, 1, fd); 
					long value = (unsigned char)temp_buffer[0]; 
					value |= (unsigned char)temp_buffer[1] << 8; 
					value |= (signed char)temp_buffer[2] << 16; 

					data[i] = (value ) / 8388608.0f; 
				}
			}
			else if ( dataType == BEAT707_SINT8 )
			{
				char *buf = (char *)data;
				if (fseek(fd, dataOffset+(long)(chunkPointer*channels), SEEK_SET) == -1) goto error;
				if (fread(buf, length*channels, 1, fd) != 1 ) { printf("Error while reading the file."); goto error2; };;
				for (i=length*channels-1; i>=0; i--) data[i] = buf[i] / 128.0f;
			}

			// CONVERT TO FLOAT & Normalize //
			max = 0.0f;
			for (i=0; i<samples; i++)
			{
			  if (fabs(data[i]) > max)
				max = (float) fabs((double) data[i]);
			}
			if (max > 0.0) max = (float) 1.0 / max; else max = 1.0f;

			for (i=0;i<samples;i++) data[i] *= max;

			// IF NOT MONO, CONVERT //
			if (channels > 1)
			{
				float *dataConv;
				dataConv = (float *) new float[samples/channels];

				for (i=0;i<(samples/channels);i++) {
					float conv = 0;
					for (int x=0; x<channels; x++) conv += data[x+(i*channels)];
					dataConv[i] = conv / channels;
				}

				delete[] data;
				samples = samples / channels;
				data = (float *) new float[samples+100];
				memset(data,0,sizeof(data));

				for (i=0;i<samples;i++) data[i] = dataConv[i];

				delete[] dataConv;
			}

			zerobyte:
			;// Do nothing
		}
		// --------======-------------============-----------==================---------------================
		else if (!strncmp(id, "smpl", 4)) {
			SamplerChunk value;
			if (fread(&value, sizeof(SamplerChunk), 1, fd))
			{
				if (value.dwStart >= 0 && value.dwEnd > 0) //value.dwStart < value.dwEnd && value.dwEnd <= samples && value.dwStart < samples)
				{
					//.loopStart = value.dwStart;
					//.LoopEnd = value.dwEnd;
				}
			}
		}
		// --------======-------------============-----------==================---------------================
		else {
			fseek(fd, -3, SEEK_CUR);
			Jobs--;
		}

		Jobs++;
		if (Jobs > 2) break;

	}
	//-------------------------------------------------------
	// ALL GOOD - CONVERT AND OUTPUT TO C //
	filePath[strlen(filePath)-4] = 0;
	fprintf(fdwrite,"// -------------------------------- Sound: %s -------------------------------- //\n", filePath);
	
	fprintf(fdwrite,"// char _PCMname_%d[] PROGMEM =  {", counter+1);
	for (i=0;i<strlen(filePath);i++) 
	{
		if (i > 0) fprintf(fdwrite,",");
		fprintf(fdwrite,"'%c'",filePath[i]);
	}
	fprintf(fdwrite,"};\n");

	data[samples-1] = data[samples] = 0.0f;

	fprintf(fdwrite,"// unsigned int _PCMrate_%d =  %d;\n", counter+1, fileRate);
	fprintf(fdwrite,"unsigned char _PCMdata_%d[] PROGMEM =  { \n", counter+1);

	for (i=0;i<samples;i++) 
	{
		if (i > 0 && i % 30 == 0) fprintf(fdwrite,"\n");
		if (i > 0) fprintf(fdwrite,",");		
		fprintf(fdwrite,"%d",(min(242,(unsigned int)(((data[i] + 1.0f) * 121.0f)))));

	}
	fprintf(fdwrite,"};\n\n");

	if (allLens[0] != 0) strcat(allLens,",");
	char xsamples[999] = {0};
	sprintf(xsamples,"%d",samples);
	strcat(allLens,xsamples);

	totalSize += samples;

	counter++;
	

	delete[] data;
	if (fd) fclose( fd );
	if (fdwrite) fclose(fdwrite);
	return;

	error:
	printf("Not a Valid WAV File.");
	error2:
	if (fd) fclose( fd );
	error3:
	if (fdwrite) fclose(fdwrite);
	return;
}


//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
void swap16(unsigned char *ptr)
{
  register unsigned char val;

  // Swap 1st and 2nd bytes
  val = *(ptr);
  *(ptr) = *(ptr+1);
  *(ptr+1) = val;
}

void swap32(unsigned char *ptr)
{
  register unsigned char val;

  // Swap 1st and 4th bytes
  val = *(ptr);
  *(ptr) = *(ptr+3);
  *(ptr+3) = val;

  //Swap 2nd and 3rd bytes
  ptr += 1;
  val = *(ptr);
  *(ptr) = *(ptr+1);
  *(ptr+1) = val;
}

void swap64(unsigned char *ptr)
{
  register unsigned char val;

  // Swap 1st and 8th bytes
  val = *(ptr);
  *(ptr) = *(ptr+7);
  *(ptr+7) = val;

  // Swap 2nd and 7th bytes
  ptr += 1;
  val = *(ptr);
  *(ptr) = *(ptr+5);
  *(ptr+5) = val;

  // Swap 3rd and 6th bytes
  ptr += 1;
  val = *(ptr);
  *(ptr) = *(ptr+3);
  *(ptr+3) = val;

  // Swap 4th and 5th bytes
  ptr += 1;
  val = *(ptr);
  *(ptr) = *(ptr+1);
  *(ptr+1) = val;
}
