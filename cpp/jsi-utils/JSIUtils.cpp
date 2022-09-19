//
// Created by julien on 04/09/22.
//

#include <jsi/jsi.h>
#include "JSIUtils.h"

namespace rnzstd {


    std::vector<jsi::PropNameID> JSIUtilsHostObject::getPropertyNames(
            jsi::Runtime &runtime) {
        std::vector<jsi::PropNameID> propertyNames;
        for (const auto &field: fields) {
            propertyNames.push_back(jsi::PropNameID::forAscii(runtime, field.first));
        }
        return propertyNames;
    }

    jsi::Value JSIUtilsHostObject::get(jsi::Runtime &runtime,
                                       const jsi::PropNameID &propNameId) {
        auto name = propNameId.utf8(runtime);
        auto func = fields.find(name);
        return func != fields.end() ? (func->second)(runtime) : jsi::Value::undefined();
    }

    void JSIStringToChar(jsi::Runtime &runtime, jsi::String &jsiStr, char *&out) {
        std::string str = jsiStr.utf8(runtime);
        out = new char[str.length() + 1];
        std::strcpy(out, str.c_str());
    }

    jsi::Array bytesToJSIArray(jsi::Runtime &runtime, uint8_t *buff, size_t size) {
        auto arr = jsi::Array(runtime, size);
        for (int i = 0; i < size; ++i) {
            arr.setValueAtIndex(runtime, i, static_cast<int>(buff[i]));
        }
        return arr;
    }


} // Namespace
