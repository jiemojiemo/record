#pragma once
#include <assert.h>

typedef struct Waves{
	SAMPLE waveData[FRAME_PER_BUFFER*CHANNEL_COUNT];
	Waves( const SAMPLE* data )
	{
		memcpy( waveData, data, FRAME_PER_BUFFER*CHANNEL_COUNT*sizeof(SAMPLE) );
	}
	Waves& operator=( const Waves& wave )
	{
		if( this == &wave )
			return *this;
		memcpy( waveData, wave.waveData, FRAME_PER_BUFFER*CHANNEL_COUNT*sizeof(SAMPLE) );
		return *this;
	}
	Waves& operator=( const SAMPLE* data )
	{
		assert( data != NULL );
		memcpy( waveData, data, FRAME_PER_BUFFER*CHANNEL_COUNT*sizeof(SAMPLE) );
	}
	SAMPLE& operator[]( int pos )
	{
		return waveData[pos];
	}
	const SAMPLE& operator[]( int pos)const
	{
		return waveData[pos];
	}
}SWaves;

int AudioPlay( void* data );
int OpenGLDisplay( void* data );
void DrawWaves();
void DrawWaveOnTop();
void DrawWavesOnButton();
void SetPoints();
void IdelFunction();
void DisplayFunction();
void ReshapeFunction( int w, int h );
void InitializeGraphics();
void KeyboradFunction( unsigned char key, int x, int y );
