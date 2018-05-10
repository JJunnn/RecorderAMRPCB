

#ifndef EMAudioQueuePlayer_hpp
#define EMAudioQueuePlayer_hpp

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

#include "interf_dec.h"
#include "interf_enc.h"
#include <string>

#define AMR_MAGIC_NUMBER "#!AMR\n"
#define PCM_FRAME_SIZE 160 // 8khz 8000*0.02=160

static const int kPlayerNumberBuffers = 3;

typedef void (*AudioQueueCompleteCallback)(void* inUserData,UInt32 boolValue);


class EMAudioQueuePlayer{
    
public:
    EMAudioQueuePlayer(AudioStreamBasicDescription mDataFormat, std::string amrPath,AudioQueueCompleteCallback completeCallBack, void* inUserData);
    ~EMAudioQueuePlayer();
    
public:
    OSStatus start();                                           //开始播放
    OSStatus stop();                                            //停止播放
    OSStatus pause();                                           //暂停播放
    UInt32 readPacketsIntoBuffer(AudioQueueBufferRef buffer);   //读取磁盘上的数据放在缓存队列中进行播放，同时此方法进行解码
    void speechPath(std::string pathName);                      //要播放的语音路径
    void complete(UInt32 boolValue);
    
    
public: //get方法
    AudioStreamBasicDescription getMDataFormat();
    AudioQueueRef getMQueue();
    AudioFileID getMAudioFile();
    UInt32 getBufferByteSize();
    SInt64 getMCurrentPacket();
    UInt32 getMNumPacketsToRead();
    bool getMIsRunning();
    FILE* getFpamr();
    void* getEnstate();
    std::string getAmrPath();

public://set方法
    void setMCurrentPacket(SInt64 mCurrentPackete);
    void setMIsRunning(bool mIsRunning);
    void setAmrPath(std::string amrPath);
    
    
private://私有方法
    OSStatus allocBuffer();
    OSStatus openFile(std::string fileName);
    
private:
    AudioStreamBasicDescription  _mDataFormat;                   // 2
    AudioQueueRef                _mQueue;                        // 3
    AudioQueueBufferRef          _mBuffers[kPlayerNumberBuffers];      // 4
    AudioFileID                  _mAudioFile;                    // 5
    UInt32                       _bufferByteSize;                // 6 //缓存的大小
    SInt64                       _mCurrentPacket;                // 7 //当前包为8424
    bool                         _mIsRunning;                    // 8
    UInt32                       _mNumPacketsToRead;             // 9
    AudioStreamPacketDescription* _mPacketDescs;                 // 10
    std::string                  _amrPath;
    FILE*                        _fpamr;
    void*                        _enstate;
    AudioQueueCompleteCallback   _completeCallBack;              // 完成的回调函数
    void*                        _inUserData;                    // 调用者的信息
    
};


#endif /* EMAudioQueuePlayer_hpp */
