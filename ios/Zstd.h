#ifdef __cplusplus
#import "react-native-zstd.h"
#endif

#ifdef RCT_NEW_ARCH_ENABLED
#import "RNZstdSpec.h"

@interface Zstd : NSObject <NativeZstdSpec>
#else
#import <React/RCTBridgeModule.h>

@interface Zstd : NSObject <RCTBridgeModule>
#endif

@end
