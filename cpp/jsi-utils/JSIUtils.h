//
// Created by julien on 04/09/22.
//

#ifndef REACTNATIVEZSTD_JSIUTILS_H
#define REACTNATIVEZSTD_JSIUTILS_H

#include <jsi/jsi.h>
#include <utility>
#include <unordered_map>

namespace jsi = facebook::jsi;

#define GET_FIELD(name, body){ name, HOST_LAMBDA_CAP(name, [=], body)}


// https://docs.microsoft.com/en-us/cpp/cpp/lambda-expressions-in-cpp?view=msvc-170#capture-clause
#define HOST_LAMBDA_CAP(name, capture, body)                                \
    capture(jsi::Runtime &runtime) {                                        \
        auto func = capture(jsi::Runtime &runtime,                          \
                                  const jsi::Value &thisValue,              \
                                  const jsi::Value *arguments,              \
                                  size_t count) -> jsi::Value body;         \
        auto propNameID = jsi::PropNameID::forAscii(runtime, name);         \
        return jsi::Function::createFromHostFunction(runtime,               \
                                                     propNameID,            \
                                                     0,                     \
                                                     func);                 \
}                                                                           \



#define CHECK(cond, msg) cond ? throw jsi::JSError(runtime, msg): void();


namespace rnzstd {

    using JSIValueBuilder = std::function<jsi::Value(jsi::Runtime &runtime)>;

    void JSIStringToChar(jsi::Runtime &runtime, jsi::String &jsiStr, char *&out);

    jsi::Array bytesToJSIArray(jsi::Runtime &runtime, uint8_t *buff, size_t size);

    class JSI_EXPORT JSIUtilsHostObject : public jsi::HostObject {
    public:
        explicit JSIUtilsHostObject() = default;

        ~JSIUtilsHostObject() override = default;

    public:
        std::unordered_map<std::string, JSIValueBuilder> fields;

        std::vector<jsi::PropNameID> getPropertyNames(jsi::Runtime &runtime) override;

        jsi::Value get(jsi::Runtime &runtime, const jsi::PropNameID &propNameId) override;
    };
} // namespace

#endif //REACTNATIVEZSTD_JSIUTILS_H
