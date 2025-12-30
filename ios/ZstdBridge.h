#ifndef ZstdBridge_h
#define ZstdBridge_h

#import <Foundation/Foundation.h>

@interface ZstdBridge : NSObject

+ (nullable NSData *)compressString:(nonnull NSString *)input
                   compressionLevel:(int)level
                              error:(NSError * _Nullable * _Nullable)error;

+ (nullable NSString *)decompressData:(nonnull NSData *)data
                                error:(NSError * _Nullable * _Nullable)error;

@end

#endif /* ZstdBridge_h */
