//
//  JJAudioRecorder.h
//  RecorderPCB
//
//  Created by AG on 2018/5/10.
//  Copyright © 2018年 JJ. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef void(^blockRecorderPath)(NSString*path);

@interface JJAudioRecorder : NSObject

@property (nonatomic, copy) blockRecorderPath blockRecorderPath;

- (void)start;
- (void)stop;

@end
