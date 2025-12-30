import { NitroModules } from 'react-native-nitro-modules';
import type { Zstd } from './Zstd.nitro';

const ZstdHybridObject = NitroModules.createHybridObject<Zstd>('Zstd');

export function compress(data: string, compressLevel: number = 3): ArrayBuffer {
  return ZstdHybridObject.compress(data, compressLevel);
}

export function decompress(data: ArrayBuffer): string {
  return ZstdHybridObject.decompress(data);
}
