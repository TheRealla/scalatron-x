# Scalatron-X — VST3 Build Instructions

## Prerequisites
- CMake 3.22+
- C++17 compiler (MSVC 2019+, Clang 12+, or GCC 10+)
- Git

## One-time setup

```bash
git clone https://github.com/YOUR_REPO/ScalatronX.git
cd ScalatronX
git submodule add https://github.com/juce-framework/JUCE.git JUCE
git submodule update --init --recursive
```

## Build (all platforms)

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --target ScalatronX_VST3
```

## Output locations

| Platform | Path |
|----------|------|
| Windows  | `build/ScalatronX_artefacts/Release/VST3/Scalatron-X.vst3` |
| macOS    | `build/ScalatronX_artefacts/Release/VST3/Scalatron-X.vst3` |
| Linux    | `build/ScalatronX_artefacts/Release/VST3/Scalatron-X.vst3` |

`COPY_PLUGIN_AFTER_BUILD TRUE` in CMakeLists.txt also copies it to your system VST3 folder automatically.

## Project structure

```
ScalatronX/
├── CMakeLists.txt
├── JUCE/                          ← git submodule
└── Source/
    ├── ScalatronCore.h            ← NTSC clock synthesis engine
    ├── ScalatronXAudioProcessor.h
    ├── ScalatronXAudioProcessor.cpp
    ├── ScalatronXAudioProcessorEditor.h
    └── ScalatronXAudioProcessorEditor.cpp
```

## Fixes applied vs original code

| Issue | Fix |
|-------|-----|
| `attachToParameter()` does not exist in JUCE 6+ | Replaced with `SliderParameterAttachment` |
| `setLookAndFeel(nullptr)` missing in destructor body | Moved before attachment reset, attachments reset first |
| `createPluginFilter()` factory missing from .cpp | Added at bottom of Processor.cpp |
| No `isMidiEffect()` override | Added (returns false) |
| Preset box out-of-range guard missing in `setCurrentProgram` | Added bounds check |
| `getStateInformation` / `setStateInformation` were empty stubs | Implemented with MemoryOutputStream/InputStream |
| Tone-tab `applyGain` called before `core.processBlock` filled buffer | Reordered: processBlock first, tilt after |
