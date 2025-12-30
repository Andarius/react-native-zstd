#include <jni.h>
#include "react-native-zstd.h"
#include "reactnativezstdOnLoad.hpp"
#include <string>
#include <vector>
#include <stdexcept>

// Helper function to throw Java exceptions from C++
void ThrowJavaException(JNIEnv *env, const char *className, const char *message) {
    jclass exClass = env->FindClass(className);
    if (exClass != NULL) {
        env->ThrowNew(exClass, message);
    }
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void*) {
    return margelo::nitro::zstd::initialize(vm);
}

extern "C"
JNIEXPORT jbyteArray JNICALL
Java_com_margelo_nitro_zstd_HybridZstd_nativeCompress(JNIEnv *env,
                                                      jclass type,
                                                      jstring buffIn_jstring,
                                                      jint compressionLevel) {
    if (buffIn_jstring == NULL) {
        ThrowJavaException(env, "java/lang/NullPointerException", "Input string cannot be null");
        return NULL;
    }
    const char *buffIn_chars = env->GetStringUTFChars(buffIn_jstring, NULL);
    if (buffIn_chars == NULL) { return NULL; }
    size_t inputSize = env->GetStringUTFLength(buffIn_jstring);
    jbyteArray outputArray = NULL;
    uint8_t *compressedData = NULL;
    try {
        unsigned int compressedSizeOut = 0;
        compressedData = rnzstd::compress(reinterpret_cast<const uint8_t*>(buffIn_chars), inputSize, compressionLevel, compressedSizeOut);
        env->ReleaseStringUTFChars(buffIn_jstring, buffIn_chars);
        buffIn_chars = NULL;
        if (compressedData != NULL && compressedSizeOut > 0) {
            outputArray = env->NewByteArray(compressedSizeOut);
            if (outputArray != NULL) {
                env->SetByteArrayRegion(outputArray, 0, compressedSizeOut, reinterpret_cast<jbyte *>(compressedData));
            }
        } else {
            outputArray = env->NewByteArray(0);
        }
    } catch (const std::exception& e) {
        if (buffIn_chars != NULL) env->ReleaseStringUTFChars(buffIn_jstring, buffIn_chars);
        delete[] compressedData;
        ThrowJavaException(env, "java/lang/RuntimeException", e.what());
        return NULL;
    } catch (...) {
        if (buffIn_chars != NULL) env->ReleaseStringUTFChars(buffIn_jstring, buffIn_chars);
        delete[] compressedData;
        ThrowJavaException(env, "java/lang/RuntimeException", "Unknown C++ exception during compression");
        return NULL;
    }
    delete[] compressedData;
    return outputArray;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_margelo_nitro_zstd_HybridZstd_nativeDecompress(JNIEnv *env,
                                                        jclass type,
                                                        jbyteArray buffIn_jbyteArray,
                                                        jint sourceSize) {
    if (buffIn_jbyteArray == NULL) {
        ThrowJavaException(env, "java/lang/NullPointerException", "Input byte array cannot be null");
        return NULL;
    }
    jsize actualSourceSize = env->GetArrayLength(buffIn_jbyteArray);
    sourceSize = actualSourceSize;
    if (sourceSize <= 0) {
        return env->NewStringUTF("");
    }

    jbyte *buffIn_bytes = env->GetByteArrayElements(buffIn_jbyteArray, NULL);
    if (buffIn_bytes == NULL) { return NULL; }

    jstring resultJString = NULL;
    uint8_t *decompressedData = NULL;
    jbyteArray tempJavaBytes = NULL;
    jstring charsetUTF = NULL;
    jclass stringClass = NULL;

    try {
        unsigned int decompressedSizeOut = 0;
        size_t compressedDataSize = static_cast<size_t>(sourceSize);

        decompressedData = rnzstd::decompress(reinterpret_cast<const uint8_t*>(buffIn_bytes),
                                              compressedDataSize,
                                              decompressedSizeOut);

        env->ReleaseByteArrayElements(buffIn_jbyteArray, buffIn_bytes, JNI_ABORT);
        buffIn_bytes = NULL;

        if (decompressedData == NULL || decompressedSizeOut == 0) {
            delete[] decompressedData;
            return env->NewStringUTF("");
        }

        tempJavaBytes = env->NewByteArray(decompressedSizeOut);
        if (tempJavaBytes == NULL) { throw std::runtime_error("Failed to allocate temp jbyteArray"); }

        env->SetByteArrayRegion(tempJavaBytes, 0, decompressedSizeOut, reinterpret_cast<jbyte *>(decompressedData));

        delete[] decompressedData;
        decompressedData = NULL;

        stringClass = env->FindClass("java/lang/String");
        if (stringClass == NULL) { throw std::runtime_error("Failed to find String class"); }

        jmethodID constructorId = env->GetMethodID(stringClass, "<init>", "([BLjava/lang/String;)V");
        if (constructorId == NULL) { throw std::runtime_error("Failed to find String constructor ([B, String)"); }

        charsetUTF = env->NewStringUTF("UTF-8");
        if (charsetUTF == NULL) { throw std::runtime_error("Failed to create UTF-8 jstring"); }

        resultJString = (jstring)env->NewObject(stringClass, constructorId, tempJavaBytes, charsetUTF);
        if (resultJString == NULL) {
            if(env->ExceptionCheck()) {
                throw std::runtime_error("Exception during String object creation");
            }
            throw std::runtime_error("Failed to create String object via NewObject");
        }

    } catch (const std::exception& e) {
        if (buffIn_bytes != NULL) env->ReleaseByteArrayElements(buffIn_jbyteArray, buffIn_bytes, JNI_ABORT);
        delete[] decompressedData;
        if (tempJavaBytes != NULL) env->DeleteLocalRef(tempJavaBytes);
        if (charsetUTF != NULL) env->DeleteLocalRef(charsetUTF);
        if (stringClass != NULL) env->DeleteLocalRef(stringClass);
        ThrowJavaException(env, "java/lang/RuntimeException", e.what());
        return NULL;
    } catch (...) {
        if (buffIn_bytes != NULL) env->ReleaseByteArrayElements(buffIn_jbyteArray, buffIn_bytes, JNI_ABORT);
        delete[] decompressedData;
        if (tempJavaBytes != NULL) env->DeleteLocalRef(tempJavaBytes);
        if (charsetUTF != NULL) env->DeleteLocalRef(charsetUTF);
        if (stringClass != NULL) env->DeleteLocalRef(stringClass);
        ThrowJavaException(env, "java/lang/RuntimeException", "Unknown C++ exception during decompression");
        return NULL;
    }

    if (tempJavaBytes != NULL) env->DeleteLocalRef(tempJavaBytes);
    if (charsetUTF != NULL) env->DeleteLocalRef(charsetUTF);
    if (stringClass != NULL) env->DeleteLocalRef(stringClass);

    return resultJString;
}
