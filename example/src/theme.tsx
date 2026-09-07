import { Text as NativeText, StyleSheet } from 'react-native';
import type { TextProps } from 'react-native';

// Local tokens from obiapp-ui; no dependency on the component library.
export const lightColors = {
  background: 'hsl(0, 0%, 96%)',
  surface: 'white',
  text: 'black',
  muted: 'hsl(209, 13%, 35%)',
  border: 'rgba(0,0,0,0.08)',
  accent: 'hsl(210, 82%, 31%)',
  ink: 'white',
  primary: 'hsl(198, 98%, 39%)',
  secondary: 'hsl(168, 98%, 23%)',
};
export const darkColors = {
  background: '#0b1117',
  surface: '#16202a',
  text: '#f3f7fb',
  muted: '#d7e2ec',
  border: 'rgba(255,255,255,0.08)',
  accent: '#1b6f9e',
  ink: '#ffffff',
  primary: '#59c7f8',
  secondary: '#32d3ae',
};

export function Text({ style, ...props }: TextProps) {
  const flat = StyleSheet.flatten(style);
  const fontFamily =
    flat?.fontFamily ??
    (flat?.fontWeight === '600' || flat?.fontWeight === '700'
      ? 'Raleway-SemiBold'
      : 'Raleway-Regular');
  return <NativeText {...props} style={[style, { fontFamily }, styles.font]} />;
}

const styles = StyleSheet.create({ font: { fontWeight: 'normal' } });
