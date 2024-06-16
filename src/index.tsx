import { Platform } from 'react-native';

const LINKING_ERROR =
  `The package 'react-native-zstd' doesn't seem to be linked. Make sure: \n\n` +
  Platform.select({ ios: "- You have run 'pod install'\n", default: '' }) +
  '- You rebuilt the app after installing the package\n' +
  '- You are not using Expo managed workflow\n';

// @ts-expect-error
const isTurboModuleEnabled = global.__turboModuleProxy != null;

const _Zstd = isTurboModuleEnabled
  ? require('./NativeZstd').default
  : new Proxy(
      {},
      {
        get() {
          throw new Error(LINKING_ERROR);
        },
      }
    );

let ZstdNative: any;

if (!isTurboModuleEnabled) {
  if ((global as any).__ZSTDProxy == null) {
    _Zstd.install();
  }
  ZstdNative = (global as any).__ZSTDProxy;
} else {
  ZstdNative = _Zstd;
}

export function compress(data: string, compressLevel: number = 3): Uint8Array {
  const out = ZstdNative.compress(data, compressLevel);
  return out;
}

export function decompress(data: Uint8Array): string {
  const out = ZstdNative.decompress(data);
  return out;
}
