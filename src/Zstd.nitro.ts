import type { HybridObject } from 'react-native-nitro-modules';

export interface Zstd
  extends HybridObject<{ ios: 'swift'; android: 'kotlin' }> {
  compress(data: string, compressionLevel: number): ArrayBuffer;
  decompress(data: ArrayBuffer): string;
}
