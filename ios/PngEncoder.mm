#import "PngEncoder.h"
#import <React/RCTBridge+Private.h>
#import <React/RCTUtils.h>
#import "react-native-png-encoder/react-native-png-encoder.h"


@implementation PngEncoder

@synthesize bridge=_bridge;
@synthesize methodQueue = _methodQueue;

RCT_EXPORT_MODULE()

+ (BOOL)requiresMainQueueSetup {
  return YES;
}

// Installing JSI Bindings as done by
// https://github.com/mrousavy/react-native-mmkv
RCT_EXPORT_BLOCKING_SYNCHRONOUS_METHOD(install)
{
    RCTBridge* bridge = [RCTBridge currentBridge];
    RCTCxxBridge* cxxBridge = (RCTCxxBridge*)bridge;
    if (cxxBridge == nil) {
        return @false;
    }

    auto jsiRuntime = (facebook::jsi::Runtime*) cxxBridge.runtime;
    if (jsiRuntime == nil) {
        return @false;
    }

    auto tmpDir = [NSFileManager defaultManager].temporaryDirectory;
    std::string cachePathStr = [tmpDir.path UTF8String];
    installPngEncoder(*(facebook::jsi::Runtime *)cxxBridge.runtime,cachePathStr);
    return @true;
}

@end
