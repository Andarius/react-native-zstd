#include <jni.h>
#include "react-native-zstd.h"

extern "C"
JNIEXPORT jint JNICALL
Java_com_reactnativezstd_ZstdModule_nativeMultiply(JNIEnv *env, jclass type, jint a, jint b) {
    return example::multiply(a, b);
}
