

#import "EMAudioPlayer.h"
#include "EMAudioQueuePlayer.h"

static void palyerComplete(void *aqData,UInt32 boolValue)
{
    EMAudioPlayer* player = (__bridge EMAudioPlayer*)aqData;
    if ([[player delegate] respondsToSelector:@selector(audioPlayerDidFinishPlaying:successfully:)]) {
        [[player delegate] audioPlayerDidFinishPlaying:player successfully:boolValue];
    }
    
}

@interface EMAudioPlayer()
{
    EMAudioQueuePlayer* _aqp;
}

@end

@implementation EMAudioPlayer

- (instancetype)init
{
    if (self = [super init]) {
        _aqp = new EMAudioQueuePlayer([self recorderFormat], "", palyerComplete,(__bridge void*)self);
    }
    
    return self;
}

- (instancetype)initWithFilePath:(NSString*)filePath
{
    if (self = [super init]) {
        [self setFilePath:filePath];
        _aqp = new EMAudioQueuePlayer([self recorderFormat],[[_filePath copy] UTF8String],palyerComplete,(__bridge void*)self);
    }
    return self;
}

- (void)dealloc
{
    delete _aqp;
}

- (void)startPlayer
{
    if (!_aqp->getMIsRunning()) {
        _aqp->start();
    }
}

- (void)stopPlayer
{
    _aqp->stop();
}

- (BOOL)playing
{
    return _aqp->getMIsRunning();
}

#pragma mark - private Methods
//设置编码格式
- (AudioStreamBasicDescription)recorderFormat
{
    AudioStreamBasicDescription mDataFormat;                        // 1
    memset(&mDataFormat, 0, sizeof(mDataFormat));
    mDataFormat.mFormatID         = kAudioFormatLinearPCM;          // 2
    mDataFormat.mSampleRate       = 8000.0f;                        // 3
    mDataFormat.mChannelsPerFrame = 1;                              // 4
    mDataFormat.mBitsPerChannel   = 16;                             // 5
    mDataFormat.mBytesPerPacket   = mDataFormat.mBytesPerFrame = mDataFormat.mBitsPerChannel / 8 * mDataFormat.mChannelsPerFrame;  // 6
    mDataFormat.mFramesPerPacket  = 1;                              // 7
    mDataFormat.mFormatFlags = kLinearPCMFormatFlagIsSignedInteger | kLinearPCMFormatFlagIsPacked;
    
    return mDataFormat;
}

#pragma mark - setter Methods
- (void)setFilePath:(NSString *)filePath
{
    _filePath = filePath;
    _aqp->setAmrPath([_filePath UTF8String]);
}

@end
