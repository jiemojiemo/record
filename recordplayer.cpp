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
** FunctionName��GetSampleFormat
** Comment:     ����bits���format
** return��     format
** Creator��    HW
** Date��		2015-8-14
** Modifier��	
** ModifyDate�� 
** Version��    1.0.0
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
**����������AudioInit��
**��������;������ʼ����Ƶ�豸
**������ֵ��return�� �����ʾ��0��ʾ����
** (����)Creator��
** (����)Date��
**���޸��ˣ�Modifier��
**���޸����ڣ�ModifyDate��
**���汾��Version��
*******************************/
int CRecordPlayer::AudioInit()
{
	PaError					err;				
	PaStreamParameters		outputParameters;
	//portaudio������ʼ��
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
**����������StartPlay��
**��������;������ʼ������Ƶ����ʾ
**������ֵ��return�� �����ʾ��0��ʾ����
** (����)Creator��
** (����)Date��
**���޸��ˣ�Modifier��
**���޸����ڣ�ModifyDate��
**���汾��Version��
*******************************/
int CRecordPlayer::StartPlay()
{
	//��ʼ��opengl,�������ڵȴ���ʾ������һ���̣߳�
	SDL_Thread* openglThread = SDL_CreateThread( OpenGLDisplay, "OpenGL Display", NULL );

	//����Ƶ�豸����ʼ����
	SDL_Thread* audioThread = SDL_CreateThread( AudioPlay, "Audio Play", NULL );
	
	int ret;
	SDL_WaitThread( audioThread, &ret );
	return ret;

	return 0;
}