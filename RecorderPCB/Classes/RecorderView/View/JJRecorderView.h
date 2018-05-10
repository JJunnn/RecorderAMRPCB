//
//  JJRecorderView.h
//  RecorderPCB
//
//  Created by AG on 2018/5/9.
//  Copyright © 2018 JJ. All rights reserved.
//

#import <UIKit/UIKit.h>


typedef void(^blockRecorderView)(NSInteger tag);

@interface JJRecorderView : UIView

@property (nonatomic, copy) blockRecorderView blockRecorderView;

@end
