# react-native-zstd
ZSTD bindings for React-Native


## Installation

```sh
npm install react-native-zstd
```
or
```sh
yarn add react-native-zstd
```

## Usage

```ts
import { compress, decompress } from "react-native-zstd";
import { Buffer } from 'buffer';

// Compress
const compressionLevel: number = 3;
const compressed: Buffer = compress("Hello World!", compressionLevel);
console.log(compressed.toString('base64'))

// Decompress
const decompressed: string = decompress(compressed);
console.log(`decompressed: ${decompressed}`)

```

## Contributing

See the [contributing guide](CONTRIBUTING.md) to learn how to contribute to the repository and the development workflow.

## License

MIT

---

Made with [create-react-native-library](https://github.com/callstack/react-native-builder-bob)
