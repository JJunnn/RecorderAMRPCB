

#include "EMAudioQueuePlayer.h"

const unsigned int PERREADFRAME =  10;//每次读取帧数

static void handleOutputBuffer(void *aqData, AudioQueueRef inAQ, AudioQueueBufferRef  inBuffer)
{
    
    //
    EMAudioQueuePlayer* pAqData = (EMAudioQueuePlayer*)aqData;
    
    if (pAqData->getMIsRunning() == 0) {
        
        pAqData->stop();
        pAqData->complete(true);
        
    } else{
        UInt32 readAMRFrame = 0;
        readAMRFrame = pAqData->readPacketsIntoBuffer(inBuffer);
        readAMRFrame ? 0 : pAqData->stop();
    }
}


void DeriveBufferSize (
                       AudioQueueRef                audioQueue,
                       AudioStreamBasicDescription  &ASBDescription,
                       Float64                      seconds,
                       UInt32                       *outBufferSize
                       ) {
    static const int maxBufferSize = 0x50000;
    
    int maxPacketSize = ASBDescription.mBytesPerPacket;
    if (maxPacketSize == 0) {
        UInt32 maxVBRPacketSize = sizeof(maxPacketSize);
        AudioQueueGetProperty (
                               audioQueue,
                               kAudioQueueProperty_MaximumOutputPacketSize,
                               &maxPacketSize,
                               &maxVBRPacketSize
                               );
    }
    
    Float64 numBytesForTime =
    ASBDescription.mSampleRate * maxPacketSize * seconds;
    *outBufferSize =
    UInt32 (numBytesForTime < maxBufferSize ?
            numBytesForTime : maxBufferSize);
}


UInt32 EMAudioQueuePlayer::readPacketsIntoBuffer(AudioQueueBufferRef inBuffer)
{
    //-----
    short pcmBuf[1600] = {0};//KSECONDSPERBUFFER * 160 * 50;
    memset(pcmBuf, 0, sizeof(pcmBuf));
    UInt32 readAMRFrame = 0;
    const short block_size[16] = { 12, 13, 15, 17, 19, 20, 26, 31, 5, 0, 0, 0, 0, 0, 0, 0 };
    char analysis[32]={0};

    size_t rCout=0;
    // while (fread(analysis, sizeof (unsigned char), 1, file_analysis ) > 0)
    while (readAMRFrame < PERREADFRAME && (rCout = fread(analysis, sizeof(unsigned char), 1, _fpamr))) {
        int dec_mode = (analysis[0] >> 3) & 0x000F;
        int read_size = block_size[dec_mode];
        _mCurrentPacket += rCout;
        rCout = fread(&analysis[1], sizeof(char), read_size, _fpamr);
        _mCurrentPacket += rCout;
        
        Decoder_Interface_Decode(_enstate, (unsigned char *)analysis, &pcmBuf[readAMRFrame * 160], 0);
        readAMRFrame++;
    }
    
    if (readAMRFrame > 0) {
        inBuffer->mAudioDataByteSize = readAMRFrame * 2 * 160;
        inBuffer->mPacketDescriptionCount = readAMRFrame * 160;
        memcpy(inBuffer->mAudioData, pcmBuf, readAMRFrame * 160 * 2);
        AudioQueueEnqueueBuffer(_mQueue, inBuffer, 0, NULL);
        
    } else {
        Decoder_Interface_exit(_enstate);
    }
    
    
    
    return readAMRFrame;
}

//构造函数
EMAudioQueuePlayer::EMAudioQueuePlayer(AudioStreamBasicDescription mDataFormat, std::string amrPath,AudioQueueCompleteCallback completeCallBack, void* inUserData):_mDataFormat(mDataFormat),_amrPath(amrPath),_completeCallBack(completeCallBack),_inUserData(inUserData)
{
    //初始化 当包的索引为0
    _mCurrentPacket = 0;
    _mIsRunning = false;
    _fpamr = nullptr;
    
    AudioQueueNewOutput(&_mDataFormat, handleOutputBuffer, this, NULL, NULL, 0, &_mQueue);//kCFRunLoopCommonModes
   _bufferByteSize = _mDataFormat.mBitsPerChannel * _mDataFormat.mChannelsPerFrame * _mDataFormat.mSampleRate * 0.2 / 8;
    for (int i = 0; i < kPlayerNumberBuffers; ++i) {
        AudioQueueAllocateBuffer(_mQueue, _bufferByteSize, &_mBuffers[i]);
    }
    
}

