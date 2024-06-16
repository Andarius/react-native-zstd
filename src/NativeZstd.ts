import type { TurboModule } from 'react-native';
import { TurboModuleRegistry } from 'react-native';

import type { Int32 } from 'react-native/Libraries/Types/CodegenTypes';

export interface Spec extends TurboModule {
  compress(data: string, compressionLevel: number): ReadonlyArray<Int32>;
  decompress(data: ReadonlyArray<Int32>): string;
}

export default TurboModuleRegistry.getEnforcing<Spec>('Zstd');
