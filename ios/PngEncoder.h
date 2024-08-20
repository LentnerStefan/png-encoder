#ifdef __cplusplus
#import "react-native-png-encoder.h"
#endif

#ifdef RCT_NEW_ARCH_ENABLED
#import "RNPngEncoderSpec.h"

@interface PngEncoder : NSObject <NativePngEncoderSpec>
#else
#import <React/RCTBridgeModule.h>

@interface PngEncoder : NSObject <RCTBridgeModule>

@property(nonatomic, assign) BOOL setBridgeOnMainQueue;
#endif

@end
