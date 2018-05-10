//
//  JJRecorderView.m
//  RecorderPCB
//
//  Created by AG on 2018/5/9.
//  Copyright © 2018 JJ. All rights reserved.
//

#import "JJRecorderView.h"


@interface JJRecorderView()

@property (nonatomic, strong) UIButton *startButton;
@property (nonatomic, strong) UIButton *stopButton;
@property (nonatomic, strong) UIButton *pauseButton;


@property (nonatomic, strong) UIButton *playStartButton;
@property (nonatomic, strong) UIButton *playStopButton;
@property (nonatomic, strong) UIButton *playPauseButton;

@end

@implementation JJRecorderView

- (instancetype)initWithFrame:(CGRect)frame
{
    if (self = [super initWithFrame:frame]) {
        
        [self setBackgroundColor:[UIColor whiteColor]];
        
        [[self startButton] setFrame:CGRectMake(40, 100, 80, 30)];
        [self addSubview:_startButton];
        
        [[self stopButton] setFrame:CGRectMake(125, 100, 80, 30)];
        [self addSubview:_stopButton];
        
        [[self pauseButton] setFrame:CGRectMake(210, 100, 80, 30)];
        [self addSubview:_pauseButton];
        
        [[self playStartButton] setFrame:CGRectMake(40, 160, 80, 30)];
        [self addSubview:_playStartButton];
        
        [[self playStopButton] setFrame:CGRectMake(125, 160, 80, 30)];
        [self addSubview:_playStopButton];
        
        [[self playPauseButton] setFrame:CGRectMake(210, 160, 80, 30)];
        [self addSubview:_playPauseButton];
        
    }
    
    return self;
}


- (void)eventWithButton:(UIButton*)button
{
    _blockRecorderView ? _blockRecorderView(button.tag) : nil;
}


- (UIButton*)startButton
{
    if (!_startButton) {
        _startButton = [UIButton buttonWithType:UIButtonTypeCustom];
        [_startButton setBackgroundColor:[UIColor colorWithRed:0.7 green:0.7 blue:0.71 alpha:1]];
        [_startButton setTitle:@"开始录音" forState:UIControlStateNormal];
        [_startButton setTag:130];
        /*
        //告诉layer将位于它之下的layer都遮盖住
        [_startButton.layer setMasksToBounds:YES];
        //设置layer的圆角,刚好是自身宽度的一半，这样就成了圆形
        [_startButton.layer setCornerRadius:8.0];
        //设置边框的宽度为20
        [_startButton.layer setBorderWidth:1.0];
        //设置边框的颜色
        [_startButton.layer setBorderColor:[UIColor whiteColor].CGColor];
     */
        [_startButton addTarget:self action:@selector(eventWithButton:) forControlEvents:UIControlEventTouchUpInside];
    }
    
    return _startButton;
}

- (UIButton*)stopButton
{
    if (!_stopButton) {
        _stopButton = [UIButton buttonWithType:UIButtonTypeCustom];
        [_stopButton setBackgroundColor:[UIColor colorWithRed:0.7 green:0.7 blue:0.71 alpha:1]];
        [_stopButton setTitle:@"停止录音" forState:UIControlStateNormal];
        [_stopButton setTag:131];
        /*
         //告诉layer将位于它之下的layer都遮盖住
         [_stopButton.layer setMasksToBounds:YES];
         //设置layer的圆角,刚好是自身宽度的一半，这样就成了圆形
         [_stopButton.layer setCornerRadius:8.0];
         //设置边框的宽度为20
         [_stopButton.layer setBorderWidth:1.0];
         //设置边框的颜色
         [_stopButton.layer setBorderColor:[UIColor whiteColor].CGColor];

         */
        [_stopButton addTarget:self action:@selector(eventWithButton:) forControlEvents:UIControlEventTouchUpInside];
    }
    
    return _stopButton;
}

