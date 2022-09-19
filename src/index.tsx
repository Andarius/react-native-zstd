import { Buffer } from 'buffer';
import { NativeModules, Platform } from 'react-native';

const LINKING_ERROR =
  `The package 'react-native-zstd' doesn't seem to be linked. Make sure: \n\n` +
  Platform.select({ ios: "- You have run 'pod install'\n", default: '' }) +
  '- You rebuilt the app after installing the package\n' +
  '- You are not using Expo managed workflow\n';

const _Zstd = NativeModules.Zstd
  ? NativeModules.Zstd
  : new Proxy(
      {},
      {
        get() {
          throw new Error(LINKING_ERROR);
        },
      }
    );

interface IZstdNative {
  compress(data: string, compressLevel: number): Buffer;
  decompress(data: ArrayBuffer): string;
}

if ((global as any).__ZSTDProxy == null) {
  _Zstd.install();
}

const ZstdNative: IZstdNative = (global as any).__ZSTDProxy;

export function compress(data: string, compressLevel: number = 3): Buffer {
  const out = ZstdNative.compress(data, compressLevel);
  return Buffer.from(out);
}

export function decompress(data: Buffer): string {
  const out = ZstdNative.decompress(data.buffer);
  return out;
}
