#import "./Zstd.h"
#import <React/RCTBridge+Private.h>
#import <React/RCTUtils.h>
#import <React/RCTLog.h> // For logging errors

@implementation Zstd

RCT_EXPORT_MODULE();


- (NSArray *)compress:(NSString *)dataStr compressionLevel:(double)compressionLevel {

    // 1. Convert NSString to raw bytes (UTF8 assumed)
    NSData *inputData = [dataStr dataUsingEncoding:NSUTF8StringEncoding];
    if (!inputData) {
        RCTLogError(@"[Zstd] Input string could not be encoded to UTF-8");
        return nil; // Return nil on input error
    }

    const uint8_t *buffIn = (const uint8_t *)inputData.bytes;
    size_t inputSize = inputData.length;
    int level = (int)compressionLevel; // Cast double to int for C++ function
    unsigned int compressedSizeOut = 0;
    uint8_t *compressedData = nullptr; // Initialize to nullptr
    NSArray<NSNumber *> *resultArray = nil; // Initialize result to nil


    try {
        // 2. Call C++ compress function (potential throw)
        compressedData = rnzstd::compress(buffIn, inputSize, level, compressedSizeOut);

        // If compress succeeded, compressedData is valid (or nullptr if 0 bytes compressed)

        // 3. Convert result (uint8_t*) to NSArray<NSNumber*>
        NSMutableArray<NSNumber *> *tempArray = [NSMutableArray arrayWithCapacity:compressedSizeOut];
        for (NSUInteger i = 0; i < compressedSizeOut; i++) {
            [tempArray addObject:[NSNumber numberWithInt:(int)compressedData[i]]];
        }
        resultArray = tempArray; // Assign the result only if conversion succeeded

    } catch (const rnzstd::ZstdError& e) {
        // 4. Catch specific C++ ZstdError
        RCTLogError(@"[Zstd] Compression Error: %s", e.what());
        resultArray = nil;
    } catch (const std::exception& e) {
        // Catch other standard C++ exceptions
        RCTLogError(@"[Zstd] Compression Standard Exception: %s", e.what());
        resultArray = nil;
    } catch (...) {
        // Catch any other unknown exceptions
        RCTLogError(@"[Zstd] Unknown error occurred during compression.");
        resultArray = nil;
    }

    // 5. Clean up the C++ allocated buffer AFTER try/catch block
    //    This is safe because delete[] nullptr has no effect.
    delete[] compressedData;

    // 6. Return the result (will be nil if any error occurred)
    return resultArray;

}

- (NSString *)decompress:(NSArray *)dataArray {
   if (!dataArray || dataArray.count == 0) {
       RCTLogInfo(@"[Zstd] Decompress called with nil or empty array.");
       return @""; // Return empty string for empty input, or nil if that's preferred
   }

    size_t inputSize = dataArray.count;
    // 1. Convert NSArray<NSNumber*> to uint8_t* buffer
    std::unique_ptr<uint8_t[]> inputBuffer(new uint8_t[inputSize]);
    for (NSUInteger i = 0; i < inputSize; i++) {
        if (![dataArray[i] isKindOfClass:[NSNumber class]]) {
             RCTLogError(@"[Zstd] Decompress input array contains non-Number object at index %lu", (unsigned long)i);
             return nil;
        }
        inputBuffer[i] = (uint8_t)[dataArray[i] intValue];
    }

    unsigned int decompressedSizeOut = 0;
    uint8_t *decompressedData = nullptr;
    NSString *resultString = nil;

    // Use C++ try/catch for C++ exceptions
    try {
        // 2. Call C++ decompress function (potential throw)
        decompressedData = rnzstd::decompress(inputBuffer.get(), inputSize, decompressedSizeOut);

        // If decompress succeeded, decompressedData is valid (or nullptr)

        // 3. Convert result (uint8_t*) to NSString (assuming UTF-8)
        resultString = [[NSString alloc] initWithBytes:decompressedData
                                                length:decompressedSizeOut
                                              encoding:NSUTF8StringEncoding];

        // 4. Check for UTF-8 conversion error
        if (resultString == nil && decompressedSizeOut > 0) {
            RCTLogError(@"[Zstd] Decompressed data is not valid UTF-8");
            // resultString is already nil
        }

    }
    // Use C++ catch for C++ exceptions
    catch (const rnzstd::ZstdError& e) {
        // 5. Catch specific C++ ZstdError
        RCTLogError(@"[Zstd] Decompression Error: %s", e.what());
        resultString = nil; // Ensure result is nil
    }
    catch (const std::exception& e) {
        // Catch other standard C++ exceptions
        RCTLogError(@"[Zstd] Decompression Standard Exception: %s", e.what());
        resultString = nil;
    }
    catch (...) {
        // Catch any other unknown exceptions
        RCTLogError(@"[Zstd] Unknown error occurred during decompression.");
        resultString = nil;
    }

    // 6. Clean up the C++ allocated buffer AFTER try/catch block
    delete[] decompressedData;

    // 7. Return the result (will be nil if any error occurred, including UTF-8 failure)
    return resultString;
}


- (std::shared_ptr<facebook::react::TurboModule>)getTurboModule:
    (const facebook::react::ObjCTurboModule::InitParams &)params
{
    return std::make_shared<facebook::react::NativeZstdSpecJSI>(params);
}


@end
