#include "record.h"
#include <stdio.h>
#include <string>

RecordeInfo CRecorder::m_Info;

CRecorder::CRecorder()
{
	m_Data.frameIndex			= 0;
	m_Data.maxFrameIndex		= 0;
	m_Data.recordedSamples		= NULL;
	m_Data.totalBytes			= 0;

	m_Info.channels				= 0;
	m_Info.sampleType			= 0;
	m_Info.sampleRate			= 0;
	m_Info.seconds				= 0;
	m_Info.framesPerBuffer		= 0;

	m_Stream					= NULL;
}

CRecorder::~CRecorder()
{
	if( m_Data.recordedSamples != NULL )
	{
		free( m_Data.recordedSamples );
		m_Data.recordedSamples = NULL;
	}
}

CRecorder::CRecorder( const RecordeInfo& info )
{
	m_Stream					= NULL;
	m_Data.recordedSamples		= NULL;
	this->SetInfo( info );
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
PaSampleFormat CRecorder::GetSampleFormat()
{
	switch( m_Info.sampleType )
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

//getter and setter
void CRecorder::SetInfo( const RecordeInfo& info )
{
	m_Info = info;
	m_Data.frameIndex			= 0;
	m_Data.maxFrameIndex		= info.sampleRate * info.seconds;

	if( m_Data.recordedSamples != NULL )
	{
		free( m_Data.recordedSamples );
		m_Data.recordedSamples = NULL;
	}

	switch( m_Info.sampleType )
	{
	case SAMPLE_TYPE_FLOAT32:
		m_Data.totalBytes	   = m_Data.maxFrameIndex * m_Info.channels * sizeof( float );
		m_Data.recordedSamples = (float*)malloc( m_Data.totalBytes );
		break;
	case SAMPLE_TYPE_INT16:
		m_Data.totalBytes	   = m_Data.maxFrameIndex * m_Info.channels * sizeof( short );
		m_Data.recordedSamples = (short*)malloc( m_Data.totalBytes );
		break;
	case SAMPLE_TYPE_INT8:
		m_Data.totalBytes	   = m_Data.maxFrameIndex * m_Info.channels * sizeof( char );
		m_Data.recordedSamples = (char*)malloc( m_Data.totalBytes );
		break;
	case SAMPLE_TYPE_UINT8:
		m_Data.totalBytes	   = m_Data.maxFrameIndex * m_Info.channels * sizeof( unsigned char );
		m_Data.recordedSamples = (char*)malloc( m_Data.totalBytes );
		break;
	default:
		m_Data.totalBytes	   = m_Data.maxFrameIndex * m_Info.channels * sizeof( float );
		m_Data.recordedSamples = (float*)malloc( m_Data.totalBytes );
		break;
	}
}

RecordeInfo CRecorder::GetInfo()const
{
	return m_Info;
}


/*******************************
** FunctionName：StartRecord
** Comment:     根据参数，进行录音
** return：     错误标志位，0表示正确
** Creator：    HW
** Date：		2015-8-14
** Modifier：	
** ModifyDate： 
** Version：    1.0.0
*******************************/
int CRecorder::StartRecord()
{
	PaStreamParameters		inputParameters;
	PaError					err = paNoError;

	printf( "Record Init\n" );fflush(stdout);
	
	//PortAudio Init
	err = Pa_Initialize();					//设备初始化
	if( err != paNoError ) return err;

	//设置输入设备参数
	inputParameters.device = Pa_GetDefaultInputDevice();	//获取默认的输入设备
	if( inputParameters.device == paNoDevice )
	{
		fprintf( stderr, "Error:No default input device.\n" );
		return -1;
	}

	inputParameters.channelCount = m_Info.channels;
	inputParameters.sampleFormat = this->GetSampleFormat();
	inputParameters.suggestedLatency =
		Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
	inputParameters.hostApiSpecificStreamInfo = NULL;

	//指定相应的录音回调函数
	static auto pFunc = recordCallback32Bits;

	switch( m_Info.sampleType )
	{
	case SAMPLE_TYPE_FLOAT32:
		pFunc = recordCallback32Bits;
		break;
	case SAMPLE_TYPE_INT16:
		break;
	case SAMPLE_TYPE_INT8:
		break;
	case SAMPLE_TYPE_UINT8:
		break;
	default:
		break;
	}

	//打开音频流，进行准备录音
	err = Pa_OpenStream( &m_Stream,
		&inputParameters,
		NULL,	//no output
		m_Info.sampleRate,
		m_Info.framesPerBuffer,
		paClipOff,
		pFunc,
		&m_Data
		);

	//开始录音
	err = Pa_StartStream( m_Stream );
	if( err != paNoError )
		return err;
	printf("\n=== Now recording!! Please speak into the microphone. ===\n"); fflush(stdout);

	//是否还在录音
	while( ( err = Pa_IsStreamActive( m_Stream ) ) == 1 )
	{
		Pa_Sleep(1000);
		printf("index = %d\n", m_Data.frameIndex ); fflush(stdout);
	}
	if( err < 0 )	return err;

	//结束录音
	err = Pa_StopStream( m_Stream );
	if( err < 0 )	return err;

	//* Measure maximum peak amplitude. */
	double max = 0;
	double average = 0.0;
	double val;
	float* data = (float*)m_Data.recordedSamples;
	for( int i=0; i<m_Data.maxFrameIndex * m_Info.channels; i++ )
	{
		val = data[i];
		if( val < 0 ) val = -val; /* ABS */
		if( val > max )
		{
			max = val;
		}
		average += val;
	}

	average = average / (double)m_Data.maxFrameIndex * m_Info.channels;

	printf("sample max amplitude = %lf\n", max );
	printf("sample average = %lf\n", average );

	//关闭录音设备
	Pa_Terminate();

	return 0;
}

/*******************************
** FunctionName：SavePcm2File
** Comment:     将record中的数据保存到文件里
**（参数）@Param inFile: 要保存的文件名
** return：    
** Creator：    HW
** Date：		2015-8-14
** Modifier：	
** ModifyDate： 
** Version：    1.0.0
*******************************/
void CRecorder::SavePcm2File( const char* inFile )
{
	FILE* file = fopen( inFile, "wb" );
	if( file == NULL )
	{
		fprintf( stderr, "Error: Cannot open the file.\n" );
		return;
	}
	if( m_Data.recordedSamples != NULL )
	{
		printf( "%d bytes save in %s.\n", m_Data.totalBytes, inFile );
		fwrite( m_Data.recordedSamples, 1, m_Data.totalBytes, file );
	}
	else
	{
		fprintf( stderr, "Error:no data can be saved.\n" );
	}

	fclose( file );
}

/*******************************
**（函数名）recordCallback32Bits
**（参数）@Param inputBuffer: 输入流
**（参数）@Param outputBuffer：输出流
**（参数）@Param framesPerBuffer：缓冲区大小
**（参数）@Param timeInfo： 
**（参数）@Param statusFlags：
**（参数）@Param userData： 自定义数据
**（功能用途）：录音的回调函数
**（返回值）return：
** (作者)Creator：HW
** (日期)Date：2015-08-14
**（修改人）Modifier：
**（修改日期）ModifyDate：
**（版本）Version：
*******************************/
int CRecorder::recordCallback32Bits(const void *inputBuffer, 
	void *outputBuffer, unsigned long framesPerBuffer, 
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags, void *userData)
{
	RecordData* data = (RecordData*)userData;
	
	(void) outputBuffer; /* Prevent unused variable warnings. */
	(void) timeInfo;
	(void) statusFlags;
	(void) userData;

	if( m_Info.sampleType == SAMPLE_TYPE_FLOAT32 )
	{
		float* buffer = (float*)data->recordedSamples;
		float* wptr = &buffer[ data->frameIndex * m_Info.channels ];
		return _DoRecord( data, wptr,inputBuffer, 0.0f );
	}

	else if( m_Info.sampleType == SAMPLE_TYPE_INT16 )
	{
		short* buffer = (short*)data->recordedSamples;
		short* wptr = &buffer[ data->frameIndex * m_Info.channels ];
		return _DoRecord( data, wptr,inputBuffer, 0.0f );
	}

	else if( m_Info.sampleType == SAMPLE_TYPE_INT8 )
	{
		char* buffer = (char*)data->recordedSamples;
		char* wptr = &buffer[ data->frameIndex * m_Info.channels ];
		return _DoRecord( data, wptr,inputBuffer, 0.0f );
	}

	else if( m_Info.sampleType == SAMPLE_TYPE_UINT8 )
	{
		unsigned char* buffer = (unsigned char*)data->recordedSamples;
		unsigned char* wptr = &buffer[ data->frameIndex * m_Info.channels ];
		return _DoRecord( data, wptr,inputBuffer, 128 );
	}

	return 1;
}

/*******************************
**（函数名）_DoRecord
**（参数）@Param data: 自定义数据
**（参数）@Param wptr：录音保存的buffer的指针
**（参数）@Param rptr：音频输入的指针
**（参数）@Param silence： 静音的值
**（功能用途）：录音的回调函数
**（返回值）return：
** (作者)Creator：HW
** (日期)Date：2015-08-14
**（修改人）Modifier：
**（修改日期）ModifyDate：
**（版本）Version：
*******************************/
template <typename T>
int CRecorder::_DoRecord( RecordData* data, T wptr,const void* inputBuffer, double silence )
{
	long framesToCalc;
	long i;
	int finished;
	unsigned long framesLeft = data->maxFrameIndex - data->frameIndex;

	const float* rptr = (const float*)inputBuffer;
	if( framesLeft < m_Info.framesPerBuffer )
	{
		framesToCalc = framesLeft;
		finished = paComplete;
	}
	else
	{
		framesToCalc = m_Info.framesPerBuffer;
		finished = paContinue;
	}

	if( inputBuffer == NULL )
	{
		for( i=0; i<framesToCalc * m_Info.channels; i++ )
			*wptr++ = silence;
	}
	else
	{
		for( i=0; i<framesToCalc * m_Info.channels; i++ )
		{
			*wptr++ = *rptr++;  /* left */
		}
	}
	data->frameIndex += framesToCalc;
	return finished;
}