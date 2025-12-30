#pragma once

#include "HybridZstdSpec.hpp"

namespace margelo::nitro::zstd {

class HybridZstd : public HybridZstdSpec {
public:
  HybridZstd() : HybridObject(TAG) {}

  std::shared_ptr<ArrayBuffer> compress(const std::string& data, double compressionLevel) override;
  std::string decompress(const std::shared_ptr<ArrayBuffer>& data) override;
};

} // namespace margelo::nitro::zstd
