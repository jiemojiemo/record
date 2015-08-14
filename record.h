#pragma once

/*******************************
**FileName	�� record.h
**Creator	�� HW
**Date		�� 2015-08-14
**Modifier	�� 
**ModifyDate�� 
**Comment	�� ¼����
**Version	�� 1.0.0
/*******************************/
#include "portaudio.h"


#define SAMPLE_TYPE_FLOAT32 0x01
#define SAMPLE_TYPE_INT16   0x03
#define SAMPLE_TYPE_INT8    0x04
#define SAMPLE_TYPE_UINT8   0x05

struct RecordeInfo
{
	unsigned int seconds;
	unsigned int channels;
	unsigned int sampleRate;
	unsigned int sampleType;
	unsigned int framesPerBuffer;
};

struct RecordData{
	int          frameIndex;  /* Index into sample array. */
	int          maxFrameIndex;
	unsigned int totalBytes;
	void	     *recordedSamples;
};

class CRecorder
{
private:
	CRecorder& operator=( const CRecorder& re ){}
	CRecorder( const CRecorder& re ){}

	/**
	* @brief : ����bits���format
	*/
	PaSampleFormat GetSampleFormat();

	template<class T>
	static int _DoRecord( RecordData* data, T wptr,const void* inputBuffer, double silence );

private:
	static RecordeInfo m_Info;
	RecordData  m_Data;
	PaStream*	m_Stream;

public:
	CRecorder();
	CRecorder( const RecordeInfo& info );
	~CRecorder();

	/**
	* @brief : ����¼������
	*/
	void SetInfo( const RecordeInfo& info );

		/**
	* @brief : ��ȡ¼������
	*/
	RecordeInfo GetInfo()const;

	/**
	* @brief : ����¼������������¼��
	*/
	int StartRecord();

	/**
	* @brief : ��¼�������ݱ���Ϊһ���ļ�
	*/
	void SavePcm2File( const char* inFile );

	/**
	* @brief :¼���Ļص����� 
	*/
	static int recordCallback(
		const void *inputBuffer, void *outputBuffer,
		unsigned long framesPerBuffer,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags,
		void *userData);
};