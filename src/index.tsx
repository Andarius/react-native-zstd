import type { Int32 } from 'react-native/Libraries/Types/CodegenTypes';
import Zstd from './NativeZstd';

export function compress(
  data: string,
  compressLevel: number = 3
): ReadonlyArray<Int32> {
  const out = Zstd.compress(data, compressLevel);
  return out;
}

export function decompress(data: ReadonlyArray<Int32>): string {
  const out = Zstd.decompress(data);
  return out;
}
