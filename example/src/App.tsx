import * as React from 'react';
import { Buffer } from 'buffer';
import {
  Keyboard,
  Platform,
  Pressable,
  ScrollView,
  StatusBar,
  StyleSheet,
  TextInput,
  useColorScheme,
  View,
} from 'react-native';
import { compress, decompress } from 'react-native-zstd';
import { SpeedBenchmark } from './SpeedBenchmark';
import { useFonts } from 'expo-font';
import { Text, lightColors, darkColors } from './theme';

const samples = [
  {
    name: 'Text',
    text: 'Small bytes. Same story.\nZstandard compresses your data and brings it back exactly as you wrote it.',
  },
  {
    name: 'JSON',
    text: JSON.stringify(
      {
        events: Array.from({ length: 12 }, (_, i) => ({
          id: i + 1,
          type: 'workout',
          activity: 'running',
          duration: 1800,
        })),
      },
      null,
      2
    ),
  },
  { name: 'Repetition', text: 'run · recover · repeat\n'.repeat(80) },
  {
    name: 'Unicode',
    text: Array.from({ length: 24 }, (_, index) =>
      [
        `🌍 Journal de voyage · Étape ${index + 1}`,
        'Bonjour ! Nous découvrons les cafés du quartier, les marchés et les jardins. ☕',
        'こんにちは世界。今日は街を歩いて、新しい景色とおいしい料理を楽しみます。🍜',
        'مرحبا بالعالم! نستكشف المدينة ونتعرف على أصدقاء جدد ونستمتع بالطعام المحلي.',
        '¡Hola, mundo! Mañana visitaremos el museo y pasearemos junto al río. ✨',
      ].join('\n')
    ).join('\n\n'),
  },
  { name: 'Empty', text: '' },
];
const levels = [1, 3, 9];

export default function App() {
  const dark = useColorScheme() === 'dark';
  const colors = dark ? darkColors : lightColors;
  const [fontsLoaded, fontError] = useFonts({
    'Raleway-Regular': require('../assets/fonts/Raleway-Regular.ttf'),
    'Raleway-SemiBold': require('../assets/fonts/Raleway-SemiBold.ttf'),
    'Montserrat-SemiBold': require('../assets/fonts/Montserrat-SemiBold.ttf'),
  });
  const [text, setText] = React.useState(samples[0]!.text);
  const [level, setLevel] = React.useState(3);
  const [expanded, setExpanded] = React.useState(false);
  const result = React.useMemo(() => {
    try {
      const compressed = compress(text, level);
      const restored = decompress(compressed);
      return {
        original: Buffer.byteLength(text, 'utf8'),
        compressed: compressed.byteLength,
        base64: Buffer.from(compressed).toString('base64'),
        restored,
        matches: restored === text,
      };
    } catch (error) {
      return { error: error instanceof Error ? error.message : String(error) };
    }
  }, [text, level]);
  const foreground = { color: colors.text };
  const panel = { backgroundColor: colors.surface, borderColor: colors.border };

  function chip(label: string, selected: boolean, onPress: () => void) {
    return (
      <Pressable
        key={label}
        accessibilityRole="button"
        accessibilityState={{ selected }}
        onPress={onPress}
        style={({ pressed }) => [
          styles.chip,
          {
            backgroundColor: selected ? colors.accent : colors.surface,
            borderColor: selected ? colors.accent : colors.border,
            opacity: pressed ? 0.65 : 1,
          },
        ]}
      >
        <Text
          style={[
            styles.chipText,
            { color: selected ? colors.ink : colors.text },
          ]}
        >
          {label}
        </Text>
      </Pressable>
    );
  }

  if (!fontsLoaded && !fontError)
    return (
      <View style={[styles.loading, { backgroundColor: colors.background }]} />
    );

  return (
    <ScrollView
      style={{ backgroundColor: colors.background }}
      contentContainerStyle={styles.content}
      contentInsetAdjustmentBehavior="automatic"
      automaticallyAdjustKeyboardInsets
      keyboardDismissMode="on-drag"
      keyboardShouldPersistTaps="handled"
    >
      <StatusBar barStyle={dark ? 'light-content' : 'dark-content'} />
      <View style={styles.header}>
        <Text style={[styles.title, { color: colors.primary }]}>
          Zstd playground
        </Text>
      </View>
      <Text style={[styles.label, foreground]}>Sample</Text>
      <View style={styles.chips}>
        {samples.map((sample) =>
          chip(sample.name, text === sample.text, () => {
            Keyboard.dismiss();
            setText(sample.text);
          })
        )}
      </View>
      <TextInput
        accessibilityLabel="Data to compress"
        multiline
        maxLength={20000}
        textAlignVertical="top"
        value={text}
        onChangeText={setText}
        placeholder="Type something to compress…"
        placeholderTextColor={colors.muted}
        style={[styles.input, panel, foreground]}
      />

      <View style={styles.section}>
        <Text style={[styles.label, foreground]}>Compression level</Text>
        <View style={styles.chips}>
          {levels.map((value) =>
            chip(
              `${value} · ${value === 1 ? 'Fast' : value === 3 ? 'Balanced' : 'Compact'}`,
              level === value,
              () => {
                Keyboard.dismiss();
                setLevel(value);
              }
            )
          )}
        </View>
      </View>
      {'error' in result ? (
        <Text accessibilityRole="alert" style={foreground}>
          Compression failed: {result.error}
        </Text>
      ) : (
        <>
          <View style={[styles.result, { backgroundColor: colors.accent }]}>
            <Text style={[styles.savings, { color: colors.ink }]}>
              {result.original === 0
                ? 'Empty input'
                : `${Math.abs((1 - result.compressed / result.original) * 100).toFixed(1)}% ${result.compressed <= result.original ? 'smaller' : 'larger'}`}
            </Text>
            <View style={styles.metrics}>
              <View>
                <Text style={[styles.metricLabel, { color: colors.ink }]}>
                  ORIGINAL
                </Text>
                <Text style={[styles.metric, { color: colors.ink }]}>
                  {result.original.toLocaleString()} B
                </Text>
              </View>
              <Text style={[styles.arrow, { color: colors.ink }]}>→</Text>
              <View>
                <Text style={[styles.metricLabel, { color: colors.ink }]}>
                  COMPRESSED
                </Text>
                <Text style={[styles.metric, { color: colors.ink }]}>
                  {result.compressed.toLocaleString()} B
                </Text>
              </View>
            </View>
            <Text style={[styles.caption, { color: colors.ink }]}>
              {result.compressed >= result.original
                ? 'Small inputs can grow because the compressed frame includes metadata.'
                : 'Size compares UTF-8 input with the raw compressed bytes, before Base64 encoding.'}
            </Text>
          </View>
          <View style={[styles.verification, panel]}>
            <Text
              accessibilityRole="text"
              style={[styles.label, { color: colors.secondary }]}
            >
              {result.matches
                ? '✓ Round trip verified'
                : '✕ Round trip mismatch'}
            </Text>
          </View>
          <SpeedBenchmark text={text} level={level} colors={colors} />
          {chip(expanded ? 'Hide output' : 'Inspect output', expanded, () =>
            setExpanded(!expanded)
          )}
          {expanded && (
            <View style={[styles.output, panel]}>
              <Text style={[styles.label, foreground]}>
                Compressed / Base64
              </Text>
              <Text selectable style={[styles.code, foreground]}>
                {result.base64}
              </Text>
              <Text style={[styles.label, foreground]}>
                Decompressed / UTF-8
              </Text>
              <Text selectable style={[styles.code, foreground]}>
                {result.restored || '(empty string)'}
              </Text>
            </View>
          )}
        </>
      )}
    </ScrollView>
  );
}

