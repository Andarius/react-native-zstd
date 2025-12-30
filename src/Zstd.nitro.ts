import type { HybridObject } from 'react-native-nitro-modules';

export interface Zstd extends HybridObject<{ ios: 'c++'; android: 'c++' }> {
  compress(data: string, compressionLevel: number): ArrayBuffer;
  decompress(data: ArrayBuffer): string;
}
