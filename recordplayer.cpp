#include "recordplayer.h"
#include "opengl_part.h"
#include "SDL.h"
#include <stdio.h>
#include <string>

CRecordPlayer::CRecordPlayer()
{

}

CRecordPlayer::CRecordPlayer( const char* url )
{

}
CRecordPlayer::CRecordPlayer( const CRecorder& recorder )
{

}

CRecordPlayer::CRecordPlayer( unsigned long byteSize, const void* data )
{

}

CRecordPlayer::~CRecordPlayer()
{
	if( m_data.recordedSamples != NULL )
	{
		free( m_data.recordedSamples );
		m_data.recordedSamples = NULL;
	}
}

/*******************************
** FunctionName：GetSampleFormat
** Comment:     根据bits获得format
** return：     format
** Creator：    HW
** Date：		2015-8-14
** Modifier：	
** ModifyDate： 
** Version：    1.0.0
*******************************/
PaSampleFormat CRecordPlayer::GetSampleFormat()
{
	switch( m_playInfo.sampleType )
	{
	case SAMPLE_TYPE_FLOAT32:
		return paFloat32;
	case SAMPLE_TYPE_INT16:
		return paInt16;
	case SAMPLE_TYPE_INT8:
		return paInt8;
	case SAMPLE_TYPE_UINT8:
		return paUInt8;
	default:
		return paFloat32;
	}
}

/*******************************
**（函数名）AudioInit：
**（功能用途）：初始化音频设备
**（返回值）return： 错误标示，0标示正常
** (作者)Creator：
** (日期)Date：
**（修改人）Modifier：
**（修改日期）ModifyDate：
**（版本）Version：
*******************************/
int CRecordPlayer::AudioInit()
{
	PaError					err;				
	PaStreamParameters		outputParameters;
	//portaudio环境初始化
	err = Pa_Initialize();
	if( err != paNoError )
		return err;

	outputParameters.device = Pa_GetDefaultOutputDevice();
	if( outputParameters.device == paNoDevice )
	{
		fprintf( stderr, "Error: No default output device.\n" );
		return -1;
	}
	outputParameters.channelCount = m_playInfo.channels;
	outputParameters.sampleFormat = this->GetSampleFormat();
	outputParameters.suggestedLatency = 
		Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
	outputParameters.hostApiSpecificStreamInfo = NULL;

	printf("\n=== Now playing back. ===\n"); fflush(stdout);

	err = Pa_OpenStream( &m_stream,
		NULL, //no input
		&outputParameters,
		m_playInfo.sampleRate,
		m_playInfo.framePerBuffer,
		paClipOff,
		playCallback,
		&m_data
		);
	if( err != paNoError ) 
		return err;

	return 0;
}

/*******************************
**（函数名）StartPlay：
**（功能用途）：开始播放音频和显示
**（返回值）return： 错误标示，0标示正常
** (作者)Creator：
** (日期)Date：
**（修改人）Modifier：
**（修改日期）ModifyDate：
**（版本）Version：
*******************************/
int CRecordPlayer::StartPlay()
{
	//初始化opengl,创建窗口等待显示（开另一条线程）
	SDL_Thread* openglThread = SDL_CreateThread( OpenGLDisplay, "OpenGL Display", NULL );

	//打开音频设备，初始化它
	SDL_Thread* audioThread = SDL_CreateThread( AudioPlay, "Audio Play", NULL );
	
	int ret;
	SDL_WaitThread( audioThread, &ret );
	return ret;

	return 0;
}