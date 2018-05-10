//
//  ViewController.m
//  RecorderPCB
//
//  Created by AG on 2018/5/9.
//  Copyright © 2018 JJ. All rights reserved.
//

#import "ViewController.h"
#import "JJRecorderView.h"
#import "JJAudioRecorder.h"
#import "EMAudioPlayer.h"


@interface ViewController () <EMAudioPlayerDelegate>

@property (nonatomic, strong) JJRecorderView *mainView;
@property (nonatomic, strong) JJAudioRecorder* audioRecorder;
@property (nonatomic, strong) EMAudioPlayer* play;

@property (nonatomic, strong) NSString *playPath;

@end

@implementation ViewController

- (void)audioPlayerDidFinishPlaying:(EMAudioPlayer*)player successfully:(BOOL)flag
{
    
    NSLog(@"播放完成");
}




- (void)loadView
{
    [self setMainView:[[JJRecorderView alloc] initWithFrame:[[UIScreen mainScreen] bounds]]];
    [self setView:_mainView];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.

    
    [self setPlay:[[EMAudioPlayer alloc] init]];
    [_play setDelegate:self];
    
    [self setAudioRecorder:[[JJAudioRecorder alloc] init]];
    
    
    __weak typeof(self) weakself = self;
    
    [_audioRecorder setBlockRecorderPath:^(NSString *path) {
        weakself.playPath = path;
    }];
    
    
    
    [_mainView setBlockRecorderView:^(NSInteger tag) {
        
        
        switch (tag) {
            case 130:{
                [weakself.audioRecorder start];
//                [[self recorder] startRecorder];
            }
                break;
                
            case 131:{
                 [weakself.audioRecorder stop];
//                [[self recorder] stopRecorder];
            }
                break;
                
            case 132:{
            
//                pause1(&cc);
//                [[self recorder] ];
            }
                break;
                
            case 133:{
                [weakself.play setFilePath:weakself.playPath];
                [weakself.play startPlayer];

            }
                break;
                
            case 134:{
                [weakself.play stopPlayer];
            }
                break;
                
            case 135:{
                
            }
                break;

        }
        
    }];
    
    
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}






@end
