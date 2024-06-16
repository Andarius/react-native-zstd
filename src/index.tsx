import { Buffer } from 'buffer';

// const LINKING_ERROR =
//   `The package 'react-native-zstd' doesn't seem to be linked. Make sure: \n\n` +
//   Platform.select({ ios: "- You have run 'pod install'\n", default: '' }) +
//   '- You rebuilt the app after installing the package\n' +
//   '- You are not using Expo Go\n';

// // @ts-expect-error
// const isTurboModuleEnabled = global.__turboModuleProxy != null;

const ZstdModule = require('./NativeZstd').default

interface IZstdNative {
    compress(data: string, compressLevel: number): Buffer;
    decompress(data: Buffer): string;
}

// const ZstdNative: IZstdNative = ZstdModule
//   ? ZstdModule
//   : new Proxy(
//       {},
//       {
//         get() {
//           throw new Error(LINKING_ERROR);
//         },
//       }
//     );



// if ((global as any).__ZSTDProxy == null) {
//   _Zstd.install();
// }

// const ZstdNative: IZstdNative = (global as any).__ZSTDProxy;

export function compress(data: string, compressionLevel: number = 3): Buffer {
  const out = ZstdModule.compress(data, compressionLevel);
  return Buffer.from(out);
}

export function decompress(data: Buffer): string {
  const out = ZstdModule.decompress(data.buffer);
  return out;
}
