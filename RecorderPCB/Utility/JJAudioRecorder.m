//
//  JJAudioRecorder.m
//  RecorderPCB
//
//  Created by AG on 2018/5/10.
//  Copyright © 2018年 JJ. All rights reserved.
//

#import "JJAudioRecorder.h"
#include "JJAudioQueueRecorder.h"

@interface JJAudioRecorder()
{
    AQRecorderState aqData;
}

@property (nonatomic, strong) NSString *filePath;

@end

@implementation JJAudioRecorder

- (instancetype)init
{
    
    if (self = [super init]) {
     
        aqData.mDataFormat.mFormatID         = kAudioFormatLinearPCM; // 2
        aqData.mDataFormat.mSampleRate       = 8000;               // 3
        aqData.mDataFormat.mChannelsPerFrame = 1;                     // 4
        aqData.mDataFormat.mBitsPerChannel   = 16;                    // 5
        aqData.mDataFormat.mBytesPerPacket   =                        // 6
        aqData.mDataFormat.mBytesPerFrame =
        aqData.mDataFormat.mChannelsPerFrame * sizeof (SInt16);
        aqData.mDataFormat.mFramesPerPacket  = 1;                     // 7
        aqData.mDataFormat.mFormatFlags =                             // 9
        kLinearPCMFormatFlagIsSignedInteger
        | kLinearPCMFormatFlagIsPacked;
        aqData.mIsRunning = false;
        
    }
    
    return self;
}

- (void)start
{
    
    if (aqData.mIsRunning) {
        return;
    }

    createAudioQueue(&aqData);
    
    self.filePath = [self recorderFilePathWithFileName:@"abc.amr"];
    InitARMFile([self.filePath UTF8String], &(aqData.mAudioFile));
    start(&aqData);
    
}

- (void)stop
{
    if (!aqData.mIsRunning) {
        return ;
    }
    stop(&aqData);
    
    _blockRecorderPath ? _blockRecorderPath(self.filePath) : nil;
    
}



//创建录音所在的路径
- (NSString*)recorderFilePathWithFileName:(NSString*)fileName
{
    NSString* filePath = [NSTemporaryDirectory() stringByAppendingString:fileName];
    [[NSFileManager defaultManager] removeItemAtPath:filePath error:nil];
    return filePath;
    
}



/*
- (AQRecorderState*)InitRecorderState
{
    
    AQRecorderState* aqData = (AQRecorderState*)malloc(sizeof(AQRecorderState));                                       // 1
    aqData->mDataFormat.mFormatID         = kAudioFormatLinearPCM; // 2
    aqData->mDataFormat.mSampleRate       = 8000;               // 3
    aqData->mDataFormat.mChannelsPerFrame = 1;                     // 4
    aqData->mDataFormat.mBitsPerChannel   = 16;                    // 5
    aqData->mDataFormat.mBytesPerPacket   =                        // 6
    aqData->mDataFormat.mBytesPerFrame =
    aqData->mDataFormat.mChannelsPerFrame * sizeof (SInt16);
    aqData->mDataFormat.mFramesPerPacket  = 1;                     // 7
    aqData->mDataFormat.mFormatFlags =                             // 9
    kLinearPCMFormatFlagIsSignedInteger
    | kLinearPCMFormatFlagIsPacked;
    
    InitARMFile( [[self recorderFilePathWithFileName:@"abc.amr"] UTF8String], &(aqData->mAudioFile));
    
    
    return aqData;
}
*/
@end