- (UIButton*)pauseButton
{
    if (!_pauseButton) {
        _pauseButton = [UIButton buttonWithType:UIButtonTypeCustom];
        [_pauseButton setBackgroundColor:[UIColor colorWithRed:0.7 green:0.7 blue:0.71 alpha:1]];
        [_pauseButton setTitle:@"暂停录音" forState:UIControlStateNormal];
        [_pauseButton setTag:132];
        /*
         //告诉layer将位于它之下的layer都遮盖住
         [_pauseButton.layer setMasksToBounds:YES];
         //设置layer的圆角,刚好是自身宽度的一半，这样就成了圆形
         [_pauseButton.layer setCornerRadius:8.0];
         //设置边框的宽度为20
         [_pauseButton.layer setBorderWidth:1.0];
         //设置边框的颜色
         [_pauseButton.layer setBorderColor:[UIColor whiteColor].CGColor];
         
         */
        [_pauseButton addTarget:self action:@selector(eventWithButton:) forControlEvents:UIControlEventTouchUpInside];
    }
    
    return _pauseButton;
}

- (UIButton*)playStartButton
{
    if (!_playStartButton) {
        _playStartButton = [UIButton buttonWithType:UIButtonTypeCustom];
        [_playStartButton setBackgroundColor:[UIColor colorWithRed:0.7 green:0.7 blue:0.71 alpha:1]];
        [_playStartButton setTitle:@"播放录音" forState:UIControlStateNormal];
        [_playStartButton setTag:133];
        /*
         //告诉layer将位于它之下的layer都遮盖住
         [_startButton.layer setMasksToBounds:YES];
         //设置layer的圆角,刚好是自身宽度的一半，这样就成了圆形
         [_startButton.layer setCornerRadius:8.0];
         //设置边框的宽度为20
         [_startButton.layer setBorderWidth:1.0];
         //设置边框的颜色
         [_startButton.layer setBorderColor:[UIColor whiteColor].CGColor];
         */
        [_playStartButton addTarget:self action:@selector(eventWithButton:) forControlEvents:UIControlEventTouchUpInside];
    }
    
    return _playStartButton;
}

- (UIButton*)playStopButton
{
    if (!_playStopButton) {
        _playStopButton = [UIButton buttonWithType:UIButtonTypeCustom];
        [_playStopButton setBackgroundColor:[UIColor colorWithRed:0.7 green:0.7 blue:0.71 alpha:1]];
        [_playStopButton setTitle:@"停止播放" forState:UIControlStateNormal];
        [_playStopButton setTag:134];
        /*
         //告诉layer将位于它之下的layer都遮盖住
         [_stopButton.layer setMasksToBounds:YES];
         //设置layer的圆角,刚好是自身宽度的一半，这样就成了圆形
         [_stopButton.layer setCornerRadius:8.0];
         //设置边框的宽度为20
         [_stopButton.layer setBorderWidth:1.0];
         //设置边框的颜色
         [_stopButton.layer setBorderColor:[UIColor whiteColor].CGColor];
         
         */
        [_playStopButton addTarget:self action:@selector(eventWithButton:) forControlEvents:UIControlEventTouchUpInside];
    }
    
    return _playStopButton;
}

- (UIButton*)playPauseButton
{
    if (!_playPauseButton) {
        _playPauseButton = [UIButton buttonWithType:UIButtonTypeCustom];
        [_playPauseButton setBackgroundColor:[UIColor colorWithRed:0.7 green:0.7 blue:0.71 alpha:1]];
        [_playPauseButton setTitle:@"暂停播放" forState:UIControlStateNormal];
        [_playPauseButton setTag:135];
        /*
         //告诉layer将位于它之下的layer都遮盖住
         [_pauseButton.layer setMasksToBounds:YES];
         //设置layer的圆角,刚好是自身宽度的一半，这样就成了圆形
         [_pauseButton.layer setCornerRadius:8.0];
         //设置边框的宽度为20
         [_pauseButton.layer setBorderWidth:1.0];
         //设置边框的颜色
         [_pauseButton.layer setBorderColor:[UIColor whiteColor].CGColor];
         
         */
        [_playPauseButton addTarget:self action:@selector(eventWithButton:) forControlEvents:UIControlEventTouchUpInside];
    }
    
    return _playPauseButton;
}

/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect {
    // Drawing code
}
*/

@end
