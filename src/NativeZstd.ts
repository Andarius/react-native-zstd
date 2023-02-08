import type { TurboModule } from 'react-native';
import { TurboModuleRegistry } from 'react-native';

export interface Spec extends TurboModule {
  compress(data: string, compressLevel: number): Buffer;
  decompress(data: ArrayBuffer): string;
}

export default TurboModuleRegistry.getEnforcing<Spec>('Zstd');
