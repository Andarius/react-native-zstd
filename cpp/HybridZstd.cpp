#include "HybridZstd.hpp"
#include "react-native-zstd.h"

namespace margelo::nitro::zstd {

std::shared_ptr<ArrayBuffer> HybridZstd::compress(const std::string& data, double compressionLevel) {
  unsigned int compressedSizeOut = 0;
  uint8_t* compressedData = rnzstd::compress(
    reinterpret_cast<const uint8_t*>(data.data()),
    data.size(),
    static_cast<int>(compressionLevel),
    compressedSizeOut
  );

  // Create ArrayBuffer and copy data
  auto buffer = ArrayBuffer::allocate(compressedSizeOut);
  std::memcpy(buffer->data(), compressedData, compressedSizeOut);
  delete[] compressedData;

  return buffer;
}

std::string HybridZstd::decompress(const std::shared_ptr<ArrayBuffer>& data) {
  unsigned int decompressedSizeOut = 0;
  uint8_t* decompressedData = rnzstd::decompress(
    static_cast<const uint8_t*>(data->data()),
    data->size(),
    decompressedSizeOut
  );

  // Convert to string
  std::string result(reinterpret_cast<char*>(decompressedData), decompressedSizeOut);
  delete[] decompressedData;

  return result;
}

} // namespace margelo::nitro::zstd
