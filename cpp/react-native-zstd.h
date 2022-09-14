#ifndef RNZSTD_H
#define RNZSTD_H

#include <string>
#include <stdexcept>

namespace rnzstd {
    extern "C" uint8_t *compress(const char *buffIn,
                                 int compressionLevel,
                                 unsigned int &compressedSizeOut);
    extern "C" const char *decompress(const uint8_t *buffIn,
                                      size_t sourceSize,
                                      unsigned int &decompressedSizeOut);

    class ZstdError : public std::runtime_error {
    public:
        explicit ZstdError(const std::string &msg) : std::runtime_error{msg} {}
    };

}

#endif /* RNZSTD_H */
