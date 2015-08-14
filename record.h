#pragma once

/*******************************
**FileName	： record.h
**Creator	： HW
**Date		： 2015-08-14
**Modifier	： 
**ModifyDate： 
**Comment	： 录音类
**Version	： 1.0.0
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
	* @brief : 根据sample type获得format
	*/
	PaSampleFormat GetSampleFormat();

	template<class T>
	static int _DoRecord( RecordData* data, T wptr,const void* inputBuffer, double silence );
	template<class T>
	static int __DoRecord( void* userData,const void* inputBuffer, double silence, T type );

private:
	static RecordeInfo m_Info;
	RecordData  m_Data;
	PaStream*	m_Stream;

public:
	CRecorder();
	CRecorder( const RecordeInfo& info );
	~CRecorder();

	/**
	* @brief : 设置录音参数
	*/
	void SetInfo( const RecordeInfo& info );

		/**
	* @brief : 获取录音参数
	*/
	RecordeInfo GetInfo()const;
	void* GetData()const{ return m_Data.recordedSamples; }
	unsigned long GetDataSize()const { return m_Data.totalBytes; }

	/**
	* @brief : 根据录音参数，进行录音
	*/
	int StartRecord();

	/**
	* @brief : 将录音的数据保存为一个文件
	*/
	void SavePcm2File( const char* inFile );

	/**
	* @brief :录音的回调函数 
	*/
	static int recordCallback(
		const void *inputBuffer, void *outputBuffer,
		unsigned long framesPerBuffer,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags,
		void *userData);
};