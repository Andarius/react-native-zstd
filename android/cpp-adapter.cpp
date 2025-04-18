#include <jni.h>
#include "react-native-zstd.h"
#include <string>
#include <vector>
#include <stdexcept> // For std::runtime_error

// Helper function to throw Java exceptions from C++
void ThrowJavaException(JNIEnv *env, const char *className, const char *message) {
    jclass exClass = env->FindClass(className);
    if (exClass != NULL) {
        env->ThrowNew(exClass, message);
        // No need to delete local ref after ThrowNew typically
        // env->DeleteLocalRef(exClass);
    }
}


extern "C"
JNIEXPORT jbyteArray JNICALL
Java_com_reactnativezstd_ZstdModule_nativeCompress(JNIEnv *env,
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
        env->ReleaseStringUTFChars(buffIn_jstring, buffIn_chars); buffIn_chars = NULL;
        if (compressedData != NULL && compressedSizeOut > 0) {
            outputArray = env->NewByteArray(compressedSizeOut);
            if (outputArray != NULL) {
                env->SetByteArrayRegion(outputArray, 0, compressedSizeOut, reinterpret_cast<jbyte *>(compressedData));
            }
        } else { outputArray = env->NewByteArray(0); }
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
Java_com_reactnativezstd_ZstdModule_nativeDecompress(JNIEnv *env,
                                                     jclass type,
                                                     jbyteArray buffIn_jbyteArray, // Compressed input
                                                     jint sourceSize) { // Size of compressed input

    if (buffIn_jbyteArray == NULL) {
         ThrowJavaException(env, "java/lang/NullPointerException", "Input byte array cannot be null");
         return NULL;
    }
    jsize actualSourceSize = env->GetArrayLength(buffIn_jbyteArray);
    sourceSize = actualSourceSize; // Use actual length from JNI
     if (sourceSize <= 0) {
         // Return empty string for empty input
         return env->NewStringUTF("");
     }

    // Get pointer to the input Java byte array data (compressed)
    jbyte *buffIn_bytes = env->GetByteArrayElements(buffIn_jbyteArray, NULL);
    if (buffIn_bytes == NULL) { return NULL; } // OOM error likely

    jstring resultJString = NULL;      // Initialize result
    uint8_t *decompressedData = NULL; // C++ buffer
    jbyteArray tempJavaBytes = NULL;  // Temp Java byte array
    jstring charsetUTF = NULL;        // Charset name jstring
    jclass stringClass = NULL;        // String class ref

    try {
        unsigned int decompressedSizeOut = 0;
        size_t compressedDataSize = static_cast<size_t>(sourceSize);

        // Call C++ decompress (returns uint8_t*)
        decompressedData = rnzstd::decompress(reinterpret_cast<const uint8_t*>(buffIn_bytes),
                                              compressedDataSize,
                                              decompressedSizeOut);

        // Release input buffer early
        env->ReleaseByteArrayElements(buffIn_jbyteArray, buffIn_bytes, JNI_ABORT);
        buffIn_bytes = NULL;

        if (decompressedData == NULL || decompressedSizeOut == 0) {
            // Decompression failed or result is empty
             delete[] decompressedData; // Clean up C++ buffer if allocated
             return env->NewStringUTF(""); // Return empty Java string
        }

        // --- Convert raw bytes to Java String using UTF-8 constructor ---

        // 1. Create temporary Java byte array for decompressed data
        tempJavaBytes = env->NewByteArray(decompressedSizeOut);
        if (tempJavaBytes == NULL) { throw std::runtime_error("Failed to allocate temp jbyteArray"); } // OOM

        // 2. Copy decompressed data into the temp Java byte array
        env->SetByteArrayRegion(tempJavaBytes, 0, decompressedSizeOut, reinterpret_cast<jbyte *>(decompressedData));

        // 3. Clean up the C++ buffer now that data is copied
        delete[] decompressedData;
        decompressedData = NULL;

        // 4. Find the String class
        stringClass = env->FindClass("java/lang/String");
        if (stringClass == NULL) { throw std::runtime_error("Failed to find String class"); } // ClassNotFound

        // 5. Get the constructor ID: String(byte[], String charsetName)
        //    Signature: ([BLjava/lang/String;)V
        jmethodID constructorId = env->GetMethodID(stringClass, "<init>", "([BLjava/lang/String;)V");
        if (constructorId == NULL) { throw std::runtime_error("Failed to find String constructor ([B, String)"); } // NoSuchMethod

        // 6. Create Java String for the charset name "UTF-8"
        charsetUTF = env->NewStringUTF("UTF-8");
        if (charsetUTF == NULL) { throw std::runtime_error("Failed to create UTF-8 jstring"); } // OOM

        // 7. Create the new Java String object using the constructor
        resultJString = (jstring)env->NewObject(stringClass, constructorId, tempJavaBytes, charsetUTF);
        if (resultJString == NULL) {
            // Check for exceptions from NewObject (though encoding errors often don't throw here)
             if(env->ExceptionCheck()) {
                 // Exception occurred during String construction (rare for encoding, maybe OOM)
                 throw std::runtime_error("Exception during String object creation");
             }
             // If no exception, NewObject might have just failed (e.g. OOM)
              throw std::runtime_error("Failed to create String object via NewObject");
        }

        // --- End Conversion ---

    } catch (const std::exception& e) { // Catch C++ exceptions (std::runtime_error, bad_alloc)
         if (buffIn_bytes != NULL) env->ReleaseByteArrayElements(buffIn_jbyteArray, buffIn_bytes, JNI_ABORT); // Ensure release
         delete[] decompressedData; // Ensure C++ buffer cleanup
         // Clean up intermediate JNI objects if created before exception
         if (tempJavaBytes != NULL) env->DeleteLocalRef(tempJavaBytes);
         if (charsetUTF != NULL) env->DeleteLocalRef(charsetUTF);
         if (stringClass != NULL) env->DeleteLocalRef(stringClass);
         // Throw Java exception
         ThrowJavaException(env, "java/lang/RuntimeException", e.what());
         return NULL; // Return null as exception is pending
    } catch (...) { // Catch any other unknown exceptions
         if (buffIn_bytes != NULL) env->ReleaseByteArrayElements(buffIn_jbyteArray, buffIn_bytes, JNI_ABORT);
         delete[] decompressedData;
         if (tempJavaBytes != NULL) env->DeleteLocalRef(tempJavaBytes);
         if (charsetUTF != NULL) env->DeleteLocalRef(charsetUTF);
         if (stringClass != NULL) env->DeleteLocalRef(stringClass);
         ThrowJavaException(env, "java/lang/RuntimeException", "Unknown C++ exception during decompression");
         return NULL;
    }

    // Clean up intermediate JNI local references if they were successfully created
    if (tempJavaBytes != NULL) env->DeleteLocalRef(tempJavaBytes);
    if (charsetUTF != NULL) env->DeleteLocalRef(charsetUTF);
    if (stringClass != NULL) env->DeleteLocalRef(stringClass);

    return resultJString;
}
