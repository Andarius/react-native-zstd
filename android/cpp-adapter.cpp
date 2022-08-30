#include <jni.h>
#include "react-native-zstd.h"

extern "C"
JNIEXPORT jbyteArray JNICALL
Java_com_reactnativezstd_ZstdModule_nativeCompress(JNIEnv *env, jclass type,
                                                         jstring data, jint compressionLevel) {
    unsigned int compressedSize;

    const char *str = env->GetStringUTFChars(data, 0);
    const char *buff = example::compress(str, compressionLevel, compressedSize);
    env->ReleaseStringUTFChars(data, str);
    jbyteArray res;
    res = env->NewByteArray(compressedSize);
    env->SetByteArrayRegion(res, 0, compressedSize, (jbyte *)buff);

    delete[] buff;

    return res;

}

//extern "C"
//JNIEXPORT jstring JNICALL
//Java_com_reactnativezstd_ZstdModule_nativeDecompress(JNIEnv *env, jclass type, jcharArray data) {
//    unsigned int decompressedSize;
//    const char *buff = example::decompress(data, decompressedSize);
//    env->ReleaseStringUTFChars(data, data);
//
//    env->Get
//    jcharArray res;
//    res = env->NewCharArray(decompressedSize);
//    env->SetCharArrayRegion(res, 0, compressedSize, (jchar*)buff);
//
//    delete[] buff;
//
//    return res;
//
//}
