#include <jni.h>
#include <jsi/jsi.h>
#include "ZstdHostObject.h"

using namespace facebook;


void install(jsi::Runtime& runtime) {
    auto hostObject = std::make_shared<rnzstd::ZstdHostObject>();
    auto object = jsi::Object::createFromHostObject(runtime, hostObject);
    runtime.global().setProperty(runtime, "__ZSTDProxy", std::move(object));
}

extern "C"
JNIEXPORT void JNICALL
Java_com_reactnativezstd_ZstdModule_nativeInstall(JNIEnv *env, jobject thiz, jlong jsiPtr) {
    auto runtime = reinterpret_cast<jsi::Runtime *>(jsiPtr);
    if (runtime) {
        install(*runtime);
    }
}
