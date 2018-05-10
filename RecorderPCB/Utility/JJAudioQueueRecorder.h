//
//  JJAudioQueueRecorder.h
//  RecorderPCB
//
//  Created by AG on 2018/5/10.
//  Copyright © 2018年 JJ. All rights reserved.
//

#ifndef JJAudioQueueRecorder_h
#define JJAudioQueueRecorder_h

#if !defined(__COREAUDIO_USE_FLAT_INCLUDES__)
#include <AudioToolbox/AudioFile.h>
#include <AudioToolbox/AudioFileStream.h>
#include <AudioToolbox/AudioFormat.h>
#include <AudioToolbox/AudioQueue.h>
#include <AudioToolbox/AudioServices.h>
#include <AudioToolbox/AUGraph.h>
#include <AudioToolbox/AudioConverter.h>
#include <AudioToolbox/ExtendedAudioFile.h>
#include <AudioToolbox/MusicPlayer.h>
#include <AudioToolbox/CAFFile.h>
#if !TARGET_OS_IPHONE
#include <AudioToolbox/AudioFileComponent.h>
#include <AudioToolbox/AudioUnitUtilities.h>
#include <AudioToolbox/AUMIDIController.h>
#include <AudioToolbox/CoreAudioClock.h>
#endif
#else
#include <AudioConverter.h>
#include <AudioFile.h>
#include <AudioFileComponent.h>
#include <AudioFileStream.h>
#include <AudioFormat.h>
#include <AudioQueue.h>
#include <AudioUnitUtilities.h>
#include <AUGraph.h>
#include <AUMIDIController.h>
#include <CAFFile.h>
#include <CoreAudioClock.h>
#include <ExtendedAudioFile.h>
#include <MusicPlayer.h>
#include <AudioServices.h>
#endif

#include "JJAudioAmr.h"

static const int kNumberBuffers = 3; //设置音频队列缓冲区的数量

typedef struct {
    
    bool mIsRunning; //表示音频队列是否在运行
    UInt32 bufferByteSize; //每个音频队列缓冲区的大小（以字节为单位）
    SInt64 mCurrentPacket; // 当前的音频队列缓冲区写入第一个包的包索引。
    
    AudioStreamBasicDescription mDataFormat; // 音频数据格式
    AudioQueueRef mQueue; //音频队列
    AudioQueueBufferRef mBuffers[kNumberBuffers]; //存放指向由音频队列管理的缓冲区指针
    RecorderFile mAudioFile; //音频数据在磁盘中的位置，以文件形式存在，该参数就是文件的句柄。
    
    
} AQRecorderState; //定义该结构表示音频队列的状态

void createAudioQueue(AQRecorderState* recorderState);
OSStatus start(AQRecorderState* recorderState);
OSStatus stop(AQRecorderState* recorderState);
OSStatus pause1(AQRecorderState* recorderState);
float peakPower(AQRecorderState* recorderState);
double currentTime(AQRecorderState* recorderState);
//是否开启监听分贝
void meteringEnabled(AQRecorderState* recorderState, UInt32 boolValue);



#endif /* JJAudioQueueRecorder_h */
