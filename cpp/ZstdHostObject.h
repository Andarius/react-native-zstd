//
// Created by Andarius on 04/09/2022.
//

#ifndef REACTNATIVEZSTD_ZSTDHOSTOBJECT_H
#define REACTNATIVEZSTD_ZSTDHOSTOBJECT_H

#include <jsi-utils/JSIUtils.h>

namespace rnzstd {


    class JSI_EXPORT ZstdHostObject: public JSIUtilsHostObject {
        public:
          explicit ZstdHostObject();
          ~ZstdHostObject() override = default;
    };
}


#endif //REACTNATIVEZSTD_ZSTDHOSTOBJECT_H
