# Scalatron-X (1974 Digital Recreation)

**Scalatron-X** is a high-fidelity digital recreation of the legendary 1974 polyphonic synthesizer architecture. [cite_start]It utilizes a precision-clocked frequency divider engine to emulate the unique "top-octave" synthesis used in early Motorola-based hardware prototypes[cite: 1, 2, 23].

## Technical Specifications

* [cite_start]**Core Engine**: Precision NTSC Master Clock running at exactly **3,579,545 Hz**[cite: 1, 2].
* [cite_start]**Polyphony**: Supports up to **240 simultaneous voices**[cite: 1, 17].
* [cite_start]**Synthesis Method**: 32-bit integer frequency division with flip-flop square wave generation[cite: 19, 20].
* [cite_start]**Output Stage**: Vintage soft-saturation modeling: $out = out / (1.0 + 0.25 * |out|)$[cite: 22, 23].
* [cite_start]**Sample Rate Support**: Adaptive processing compatible with modern DAW standards via JUCE[cite: 15, 25].

## Factory Preset Bank (250 Entries)

[cite_start]The plugin features a built-in library of 250 presets, categorized by historical significance and microtonal utility[cite: 33]:

| Bank Range | Category | Description |
| :--- | :--- | :--- |
| **1 – 50** | **Historical Attacks** | Recreations of the original 1974 factory defaults and Motorola lab cleans. |
| **51 – 120** | **EDO Assaults** | Fast-transient presets optimized for Equal Divisions of the Octave (31, 53, 72-EDO). |
| **121 – 170** | **JI Blades** | Long-release "Cloud" presets designed for Just-Intonation and Partch-style scales. |
| **171 – 210** | **Secor Grid** | Aggressive, percussive settings based on George Secor's generalized keyboard layouts. |
| **211 – 250** | **Exp. Cracks** | Ultra-short attack "cracks" for experimental and noise-based microtonalism. |

## Interface Controls

* [cite_start]**Tone Tabs**: Four classic filtering tilts: **Pure**, **Flute**, **Reed**, and **String**[cite: 31, 37].
* [cite_start]**Envelopes**: Independent **Attack** (0.001s – 2.0s) and **Release** (0.01s – 8.0s) controls[cite: 32, 50].
* [cite_start]**Tuning Monitor**: A vintage-style CRT display providing visual feedback on active voice status[cite: 52].

## Development Roadmap

1. **Phase 1**: (Completed) 250-preset core architecture and NTSC clock implementation.
2. **Phase 2**: (Active) Expansion to 1,000 preset capacity with algorithmic bank generation.
3. **Phase 3**: (Active) Category-based filtering and searchable preset browser.

---
*Built with JUCE and inspired by the original Motorola Scalatron prototypes (1974).*
