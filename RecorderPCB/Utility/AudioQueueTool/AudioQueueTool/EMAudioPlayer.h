

#import <Foundation/Foundation.h>

@class EMAudioPlayer;

@protocol EMAudioPlayerDelegate <NSObject>

@optional
/* audioPlayerDidFinishPlaying:successfully: is called when a sound has finished playing. This method is NOT called if the player is stopped due to an interruption. */
- (void)audioPlayerDidFinishPlaying:(EMAudioPlayer*)player successfully:(BOOL)flag;

@end


@interface EMAudioPlayer : NSObject

@property (nonatomic, strong) NSString* filePath; /**<语音文件路径>*/
@property (nonatomic, weak) id<EMAudioPlayerDelegate> delegate; /**<代理对象>*/

- (instancetype)initWithFilePath:(NSString*)filePath;
- (void)startPlayer; //播放录音
- (void)stopPlayer;  //关闭播放录音
- (BOOL)playing; //判断是否在播放


@end
