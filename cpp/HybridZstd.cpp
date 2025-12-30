#include "HybridZstd.hpp"
#include "react-native-zstd.h"
#include <stdexcept>

namespace margelo::nitro::zstd {

std::shared_ptr<ArrayBuffer> HybridZstd::compress(const std::string& data, double compressionLevel) {
  unsigned int compressedSizeOut = 0;
  // rnzstd::compress uses new[] allocation and throws ZstdError on failure
  uint8_t* compressedData = rnzstd::compress(
    reinterpret_cast<const uint8_t*>(data.data()),
    data.size(),
    static_cast<int>(compressionLevel),
    compressedSizeOut
  );

  if (compressedData == nullptr) {
    throw std::runtime_error("ZSTD compression failed");
  }

  // Create ArrayBuffer and copy data
  auto buffer = ArrayBuffer::allocate(compressedSizeOut);
  if (buffer == nullptr || buffer->data() == nullptr) {
    delete[] compressedData;
    throw std::runtime_error("Failed to allocate output buffer");
  }
  std::memcpy(buffer->data(), compressedData, compressedSizeOut);
  delete[] compressedData;

  return buffer;
}

std::string HybridZstd::decompress(const std::shared_ptr<ArrayBuffer>& data) {
  unsigned int decompressedSizeOut = 0;
  // rnzstd::decompress uses new[] allocation and throws ZstdError on failure
  uint8_t* decompressedData = rnzstd::decompress(
    static_cast<const uint8_t*>(data->data()),
    data->size(),
    decompressedSizeOut
  );

  if (decompressedData == nullptr) {
    throw std::runtime_error("ZSTD decompression failed");
  }

  // Convert to string
  std::string result(reinterpret_cast<char*>(decompressedData), decompressedSizeOut);
  delete[] decompressedData;

  return result;
}

} // namespace margelo::nitro::zstd
