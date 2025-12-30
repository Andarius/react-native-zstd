# react-native-zstd

ZSTD compression bindings for React Native, powered by [Nitro Modules](https://github.com/mrousavy/nitro).

## Installation

```sh
yarn add react-native-zstd react-native-nitro-modules
```

For iOS, run `pod install` in your `ios` directory.

## Usage

```ts
import { compress, decompress } from 'react-native-zstd';

// Compress a string (returns ArrayBuffer)
const compressed = compress('Hello World!', 3);

// Decompress back to string
const decompressed = decompress(compressed);
console.log(decompressed); // "Hello World!"
```

### API

#### `compress(data: string, compressionLevel?: number): ArrayBuffer`

Compresses a UTF-8 string using ZSTD.

- `data` - The string to compress
- `compressionLevel` - Compression level (default: 3). Higher = better compression, slower speed.

#### `decompress(data: ArrayBuffer): string`

Decompresses ZSTD-compressed data back to a UTF-8 string.

- `data` - The compressed data as an ArrayBuffer

## Contributing

See the [contributing guide](CONTRIBUTING.md) to learn how to contribute to the repository and the development workflow.

## License

MIT

---

Made with [create-react-native-library](https://github.com/callstack/react-native-builder-bob)
