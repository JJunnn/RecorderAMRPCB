//
//  JJAudioAmr.h
//  RecorderPCB
//
//  Created by AG on 2018/5/10.
//  Copyright © 2018年 JJ. All rights reserved.
//

#ifndef JJAudioAmr_h
#define JJAudioAmr_h

#include <stdio.h>
#include <MacTypes.h>

#include "JJAudioAmr.h"

#define AMR_MAGIC_NUMBER "#!AMR\n"

#define PCM_FRAME_SIZE 160 // 8khz 8000*0.02=160

typedef int OSStatus;

typedef struct {
    FILE* fpamr;                         // 文件句柄
    void* enstate;                       // amr解码的句柄
    
} RecorderFile;

OSStatus InitARMFile(const char* filePath, RecorderFile* recordFile);
OSStatus AMRAudioFileWritePackets(RecorderFile* recordFile, UInt32 nNumBytes, UInt32 *ioNumPackets, void* const inBuffer);
void  destroyARM(RecorderFile* recordFile);

#endif /* JJAudioAmr_h */
