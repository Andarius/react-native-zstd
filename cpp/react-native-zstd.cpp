#include "react-native-zstd.h"
#include "zstd.h"

#include <cstring>


namespace rnzstd {


    uint8_t *compress(const char *buffIn, int compressionLevel, unsigned int &compressedSizeOut) {
        int const inputSize = (int) strlen(buffIn);

        size_t const outputSize = ZSTD_compressBound(inputSize);
        auto buffOut = new uint8_t[outputSize];

        size_t const compressedSize = ZSTD_compress(buffOut, outputSize, buffIn, inputSize,
                                                    compressionLevel);
        if (ZSTD_isError(compressedSize)) {
            throw ZstdError(ZSTD_getErrorName(compressedSize));
        }
        compressedSizeOut = compressedSize;

        return buffOut;
    }

    const char *decompress(const u_int8_t* buffIn,
                           size_t sourceSize,
                           unsigned int &decompressedSizeOut) {
        unsigned long long const outputSize = ZSTD_getFrameContentSize(buffIn, sourceSize);
        if (outputSize == ZSTD_CONTENTSIZE_ERROR) {
            throw ZstdError("Not compressed by zstd");
        }
        if (outputSize == ZSTD_CONTENTSIZE_UNKNOWN) {
            throw ZstdError("Original size unknown");
        }

        auto const buffOut = new char[outputSize];
        size_t const dSize = ZSTD_decompress(buffOut, outputSize, buffIn, sourceSize);
        decompressedSizeOut = dSize;

        /* When zstd knows the content size, it will error if it doesn't match. */
        if (dSize != outputSize) {
            throw ZstdError("Impossible because zstd will check this condition");
        }

        return buffOut;

    }

}
