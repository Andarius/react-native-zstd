# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build & Development Commands

```bash
# Install dependencies (use yarn, not npm - monorepo requirement)
yarn

# Bootstrap project (init git submodules + install example deps)
yarn bootstrap

# Build the library (runs nitrogen codegen + module + typescript)
yarn prepare

# Run nitrogen codegen only
yarn nitrogen

# Type-check
yarn typecheck

# Lint (ESLint + Prettier)
yarn lint
yarn lint --fix

# Run tests
yarn test

# Example app commands
yarn example start          # Start Metro bundler
yarn example android        # Run on Android
yarn example ios            # Run on iOS

# iOS CocoaPods
yarn pod:install            # Install pods for example app
yarn pod:deintegrate        # Clean pods

# Release (uses release-it)
yarn release
```

## Architecture

This is a React Native Nitro Module providing ZSTD compression/decompression bindings.

### Layer Structure

1. **TypeScript API** (`src/index.tsx`): Exports `compress(data, level)` and `decompress(data)` functions
2. **Nitro Spec** (`src/Zstd.nitro.ts`): HybridObject interface definition used by Nitrogen codegen
3. **Platform Implementations**:
   - iOS: `ios/HybridZstd.swift` - Swift implementation extending `HybridZstdSpec`
   - iOS: `ios/ZstdBridge.mm` - Objective-C++ bridge to call C++ from Swift
   - Android: `android/src/main/java/com/margelo/nitro/zstd/HybridZstd.kt` - Kotlin implementation
   - Android: `android/src/main/cpp/cpp-adapter.cpp` - JNI bridge to C++
4. **Core C++** (`cpp/react-native-zstd.cpp`): Platform-agnostic ZSTD wrapper in `rnzstd` namespace
5. **ZSTD Library** (`externals/zstd/`): Git submodule with the official zstd library

### Data Flow

- Input: UTF-8 string -> compress -> `ArrayBuffer`
- Output: `ArrayBuffer` -> decompress -> UTF-8 string

### Native Build Configuration

- **iOS**: Configured via `react-native-zstd.podspec`, uses nitrogen autolinking
- **Android**: Uses CMake (`android/CMakeLists.txt`) with nitrogen autolinking, builds ZSTD as static library

### Nitrogen Codegen

Generated code lives in `nitrogen/generated/`:
- `ios/` - Swift specs and C++ bridges
- `android/` - Kotlin specs and JNI bindings
- `shared/` - Shared C++ code

Config is in `nitro.json`. Run `yarn nitrogen` to regenerate.

## Commit Convention

Uses conventional commits: `fix:`, `feat:`, `refactor:`, `docs:`, `test:`, `chore:`
