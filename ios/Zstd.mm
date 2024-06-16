#import "./Zstd.h"



@implementation Zstd


// RCT_EXPORT_MODULE(Zstd)
RCT_EXPORT_MODULE();

// RCT_EXPORT_BLOCKING_SYNCHRONOUS_METHOD(install) {
//   NSLog(@"Installing JSI bindings for react-native-zstd...");

//   RCTBridge* bridge = [RCTBridge currentBridge];
//   RCTCxxBridge* cxxBridge = (RCTCxxBridge*)bridge;

//   if (cxxBridge == nil) {
//     return @false;
//   }

//   using namespace facebook;

//   auto jsiRuntime = (jsi::Runtime*)cxxBridge.runtime;
//   if (jsiRuntime == nil) {
//     return @false;
//   }
//   auto& runtime = *jsiRuntime;

//   auto hostObject = std::make_shared<rnzstd::ZstdHostObject>();
//   auto object = jsi::Object::createFromHostObject(runtime, hostObject);
//   runtime.global().setProperty(runtime, "__ZSTDProxy", std::move(object));

//   NSLog(@"Successfully installed JSI bindings for react-native-zstd");
//   return @true;
// }

// Don't compile this code when we build for the old architecture.
#ifdef RCT_NEW_ARCH_ENABLED


- (NSArray *)compress:(NSString *)buffIn compressionLevel:(double)compressionLevel {
    unsigned int compressedSizeOut = 0;
    const char* _buffIn = [buffIn UTF8String];

    uint8_t *compressedData = rnzstd::compress(_buffIn, compressionLevel, compressedSizeOut);

    // if (compressedData == nullptr) {
    //     // Handle the error appropriately, maybe return nil or throw an exception
    //     return nil;
    // }

    NSMutableArray *array = [NSMutableArray arrayWithCapacity:compressedSizeOut];
    for (NSUInteger i = 0; i < compressedSizeOut; i++) {
        [array addObject:[NSNumber numberWithUnsignedChar:compressedData[i]]];
    }
    NSArray *compressedArr = [NSArray arrayWithArray:array];
    delete[] compressedData;

    return compressedArr;
}

- (NSString *)decompress:(NSArray *)buffIn {
    unsigned int decompressedSizeOut = 0;
    uint8_t *compressedData = new uint8_t[buffIn.count];
    for (NSUInteger i = 0; i < buffIn.count; i++) {
        compressedData[i] = [buffIn[i] unsignedCharValue];
    }

    const char* decompressedData = rnzstd::decompress(compressedData, buffIn.count, decompressedSizeOut);

    // if (decompressedData == nullptr) {
    //     // Handle the error appropriately, maybe return nil or throw an exception
    //     return nil;
    // }

    NSString *decompressedStr = [NSString stringWithUTF8String:decompressedData];
    delete[] compressedData;

    return decompressedStr;
}


- (std::shared_ptr<facebook::react::TurboModule>)getTurboModule:
    (const facebook::react::ObjCTurboModule::InitParams &)params
{
    return std::make_shared<facebook::react::NativeZstdSpecJSI>(params);
}
#endif




@end
