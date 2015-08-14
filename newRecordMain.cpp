#include "record.h"
#include <stdio.h>

#pragma comment( lib, "portaudio_x86.lib" )

int main()
{
	RecordeInfo info;
	info.channels			= 2;
	info.framesPerBuffer	= 512;
	info.sampleType			= SAMPLE_TYPE_FLOAT32;
	info.sampleRate			= 44100;
	info.seconds			= 5;

	CRecorder recorder( info );
	recorder.StartRecord();
	recorder.SavePcm2File( "pcm_2_44100_float_5" );

	printf( "Data size:%ld\n",recorder.GetDataSize() );

	return 0;
}