const styles = StyleSheet.create({
  loading: { flex: 1 },
  content: {
    padding: 16,
    paddingTop:
      Platform.OS === 'android' ? (StatusBar.currentHeight ?? 0) + 12 : 12,
    paddingBottom: 48,
    maxWidth: 640,
    width: '100%',
    alignSelf: 'center',
    gap: 12,
  },
  header: { marginBottom: 4 },
  title: {
    fontSize: 24,
    lineHeight: 30,
    fontFamily: 'Montserrat-SemiBold',
  },
  label: { fontSize: 16, fontWeight: '600' },
  chips: { flexDirection: 'row', flexWrap: 'wrap', gap: 8 },
  chip: {
    minHeight: 48,
    borderWidth: 1,
    borderRadius: 24,
    paddingHorizontal: 10,
    paddingVertical: 8,
    alignItems: 'center',
    justifyContent: 'center',
  },
  chipText: { fontSize: 14, fontWeight: '600' },
  input: {
    borderWidth: 1,
    borderRadius: 16,
    padding: 16,
    height: 88,
    fontSize: 15,
    lineHeight: 22,
  },
  caption: { fontSize: 13, lineHeight: 19 },
  section: { gap: 8, marginTop: 4 },
  result: { padding: 16, borderRadius: 16, gap: 8 },
  savings: { fontSize: 26, fontWeight: '700', letterSpacing: -1 },
  metrics: {
    flexDirection: 'row',
    alignItems: 'center',
    justifyContent: 'space-between',
    flexWrap: 'wrap',
    gap: 12,
  },
  metricLabel: {
    fontSize: 10,
    letterSpacing: 1,
    fontWeight: '600',
  },
  metric: {
    fontSize: 24,
    fontFamily: 'Montserrat-SemiBold',
    marginTop: 4,
    fontVariant: ['tabular-nums'],
  },
  arrow: { fontSize: 24 },
  verification: { paddingVertical: 4, paddingHorizontal: 12, borderRadius: 12 },
  output: { padding: 20, borderWidth: 1, borderRadius: 16, gap: 16 },
  code: {
    fontFamily: Platform.OS === 'ios' ? 'Menlo' : 'monospace',
    fontSize: 12,
    lineHeight: 19,
  },
});
