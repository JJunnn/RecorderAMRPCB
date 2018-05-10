//
//  JJAudioAmr.c
//  RecorderPCB
//
//  Created by AG on 2018/5/10.
//  Copyright © 2018年 JJ. All rights reserved.
//

#include "JJAudioAmr.h"
#include "interf_dec.h"
#include "interf_enc.h"
#include <string.h>
#include <stdlib.h>





void createAMRFile(FILE** fpamr, const char* fileName)
{
    OSStatus status = 0;
    // 创建并初始化amr文件
    *fpamr = fopen(fileName, "wb+");
    if (*fpamr) {

        fseek(*fpamr, 0, SEEK_SET);
        /* write magic number to indicate single channel AMR file storage format */
        fwrite(AMR_MAGIC_NUMBER, sizeof(char), strlen(AMR_MAGIC_NUMBER), *fpamr);

        status = 1;

    } else {

        *fpamr = NULL;

    }


}

//初始化ARM文件
OSStatus InitARMFile(const char* filePath, RecorderFile* recordFile)
{
    //创建文件
    createAMRFile(&(recordFile->fpamr), filePath);
    if (!recordFile->fpamr) {
        recordFile->fpamr = NULL;
        return 2;
    }
    
    //初始化amr库
    recordFile->enstate =  Encoder_Interface_init(0);
    if (!recordFile->enstate) {
        fclose(recordFile->fpamr);
        recordFile->fpamr = NULL;
        return 3;
    }
    
    return 0;
}


void encodeBuffer(RecorderFile* recordFile, short *buf)
{

    //模式
    enum Mode req_mode = MR475;
    //初始化编码后的缓存大小
    unsigned char encodeBuf[PCM_FRAME_SIZE * 2];
    memset(encodeBuf, 0, sizeof(encodeBuf));
    //初始化编码后的大小
    int frameLen =0;
    frameLen = Encoder_Interface_Encode(recordFile->enstate, req_mode, buf, encodeBuf, 0);

    //把编码后的数据写入到amr文件中
    size_t ilen = 0;
    if (0 != recordFile->fpamr) {
        ilen = fwrite(encodeBuf, 1, frameLen, recordFile->fpamr);
    }

}


/**
 * inUseCache 是否使用缓存
 * nNumBytes 字节数量
 * inBuffer 数据
 */
OSStatus AMRAudioFileWritePackets(RecorderFile* recordFile, UInt32 nNumBytes, UInt32 *ioNumPackets,void *const inBuffer)
{
    //录音后的数据，都会经过此方法处理
    if (*ioNumPackets > 0) {

        for (int i = 0; i < nNumBytes ; i += PCM_FRAME_SIZE * 2) {

            short* pPacket = (short*)(((unsigned char*)inBuffer) + i);

            const short par = 2;
            for (int j=0; j<PCM_FRAME_SIZE; j++) {
                if (pPacket[j]<(0x7FFF/par) && pPacket[j]>(-0x7FFF/par)) {
                    if (pPacket[j] > 0x7FFF/2) {
                        pPacket[j] = 0x7FFF-1;
                    } else if (pPacket[j] < -0x7FFF/2) {
                        pPacket[j] = -0x7FFF+1;
                    } else {
                        pPacket[j] = pPacket[j]*par;
                    }
                }
            }
              //进行编解码
            encodeBuffer(recordFile, pPacket);
        }

    }

    return 0;
}


//销毁
void  destroyARM(RecorderFile* recordFile)
{
    Encoder_Interface_exit(recordFile->enstate);
    fclose(recordFile->fpamr);
    recordFile->fpamr = NULL;
}





//void test()
//{
//
//    const char *filePath = "tmp";
//    CFURLRef audioFileURL =
//    CFURLCreateFromFileSystemRepresentation (            // 1
//                                             NULL,                                            // 2
//                                             (const UInt8 *) filePath,                        // 3
//                                             strlen (filePath),                               // 4
//                                             false                                            // 5
//                                             );
//
//
//
//
//    CFStringRef s = CFURLCopyFileSystemPath(audioFileURL,kCFURLPOSIXPathStyle);
//
//
//
//    char buffer[4097];
//    memset( buffer, 0, 1025 );
//    CFStringGetCString(s, buffer, 4096,kCFStringEncodingUTF8);
//
//    const char *ss =  CFStringGetCStringPtr(s, kCFStringEncodingUTF8);
//
//    CFRelease(audioFileURL);
//
//
//    printf("%s",buffer);
//
//    CFRelease(s);
//
//
//}
//OSStatus AMRAudioFileWritePackets(RecorderFile* recordFile, UInt32 nNumBytes, SInt64 inStartingPacket, UInt32 *ioNumPackets,void *const inBuffer)
//{
//    //录音后的数据，都会经过此方法处理
//    if (*ioNumPackets > 0) {
//
//        for (int i = 0; i < nNumBytes ; i += PCM_FRAME_SIZE * 2) {
//
//            short* pPacket = (short*)(((unsigned char*)inBuffer) + i);
//
//            const short par = 2;
//            for (int j=0; j<PCM_FRAME_SIZE; j++) {
//                if (pPacket[j]<(0x7FFF/par) && pPacket[j]>(-0x7FFF/par)) {
//                    if (pPacket[j] > 0x7FFF/2) {
//                        pPacket[j] = 0x7FFF-1;
//                    } else if (pPacket[j] < -0x7FFF/2) {
//                        pPacket[j] = -0x7FFF+1;
//                    } else {
//                        pPacket[j] = pPacket[j]*par;
//                    }
//                }
//            }
//              //进行编解码
//            encodeBuffer(recordFile, pPacket);
//        }
//
//    }
//
//    return 0;
//}