//析构函数
EMAudioQueuePlayer::~EMAudioQueuePlayer()
{
    AudioQueueDispose(_mQueue, true);
}

/**
 * @breif 开启播放
 */
OSStatus EMAudioQueuePlayer::start()
{
    
    OSStatus status = 0;
    // 创建缓存并往缓存里注入数据
    if (!openFile(_amrPath)) {
        
        //初始化 arm编码器
        _enstate = Decoder_Interface_init();
        
        status = AudioQueueStart(_mQueue, NULL);
        allocBuffer();
        // 是否运营 赋值true表示播放正在进行
        _mIsRunning = true;
    }
    
    return status;
}

/**
 * @breif 停止播放
 */
OSStatus EMAudioQueuePlayer::stop()
{
    // 表示停止录音
    OSStatus status = AudioQueueStop(_mQueue, false);
    if (_fpamr) {
        fclose(_fpamr);
        _fpamr = nullptr;
    }
    _mIsRunning = false;

    return status;
}

/**
 * @breif 暂停录音
 */
OSStatus EMAudioQueuePlayer::pause()
{
    // 表示停止录音
    _mIsRunning = false;
    if (_fpamr) {
        fclose(_fpamr);
        _fpamr = nullptr;
    }
    return AudioQueuePause(_mQueue);
}

void EMAudioQueuePlayer::speechPath(std::string pathName)
{
    _amrPath = pathName;
}

void EMAudioQueuePlayer::complete(UInt32 boolValue)
{
    _completeCallBack(_inUserData,boolValue);
}


AudioStreamBasicDescription EMAudioQueuePlayer::getMDataFormat()
{
    return _mDataFormat;
}

AudioQueueRef EMAudioQueuePlayer::getMQueue()
{
    return _mQueue;
}

AudioFileID EMAudioQueuePlayer::getMAudioFile()
{
    return _mAudioFile;
}

UInt32 EMAudioQueuePlayer::getBufferByteSize()
{
    return _bufferByteSize;
}

SInt64 EMAudioQueuePlayer::getMCurrentPacket()
{
    return _mCurrentPacket;
}

UInt32 EMAudioQueuePlayer::getMNumPacketsToRead()
{
    return _mNumPacketsToRead;
}

bool EMAudioQueuePlayer::getMIsRunning()
{
    return _mIsRunning;
}

FILE* EMAudioQueuePlayer::getFpamr()
{
    return _fpamr;
}

void* EMAudioQueuePlayer::getEnstate()
{
    return _enstate;
}

std::string EMAudioQueuePlayer::getAmrPath()
{
    return _amrPath;
}

void EMAudioQueuePlayer::setMCurrentPacket(SInt64 mCurrentPackete)
{
    _mCurrentPacket = mCurrentPackete;
}

void EMAudioQueuePlayer::setMIsRunning(bool mIsRunning)
{
    _mIsRunning = mIsRunning;
}

void EMAudioQueuePlayer::setAmrPath(std::string amrPath)
{
    _amrPath = amrPath;
}

OSStatus EMAudioQueuePlayer::allocBuffer()
{
    _mCurrentPacket = 0;
    OSStatus status = 0;
    status = AudioQueueReset(_mQueue);
    //创建缓存队列
    for (int i = 0; i < kPlayerNumberBuffers; ++i) {
        //读取包数据
        if (this->readPacketsIntoBuffer(_mBuffers[i]) == 0) {
            break;
        }
    }
    

    return status;
}


OSStatus EMAudioQueuePlayer::openFile(std::string fileName)
{
    OSStatus status = 0;
    
    // 创建并初始化amr文件
    _fpamr = fopen(fileName.c_str(), "rb+");
    if (_fpamr) {
        
//        fseek(_fpamr, 0, SEEK_SET);
        /* read magic number to indicate single channel AMR file storage format */
        char magic[8];
        fread(magic, sizeof(char), strlen(AMR_MAGIC_NUMBER), _fpamr);
        
        
    } else {

        status = -1;
        _fpamr = nullptr;
    }
    
    return status;
}






