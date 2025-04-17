#ifndef RNZSTD_H
#define RNZSTD_H

#include <string>
#include <stdexcept>
#include <cstddef>
#include <cstdint>

namespace rnzstd {
    uint8_t *compress(const uint8_t *buffIn,
                      size_t inputSize,
                      int compressionLevel,
                      unsigned int &compressedSizeOut);


    uint8_t *decompress(const uint8_t *buffIn,
                                      size_t sourceSize, // size of compressed data
                                      unsigned int &decompressedSizeOut);

    class ZstdError : public std::runtime_error {
    public:
        explicit ZstdError(const std::string &msg) : std::runtime_error{msg} {}
    };

}

#endif /* RNZSTD_H */
