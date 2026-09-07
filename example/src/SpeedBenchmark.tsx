import * as React from 'react';
import { Buffer } from 'buffer';
import { Pressable, StyleSheet, View } from 'react-native';
import { compress, decompress } from 'react-native-zstd';
import { compress as compressJS, decompress as decompressJS } from 'zstdify';
import { Text } from './theme';

const largeSample = JSON.stringify(
  Array.from({ length: 500 }, (_, id) => ({
    id,
    activity: ['running', 'cycling', 'swimming'][id % 3],
    duration: 1200 + ((id * 137) % 6000),
    distance: (id * 7919) % 20000,
    timestamp: 1700000000 + id * 3600,
  }))
);

type Measurement = { milliseconds: number; iterations: number };
type EngineResult = {
  compression: Measurement;
  decompression: Measurement;
  size: number;
};
type Result = { bytes: number; native: EngineResult; javascript: EngineResult };
const yieldToUI = () => new Promise<void>((resolve) => setTimeout(resolve, 0));

async function measure(
  operation: () => void,
  cancelled: () => boolean
): Promise<Measurement> {
  for (let i = 0; i < 5; i++) {
    if (cancelled()) throw new Error('Benchmark cancelled');
    operation();
    await yieldToUI();
  }
  let elapsed = 0;
  let iterations = 0;
  while ((elapsed < 150 || iterations < 3) && iterations < 10000) {
    if (cancelled()) throw new Error('Benchmark cancelled');
    const start = performance.now();
    do {
      operation();
      iterations++;
    } while (performance.now() - start < 8 && iterations < 10000);
    elapsed += performance.now() - start;
    await yieldToUI();
  }
  if (elapsed <= 0)
    throw new Error('Timer resolution is too low; use a larger input.');
  return { milliseconds: elapsed / iterations, iterations };
}

