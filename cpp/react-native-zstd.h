#ifndef EXAMPLE_H
#define EXAMPLE_H

namespace example {
  extern "C" char* compress(const char* data, int compressionLevel, unsigned int &compressedSizeOut);
  extern "C" char* decompress(const char* data, unsigned int &decompressedSizeOut);
}

#endif /* EXAMPLE_H */
