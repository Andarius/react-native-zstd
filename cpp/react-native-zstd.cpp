#include "react-native-zstd.h"
#include "zstd.h"

#include <cstring> // For std::memcpy if needed, though ZSTD handles buffer content
#include <vector>  // Can be useful for temporary buffers if needed

namespace rnzstd {


    uint8_t *compress(const uint8_t *buffIn,
                                 size_t inputSize,
                                 int compressionLevel,
                                 unsigned int &compressedSizeOut) {

        // Calculate the maximum potential compressed size
        size_t const outputSizeBound = ZSTD_compressBound(inputSize);
        // Allocate buffer for compressed data
        auto buffOut = new uint8_t[outputSizeBound];

        size_t const compressedSize = ZSTD_compress(buffOut, outputSizeBound, buffIn, inputSize,
                                                    compressionLevel);

        if (ZSTD_isError(compressedSize)) {
            delete[] buffOut; // Clean up allocated buffer before throwing
            throw ZstdError(ZSTD_getErrorName(compressedSize));
        }

        compressedSizeOut = static_cast<unsigned int>(compressedSize);

        // NOTE: Consider shrinking the buffer if outputSizeBound is much larger
        // than compressedSize to save memory, but this adds complexity (realloc/copy).
        // For simplicity, returning the potentially larger buffer is often done.

        return buffOut; // Return pointer to compressed data buffer
    }


    uint8_t *decompress(const uint8_t* buffIn, // Input is compressed bytes
                           size_t sourceSize,  // Size of compressed data
                           unsigned int &decompressedSizeOut) { // Output for decompressed size


        unsigned long long const outputSize = ZSTD_getFrameContentSize(buffIn, sourceSize);

        // Handle errors or unknown size
        if (outputSize == ZSTD_CONTENTSIZE_ERROR) {
            throw ZstdError("Not compressed by zstd or invalid header");
        }
        if (outputSize == ZSTD_CONTENTSIZE_UNKNOWN) {
            // This implementation requires knowing the original size.
            // Alternative: Decompress chunk-by-chunk if size is unknown (more complex).
            throw ZstdError("Original size unknown (cannot decompress)");
        }
        // Check for potential size issues (e.g., unreasonably large size)
        // if (outputSize > 1024 * 1024 * 1024) { // Example limit: 1GB
        //      throw ZstdError("Decompressed size too large");
        // }


        auto buffOut = new uint8_t[outputSize];
        size_t const dSize = ZSTD_decompress(buffOut, outputSize, buffIn, sourceSize);

        // Check for decompression errors
        if (ZSTD_isError(dSize)) {
            delete[] buffOut; // Clean up allocated buffer before throwing
            throw ZstdError(ZSTD_getErrorName(dSize));
        }

        // Zstd already verifies dSize == outputSize when outputSize is known,
        // but a check doesn't hurt (though redundant if ZSTD_decompress succeeded).
        if (dSize != outputSize) {
             delete[] buffOut;
             throw ZstdError("Decompression size mismatch (should not happen if ZSTD_decompress succeeded)");
        }

        // Store the actual decompressed size
        decompressedSizeOut = static_cast<unsigned int>(dSize);

        return buffOut;
    }

}