export function SpeedBenchmark({
  text,
  level,
  colors,
}: {
  text: string;
  level: number;
  colors: {
    surface: string;
    border: string;
    text: string;
    muted: string;
    accent: string;
    ink: string;
  };
}) {
  const [details, setDetails] = React.useState(false);
  const [large, setLarge] = React.useState(true);
  const [running, setRunning] = React.useState(false);
  const [result, setResult] = React.useState<Result | null>(null);
  const [error, setError] = React.useState<string | null>(null);
  const generation = React.useRef({ value: 0 });
  React.useEffect(() => {
    const lifecycle = generation.current;
    lifecycle.value++;
    setResult(null);
    setError(null);
    setRunning(false);
    return () => {
      lifecycle.value++;
    };
  }, [text, level, large]);

  async function run() {
    const token = ++generation.current.value;
    const cancelled = () => token !== generation.current.value;
    setRunning(true);
    setResult(null);
    setError(null);
    try {
      await yieldToUI();
      if (cancelled()) return;
      const input = large ? largeSample : text;
      let compressed = compress(input, level);
      let restored = '';
      const compression = await measure(() => {
        compressed = compress(input, level);
      }, cancelled);
      const decompression = await measure(() => {
        restored = decompress(compressed);
      }, cancelled);
      if (restored !== input) throw new Error('Native round trip failed.');
      let jsCompressed = compressJS(
        Uint8Array.from(Buffer.from(input, 'utf8')),
        { level }
      );
      const jsCompression = await measure(() => {
        jsCompressed = compressJS(Uint8Array.from(Buffer.from(input, 'utf8')), {
          level,
        });
      }, cancelled);
      const nativeFrame = new Uint8Array(compressed);
      const jsDecompression = await measure(() => {
        restored = Buffer.from(decompressJS(nativeFrame)).toString('utf8');
      }, cancelled);
      const jsFrame = new ArrayBuffer(jsCompressed.byteLength);
      new Uint8Array(jsFrame).set(jsCompressed);
      if (
        restored !== input ||
        decompress(jsFrame) !== input ||
        Buffer.from(decompressJS(jsCompressed)).toString('utf8') !== input
      ) {
        throw new Error('JavaScript/native interoperability check failed.');
      }
      if (!cancelled())
        setResult({
          bytes: Buffer.byteLength(input, 'utf8'),
          native: { compression, decompression, size: compressed.byteLength },
          javascript: {
            compression: jsCompression,
            decompression: jsDecompression,
            size: jsCompressed.byteLength,
          },
        });
    } catch (failure) {
      if (!cancelled())
        setError(failure instanceof Error ? failure.message : String(failure));
    } finally {
      if (!cancelled()) setRunning(false);
    }
  }

  return (
    <View
      style={[
        styles.card,
        { backgroundColor: colors.surface, borderColor: colors.border },
      ]}
    >
      <Text style={[styles.title, { color: colors.text }]}>
        Native vs JavaScript
      </Text>
      <Text style={[styles.note, { color: colors.muted }]}>
        Level {level} ·{' '}
        {large
          ? `${Buffer.byteLength(largeSample).toLocaleString()} B of generated JSON`
          : `${Buffer.byteLength(text).toLocaleString()} B of your input`}
      </Text>
      <View style={styles.row}>
        {[false, true].map((value) => (
          <Pressable
            key={String(value)}
            accessibilityRole="button"
            accessibilityState={{
              selected: large === value,
              disabled: running,
            }}
            disabled={running}
            onPress={() => setLarge(value)}
            style={[
              styles.button,
              {
                borderColor: colors.border,
                backgroundColor:
                  large === value ? colors.accent : colors.surface,
              },
            ]}
          >
            <Text style={{ color: large === value ? colors.ink : colors.text }}>
              {value ? 'Large JSON' : 'Your input'}
            </Text>
          </Pressable>
        ))}
      </View>
      <Pressable
        accessibilityRole="button"
        accessibilityState={{ disabled: running, busy: running }}
        disabled={running}
        onPress={run}
        style={[
          styles.button,
          running && styles.disabled,
          {
            backgroundColor: colors.accent,
            borderColor: colors.accent,
          },
        ]}
      >
        <Text style={[styles.title, { color: colors.ink }]}>
          {running ? 'Measuring…' : 'Run benchmark'}
        </Text>
      </Pressable>
      {error && (
        <Text accessibilityRole="alert" style={{ color: colors.text }}>
          {error}
        </Text>
      )}
      {result && (
        <View accessibilityLiveRegion="polite" style={styles.results}>
          {(['native', 'javascript'] as const).map((engine) => (
            <View
              key={engine}
              style={[styles.engine, { borderColor: colors.border }]}
            >
              <Text style={[styles.title, { color: colors.text }]}>
                {engine === 'native'
                  ? 'Native · react-native-zstd'
                  : 'JavaScript · zstdify'}
              </Text>
              <Text style={[styles.note, { color: colors.muted }]}>
                {result[engine].size.toLocaleString()} B compressed
              </Text>
              {(['compression', 'decompression'] as const).map((kind) => (
                <View key={kind} style={styles.results}>
                  <Text style={{ color: colors.text }}>
                    {kind === 'compression' ? 'Compress' : 'Decompress'}
                  </Text>
                  <Text style={[styles.value, { color: colors.text }]}>
                    {result[engine][kind].milliseconds.toFixed(3)} ms / op
                  </Text>
                  <Text style={[styles.note, { color: colors.muted }]}>
                    {result.bytes === 0
                      ? 'Throughput N/A for empty input'
                      : `${(result.bytes / result[engine][kind].milliseconds / 1000).toFixed(1)} MB/s`}{' '}
                    · {result[engine][kind].iterations.toLocaleString()} runs
                  </Text>
                </View>
              ))}
            </View>
          ))}
          <Text style={[styles.note, { color: colors.text }]}>
            ✓ Both round trips and cross-decoding verified
          </Text>
        </View>
      )}
      <Pressable
        accessibilityRole="button"
        accessibilityState={{ expanded: details }}
        onPress={() => setDetails(!details)}
        style={styles.button}
      >
        <Text style={{ color: colors.muted }}>
          {details ? 'Hide methodology' : 'Methodology'}
        </Text>
      </Pressable>
      {details && (
        <Text style={[styles.note, { color: colors.muted }]}>
          5 warm-ups; at least 3 measured runs targeting 150 ms, capped at
          10,000. UI pauses excluded; UTF-8 conversion included. MB/s uses
          original bytes. Both decoders read the native frame. Equal levels can
          produce different sizes. Use a release build on a physical device for
          representative results.
        </Text>
      )}
    </View>
  );
}

const styles = StyleSheet.create({
  engine: { gap: 12, borderTopWidth: 1, paddingTop: 16 },
  disabled: { opacity: 0.6 },
  card: { padding: 16, borderWidth: 1, borderRadius: 16, gap: 8 },
  title: { fontSize: 16, fontWeight: '600' },
  note: { fontSize: 13, lineHeight: 19 },
  row: { flexDirection: 'row', flexWrap: 'wrap', gap: 8 },
  button: {
    minHeight: 48,
    paddingHorizontal: 16,
    paddingVertical: 8,
    borderWidth: 1,
    borderRadius: 24,
    alignItems: 'center',
    justifyContent: 'center',
  },
  results: { gap: 8 },
  value: {
    fontSize: 18,
    fontFamily: 'Montserrat-SemiBold',
    fontVariant: ['tabular-nums'],
  },
});
