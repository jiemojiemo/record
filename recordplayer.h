#pragma once
#include "record.h"

struct RecordPlayInfo{
	unsigned int sampleRate;
	unsigned int channels;
	unsigned int sampleType;
	unsigned int framePerBuffer;
};

class CRecordPlayer{
	friend class CRecorder;
private:
	CRecordPlayer( const CRecordPlayer& p ){}
	CRecordPlayer& operator=( const CRecordPlayer& p ){}

	/**
	* @brief : ����sample type���format
	*/
	PaSampleFormat GetSampleFormat();

	/**
	* @brief : ��ʼ����Ƶ�豸��opengl
	*/
	int Init();
	/**
	* @brief : ��Ƶ�豸��ʼ��
	*/
	int AudioInit();
public:
	CRecordPlayer( unsigned long byteSize, const void* data );
	CRecordPlayer( const CRecorder& recorder );
	CRecordPlayer( const char* url );
	CRecordPlayer();
	~CRecordPlayer();

	/**
	* @brief : ��ʼ���ź���ʾ
	*/
	int StartPlay();

	/**
	* @brief : ������Ƶ�Ļص�����
	*/
	static int playCallback(const void *inputBuffer,
		void *outputBuffer, unsigned long framesPerBuffer,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags, 
		void *userData);

private:
	RecordData	m_data;
	RecordPlayInfo m_playInfo;
	PaStream*	m_stream;
};