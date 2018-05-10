//
//  JJAudioQueueRecorder.c
//  RecorderPCB
//
//  Created by AG on 2018/5/10.
//  Copyright © 2018年 JJ. All rights reserved.
//

#include "JJAudioQueueRecorder.h"


//callback 将缓存区的数据存储到音频文件中，将其内容刚刚写入磁盘的缓冲区加入到缓冲区队列
/**
 * aqData 指向音频队列数据的自定义结构 AQRecorderState
 * inAQ 拥有此回调的音频队列
 * inBuffer 包含要录制的传入音频数据的音频队列缓冲区
 * inStartTime  音频队列缓冲区中第一个采样的采样时间（简单记录不需要）
 * inNUmpackets inPacketDesc参数中的数据包描述数量。值0表示CBR数据
 * inPacketDesc 对于需要数据包描述的压缩音频数据格式，编码器为缓冲区中数据包生成的数据包描述。
 */
static void HandleInputBuffer(void* aqData, AudioQueueRef inAQ, AudioQueueBufferRef inBuffer, const AudioTimeStamp* inStartTime, UInt32 inNumpackets, const AudioStreamPacketDescription* inPacketDesc)
{
   
    AQRecorderState* pAqData = (AQRecorderState*)aqData;
    
    //写入AMR文件中
    AMRAudioFileWritePackets(&pAqData->mAudioFile, inBuffer->mAudioDataByteSize, &inNumpackets,inBuffer->mAudioData);
    

    /*
     if (inNumpackets == 0 && pAqData->mDataFormat.mBytesPerPacket != 0 ) {
     //如果音频队列缓冲区包含CBR数据，则计算缓冲区中的数据包数量。该数字等于缓冲区中数据的总字节数除以每个数据包的（恒定）字节数。对于VBR数据，音频队列在调用回调时提供缓冲区中的数据包数量。
     inNumpackets = inBuffer->mAudioDataByteSize / pAqData->mDataFormat.mBytesPerPacket;
     }

    if (AudioFileWritePackets(pAqData->mAudioFile, false, inBuffer->mAudioDataByteSize, inPacketDesc, pAqData->mCurrentPacket, &inNumpackets, inBuffer->mAudioData) == noErr) {
        pAqData->mCurrentPacket += inNumpackets;
    }
     */
    
    
    if (pAqData->mIsRunning == false) {
        return;
    }
    
    AudioQueueEnqueueBuffer(pAqData->mQueue, inBuffer, 0, NULL);
    
}

void allocBuffer(AQRecorderState* recorderState)
{
    recorderState->bufferByteSize = recorderState->mDataFormat.mBitsPerChannel * recorderState->mDataFormat.mChannelsPerFrame * recorderState->mDataFormat.mSampleRate * 0.2 / 8;
    for (int i = 0; i < kNumberBuffers; ++i) {
        AudioQueueAllocateBuffer(recorderState->mQueue, recorderState->bufferByteSize, &recorderState->mBuffers[i]);
    }
}


void createAudioQueue(AQRecorderState* recorderState)
{

    // 创建录音队列
    AudioQueueNewInput(&recorderState->mDataFormat, HandleInputBuffer, recorderState, NULL, kCFRunLoopCommonModes, 0, &recorderState->mQueue);
    
    allocBuffer(recorderState);
}



/**
 * @breif 开启录音
 */
OSStatus start(AQRecorderState* recorderState)
{

    AudioQueueReset(recorderState->mQueue);
    recorderState->mCurrentPacket = 0;
    OSStatus status = 0;
    
    recorderState->mIsRunning = true;
    status = AudioQueueStart(recorderState->mQueue, NULL);
    for (int i = 0; i < kNumberBuffers; ++i) {
        AudioQueueEnqueueBuffer(recorderState->mQueue, recorderState->mBuffers[i], 0, NULL);
    }

    return status;
}

/**
 * @breif 停止录音
 */
OSStatus stop(AQRecorderState* recorderState)
{
    
    OSStatus status = AudioQueueStop(recorderState->mQueue, true);
    // 表示停止录音
    recorderState->mIsRunning = false;
    
    /**
     * 该AudioQueueDispose函数处理音频队列及其所有资源，包括其缓冲区。
     * 您想要处理的音频队列。
     * 用于true同步处理音频队列（即立即）。
     */
    AudioQueueDispose(recorderState->mQueue, true);
    
    destroyARM(&recorderState->mAudioFile);
    
    return status;
}

/**
 * @breif 暂停录音
 */
OSStatus pause1(AQRecorderState* recorderState)
{
    // 表示停止录音
    recorderState->mIsRunning = false;
    return AudioQueuePause(recorderState->mQueue);
}


float peakPower(AQRecorderState* recorderState)
{
    
    UInt32 ioDataSize = sizeof(AudioQueueLevelMeterState) * recorderState->mDataFormat.mChannelsPerFrame;
    AudioQueueLevelMeterState* meterState = (AudioQueueLevelMeterState*)malloc(ioDataSize);
    AudioQueueGetProperty(recorderState->mQueue, kAudioQueueProperty_CurrentLevelMeter, meterState, &ioDataSize);
    
    float channelAvg = 0;
    for (int i = 0; i < recorderState->mDataFormat.mChannelsPerFrame; i++) {
        channelAvg += meterState[i].mPeakPower;
    }
    free(meterState);
    
    return channelAvg;
    
}

double currentTime(AQRecorderState* recorderState)
{
    
    double timeInterval = 0;
    AudioQueueTimelineRef timeLine;
    OSStatus status = AudioQueueCreateTimeline(recorderState->mQueue, &timeLine);
    if(status == noErr) {
        AudioTimeStamp timeStamp;
        AudioQueueGetCurrentTime(recorderState->mQueue, timeLine, &timeStamp, NULL);
        timeInterval = timeStamp.mSampleTime / recorderState->mDataFormat.mSampleRate; // modified
    }
    return timeInterval;
    
}

//是否开启监听分贝
void meteringEnabled(AQRecorderState* recorderState, UInt32 boolValue)
{
    AudioQueueSetProperty(recorderState->mQueue, kAudioQueueProperty_EnableLevelMetering, &boolValue, sizeof(UInt32));
}










