import { Buffer } from 'buffer';
import { NativeModules, Platform } from 'react-native';

const LINKING_ERROR =
  `The package 'react-native-zstd' doesn't seem to be linked. Make sure: \n\n` +
  Platform.select({ ios: "- You have run 'pod install'\n", default: '' }) +
  '- You rebuilt the app after installing the package\n' +
  '- You are not using Expo managed workflow\n';

const Zstd = NativeModules.Zstd
  ? NativeModules.Zstd
  : new Proxy(
      {},
      {
        get() {
          throw new Error(LINKING_ERROR);
        },
      }
    );

export async function compress(
  data: string,
  compressLevel: number
): Promise<Buffer> {
  const out = await Zstd.compress(data, compressLevel);
  return Buffer.from(out);
}

export async function decompress(data: Buffer): Promise<string> {
  return Zstd.decompress(data);
}
