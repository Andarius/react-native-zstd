#include <jni.h>
#include "react-native-zstd.h"

extern "C"
JNIEXPORT jbyteArray JNICALL
Java_com_reactnativezstd_ZstdModule_nativeCompress(JNIEnv *env,
                                                   jclass type,
                                                   jstring buffIn,
                                                   jint compressionLevel) {
    // Convert Java string to C++ string
    const char *inputStr = env->GetStringUTFChars(buffIn, NULL);
    unsigned int compressedSizeOut = 0;

    uint8_t *compressedData = rnzstd::compress(inputStr,
                                               compressionLevel,
                                               compressedSizeOut);

    // Release the input string
    env->ReleaseStringUTFChars(buffIn, inputStr);

    // Convert the compressed data to a Java byte array
    jbyteArray outputArray = env->NewByteArray(compressedSizeOut);
    env->SetByteArrayRegion(outputArray, 0, compressedSizeOut, reinterpret_cast<jbyte *>(compressedData));

    delete[] compressedData;

    return outputArray;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_reactnativezstd_ZstdModule_nativeDecompress(JNIEnv *env,
                                                     jclass type,
                                                     jbyteArray buffIn,
                                                     jint sourceSize) {
    // Convert Java byte array to C++ byte array
    jbyte *inputBuff = env->GetByteArrayElements(buffIn, NULL);

    unsigned int decompressedSizeOut = 0;

    const char *decompressedData = rnzstd::decompress(reinterpret_cast<const uint8_t *>(inputBuff),
                                                      static_cast<size_t>(sourceSize),
                                                      decompressedSizeOut);

    // Release the input buffer
    env->ReleaseByteArrayElements(buffIn, inputBuff, JNI_ABORT);

    // Convert the decompressed data to a UTF-8 Java string
    jstring outputString = env->NewStringUTF(decompressedData);

    delete[] decompressedData;

    return outputString;
}
