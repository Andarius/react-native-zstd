#include "react-native-zstd.h"
#include "zstd.h"
#include <cstring>
#include <iostream> // exit

//class ZstdError : public std::exception {
//
//    size_t code;
//
//    const char *what() const throw() {
//        return ZSTD_getErrorName(code);
//    }
//};

namespace example {

    char *compress(const char *data, int compressionLevel, unsigned int &compressedSizeOut) {
        int const inputSize = (int) strlen(data);

        size_t const outputSize = ZSTD_compressBound(inputSize);
        char *const outputBuff = new char[outputSize];

        size_t const compressedSize = ZSTD_compress(outputBuff, outputSize, data, inputSize,
                                                    compressionLevel);
        if (ZSTD_isError(compressedSize)) {
//            std::cout << ZSTD_getErrorName(compressedSize) << std::endl;
            exit(1);
        }
        compressedSizeOut = compressedSize;

        return outputBuff;
    }

    char *decompress(const char *data, unsigned int &decompressedSizeOut) {
        size_t cSize = 0;

        unsigned long long const outputSize = ZSTD_getFrameContentSize(data, cSize);

        char *const outputBuff = new char[outputSize];

        size_t const dSize = ZSTD_decompress(outputBuff, outputSize, data, cSize);
        decompressedSizeOut = dSize;
        return outputBuff;
    }


}
