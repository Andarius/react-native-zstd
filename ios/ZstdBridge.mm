#import "ZstdBridge.h"
#import "react-native-zstd.h"

static NSString *const ZstdErrorDomain = @"com.reactnativezstd.ZstdError";

@implementation ZstdBridge

+ (nullable NSData *)compressString:(nonnull NSString *)input
                   compressionLevel:(int)level
                              error:(NSError * _Nullable * _Nullable)error {
    NSData *inputData = [input dataUsingEncoding:NSUTF8StringEncoding];
    if (!inputData) {
        if (error) {
            *error = [NSError errorWithDomain:ZstdErrorDomain
                                         code:1
                                     userInfo:@{NSLocalizedDescriptionKey: @"Failed to encode input string to UTF-8"}];
        }
        return nil;
    }

    const uint8_t *buffIn = (const uint8_t *)inputData.bytes;
    size_t inputSize = inputData.length;
    unsigned int compressedSizeOut = 0;

    try {
        uint8_t *compressedData = rnzstd::compress(buffIn, inputSize, level, compressedSizeOut);
        NSData *result = [NSData dataWithBytes:compressedData length:compressedSizeOut];
        delete[] compressedData;
        return result;
    } catch (const rnzstd::ZstdError& e) {
        if (error) {
            *error = [NSError errorWithDomain:ZstdErrorDomain
                                         code:2
                                     userInfo:@{NSLocalizedDescriptionKey: [NSString stringWithUTF8String:e.what()]}];
        }
        return nil;
    } catch (const std::exception& e) {
        if (error) {
            *error = [NSError errorWithDomain:ZstdErrorDomain
                                         code:3
                                     userInfo:@{NSLocalizedDescriptionKey: [NSString stringWithUTF8String:e.what()]}];
        }
        return nil;
    }
}

+ (nullable NSString *)decompressData:(nonnull NSData *)data
                                error:(NSError * _Nullable * _Nullable)error {
    if (data.length == 0) {
        return @"";
    }

    const uint8_t *buffIn = (const uint8_t *)data.bytes;
    size_t inputSize = data.length;
    unsigned int decompressedSizeOut = 0;

    try {
        uint8_t *decompressedData = rnzstd::decompress(buffIn, inputSize, decompressedSizeOut);
        NSString *result = [[NSString alloc] initWithBytes:decompressedData
                                                    length:decompressedSizeOut
                                                  encoding:NSUTF8StringEncoding];
        delete[] decompressedData;

        if (result == nil && decompressedSizeOut > 0) {
            if (error) {
                *error = [NSError errorWithDomain:ZstdErrorDomain
                                             code:4
                                         userInfo:@{NSLocalizedDescriptionKey: @"Decompressed data is not valid UTF-8"}];
            }
            return nil;
        }
        return result;
    } catch (const rnzstd::ZstdError& e) {
        if (error) {
            *error = [NSError errorWithDomain:ZstdErrorDomain
                                         code:5
                                     userInfo:@{NSLocalizedDescriptionKey: [NSString stringWithUTF8String:e.what()]}];
        }
        return nil;
    } catch (const std::exception& e) {
        if (error) {
            *error = [NSError errorWithDomain:ZstdErrorDomain
                                         code:6
                                     userInfo:@{NSLocalizedDescriptionKey: [NSString stringWithUTF8String:e.what()]}];
        }
        return nil;
    }
}

@end
