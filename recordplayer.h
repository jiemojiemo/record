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
	* @brief : 根据sample type获得format
	*/
	PaSampleFormat GetSampleFormat();

	/**
	* @brief : 初始化音频设备和opengl
	*/
	int Init();
	/**
	* @brief : 音频设备初始化
	*/
	int AudioInit();
public:
	CRecordPlayer( unsigned long byteSize, const void* data );
	CRecordPlayer( const CRecorder& recorder );
	CRecordPlayer( const char* url );
	CRecordPlayer();
	~CRecordPlayer();

	/**
	* @brief : 开始播放和显示
	*/
	int StartPlay();

	/**
	* @brief : 播放音频的回调函数
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