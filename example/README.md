# Expo development example

This example uses Expo SDK 57 and a development build containing the local native library. Expo Go cannot load it.
Run these commands from the repository root:

```sh
yarn install --immutable
yarn prepare
yarn example prebuild --clean
yarn example android
# Or: yarn example ios
```

Use `yarn example start` for subsequent JavaScript development. Rebuild after changing native code.
The example's `ios/` and `android/` directories are generated and ignored. Keep native configuration in app config and config plugins; `prebuild --clean` replaces generated projects.

Validate with `yarn lint`, `yarn typecheck`, `yarn prepare`, `yarn example export`, and `cd example && npx expo-doctor@1.20.4`.
After prebuild, `yarn example build:android` builds an arm64 debug APK and `yarn example build:ios` compiles an unsigned simulator app. CI runs both native builds.

Initialize the native dependency first with `git submodule update --init --recursive`. Run `yarn test --runInBand` for the existing Jest suite.

The playground includes editable text, JSON, repetition, Unicode, and empty-input samples, with levels 1, 3, and 9 and round-trip verification. The benchmark compares native Zstd with the example-only `zstdify` development dependency using your input or generated JSON. It reports timings, throughput, output sizes, and cross-decoding checks; expand Methodology for measurement details. Use a release build on a physical device for representative performance.

Theme colors and bundled Raleway/Montserrat fonts follow obiapp-ui without importing it. Font licenses are included in `assets/fonts/`.
