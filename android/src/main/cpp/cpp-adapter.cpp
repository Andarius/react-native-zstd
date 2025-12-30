#include <jni.h>
#include "reactnativezstdOnLoad.hpp"

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void*) {
    return margelo::nitro::zstd::initialize(vm);
}
