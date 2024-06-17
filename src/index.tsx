import { NativeModules, Platform } from 'react-native';

const LINKING_ERROR =
  `The package 'react-native-zstd' doesn't seem to be linked. Make sure: \n\n` +
  Platform.select({ ios: "- You have run 'pod install'\n", default: '' }) +
  '- You rebuilt the app after installing the package\n' +
  '- You are not using Expo Go\n';

// @ts-expect-error
const isTurboModuleEnabled = global.__turboModuleProxy != null;

const ZstdModule = isTurboModuleEnabled
  ? require('./NativeZstd').default
  : NativeModules.Zstd;

const Zstd = ZstdModule
  ? ZstdModule
  : new Proxy(
      {},
      {
        get() {
          throw new Error(LINKING_ERROR);
        },
      }
    );

export function compress(data: string, compressLevel: number = 3): Uint8Array {
  const out = Zstd.compress(data, compressLevel);
  return out;
}

export function decompress(data: Uint8Array): string {
  const out = Zstd.decompress(data);
  return out;
}
