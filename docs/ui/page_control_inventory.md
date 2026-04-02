# SY-1000 FloorBoard — Per-Page Control Inventory

> Generated 2026-03-18 from source code analysis of all stompbox_*.cpp,
> soundSource_inst*.cpp, menuPage_*.cpp, and qml/*.qml files.
> **Do not edit by hand** — re-generate from source + `midi.xml` when controls change.
> To regenerate: grep `addKnob\|addSwitch\|addComboBox\|addRange\|addTarget` across stompbox_*.cpp and cross-reference with midi.xml addresses.

This document maps **every UI page → every control → MIDI address** so we know
exactly what each page should display and what SysEx commands it sends/receives.

**Address format:** `hex0 hex1 hex2 hex3` — these four bytes form the SysEx
parameter address routed through `MidiTable.cpp` → `midi.xml`.

**Guitar/Bass mode:** hex1 shifts between guitar ("00") and bass ("02") for
patch parameters; system parameters shift hex1 between "01"/"04" (guitar) and
"08"/"0B" (bass).

---

## Table of Contents

1. [Stomp/Effect Blocks](#1-stompeffect-blocks)
2. [Instrument/Source Pages](#2-instrumentsource-pages)
3. [Left Navigation Pages](#3-left-navigation-pages)
4. [Top Navigation & Status Bar](#4-top-navigation--status-bar)
5. [Signal Chain Floor Layout](#5-signal-chain-floor-layout)

---

# 1. Stomp/Effect Blocks

All stomp/effect blocks have been migrated to QML via `setQmlPage()`.

---

## AMP (stompbox_amp.cpp → AmpPanel.qml)

- **Power switch:** 10 00 38 00
- **Tabs:** 1

| Control | Type | hex0 | hex1 | hex2 | hex3 | Conditional? | Notes |
|---------|------|------|------|------|------|-------------|-------|
| Amp Type | ComboBox | 10 | 00 | 38 | 01 | No | Model selector |
| Gain | FilmstripKnob | 10 | 00 | 38 | 02 | No | |
| Bass | FilmstripKnob | 10 | 00 | 38 | 03 | No | Tone stack |
| Middle | FilmstripKnob | 10 | 00 | 38 | 04 | No | Tone stack |
| Effect Level | FilmstripKnob | 10 | 00 | 38 | 05 | No | |
| Treble | FilmstripKnob | 10 | 00 | 38 | 06 | No | Tone stack |
| Presence | FilmstripKnob | 10 | 00 | 38 | 07 | No | Tone stack |
| Resonance | FilmstripKnob | 10 | 00 | 38 | 08 | No | |
| Master | FilmstripKnob | 10 | 00 | 38 | 09 | No | |
| Bright Switch | SySwitch | 10 | 00 | 38 | 0A | No | |
| Gain Select | ComboBox | 10 | 00 | 38 | 0B | No | |
| Solo Switch | SySwitch | 10 | 00 | 38 | 0C | No | |
| Solo Level | FilmstripKnob | 10 | 00 | 38 | 0D | No | |
| Speaker Type | ComboBox | 10 | 00 | 38 | 0E | No | |
| Speaker Sensitivity | ComboBox | 10 | 00 | 38 | 0F | No | |
| Mic Type | ComboBox | 10 | 00 | 38 | 10 | No | |
| Mic Position | FilmstripKnob | 10 | 00 | 38 | 11 | No | |
| Mic Distance | FilmstripKnob | 10 | 00 | 38 | 12 | No | |
| Direct Level | FilmstripKnob | 10 | 00 | 38 | 13 | No | |

---

## COMPRESSOR (stompbox_cs.cpp → CsPanel.qml)

- **Power switch:** 10 00 36 00
- **Tabs:** 1

| Control | Type | hex0 | hex1 | hex2 | hex3 | Conditional? |
|---------|------|------|------|------|------|-------------|
| Type | ComboBox | 10 | 00 | 36 | 01 | No |
| Attack | FilmstripKnob | 10 | 00 | 36 | 02 | No |
| Sustain | FilmstripKnob | 10 | 00 | 36 | 03 | No |
| Level | FilmstripKnob | 10 | 00 | 36 | 04 | No |
| Output | FilmstripKnob | 10 | 00 | 36 | 05 | No |
| Release | FilmstripKnob | 10 | 00 | 36 | 06 | No |

---

## DISTORTION (stompbox_ds.cpp → DsPanel.qml)

- **Power switch:** 10 00 37 00
- **Tabs:** 1

| Control | Type | hex0 | hex1 | hex2 | hex3 | Conditional? | Notes |
|---------|------|------|------|------|------|-------------|-------|
| Type | ComboBox | 10 | 00 | 37 | 01 | No | OD/DS model |
| Drive | FilmstripKnob | 10 | 00 | 37 | 02 | No | |
| Tone | FilmstripKnob | 10 | 00 | 37 | 03 | No | |
| Bottom | FilmstripKnob | 10 | 00 | 37 | 04 | No | Changes label per type |
| Level | FilmstripKnob | 10 | 00 | 37 | 05 | No | |
| Direct Mix | FilmstripKnob | 10 | 00 | 37 | 06 | No | Changes label per type |
| Solo SW | SySwitch | 10 | 00 | 37 | 07 | No | |
| Solo Level | FilmstripKnob | 10 | 00 | 37 | 08 | No | |

---

## NOISE SUPPRESSOR (stompbox_ns.cpp → NsPanel.qml)

- **Power switch:** 10 00 39 00
- **Tabs:** 1

| Control | Type | hex0 | hex1 | hex2 | hex3 | Conditional? |
|---------|------|------|------|------|------|-------------|
| Threshold | FilmstripKnob | 10 | 00 | 39 | 01 | No |
| Release | FilmstripKnob | 10 | 00 | 39 | 02 | No |
| Reduction | ComboBox | 10 | 00 | 39 | 03 | No |

---

## EQ 1 (stompbox_eq1.cpp → Eq1Panel.qml)

- **Power switch:** 10 00 3A 00
- **Tabs:** 1

| Control | Type | hex0 | hex1 | hex2 | hex3 | Conditional? |
|---------|------|------|------|------|------|-------------|
| ParaEQ (4 bands + master) | ParaEqComponent | 10 | 00 | 3A | 01–0F | No |

---

## EQ 2 (stompbox_eq2.cpp → Eq2Panel.qml)

- **Power switch:** 10 00 3B 00
- **Tabs:** 1

| Control | Type | hex0 | hex1 | hex2 | hex3 | Conditional? |
|---------|------|------|------|------|------|-------------|
| ParaEQ (4 bands + master) | ParaEqComponent | 10 | 00 | 3B | 01–0F | No |

---

## DELAY 1 (stompbox_dd1.cpp → Dd1Panel.qml)

- **Power switch:** 10 00 3C 00
- **Tabs:** 1 (type-conditional layout)

| Control | Type | hex0 | hex1 | hex2 | hex3 | Conditional? | Notes |
|---------|------|------|------|------|------|-------------|-------|
| Type | ComboBox | 10 | 00 | 3C | 01 | No | Delay type selector |
| Time | DataKnob | 10 | 00 | 3C | 02 | Yes | Varies by type |
| Feedback | FilmstripKnob | 10 | 00 | 3C | 06 | Yes | |
| HighCut | FilmstripKnob | 10 | 00 | 3C | 07 | Yes | |
| Effect Level | FilmstripKnob | 10 | 00 | 3C | 08 | Yes | |
| Direct Level | FilmstripKnob | 10 | 00 | 3C | 09 | Yes | |
| Time1 (Dual) | DataKnob | 10 | 00 | 3C | 0A | Yes | Dual mode |
| Feedback1 (Dual) | FilmstripKnob | 10 | 00 | 3C | 0E | Yes | Dual mode |
| Time2 (Dual) | DataKnob | 10 | 00 | 3C | 11 | Yes | Dual mode |
| Feedback2 (Dual) | FilmstripKnob | 10 | 00 | 3C | 15 | Yes | Dual mode |
| Mod Rate/Depth/Level | FilmstripKnob | 10 | 00 | 3C | 18–1A | Yes | MODULATE mode |
| Twist Mode | ComboBox | 10 | 00 | 3C | 1B | Yes | TWIST mode |
| Rise/Fall | FilmstripKnob | 10 | 00 | 3C | 1C–1E | Yes | TWIST mode |
| Tap Time % | FilmstripKnob | 10 | 00 | 3C | 1F | Yes | PAN mode |
| CarryOver | FilmstripKnob | 10 | 00 | 3C | 20 | Yes | |

---

## DELAY 2 (stompbox_dd2.cpp → Dd2Panel.qml)

- **Power switch:** 10 00 3D 00
- **Tabs:** 1 (identical structure to DD1, hex2=3D)

Same control layout as DELAY 1 with hex2="3D".

---

## MASTER DELAY (stompbox_dd3.cpp → Dd3Panel.qml)

- **Power switch:** 10 00 3E 00
- **Tabs:** 1 (identical structure to DD1, hex2=3E)

Same control layout as DELAY 1 with hex2="3E".

---

## CHORUS (stompbox_ce.cpp → CePanel.qml)

- **Power switch:** 10 00 3F 00
- **Tabs:** 1 (mode-conditional dual controls)

| Control | Type | hex0 | hex1 | hex2 | hex3 | Conditional? | Notes |
|---------|------|------|------|------|------|-------------|-------|
| Mode | ComboBox | 10 | 00 | 3F | 01 | No | stackField selector |
| Rate | FilmstripKnob | 10 | 00 | 3F | 02 | No | |
| Depth | FilmstripKnob | 10 | 00 | 3F | 03 | No | |
| PreDelay | FilmstripKnob | 10 | 00 | 3F | 04 | No | |
| Effect Level | FilmstripKnob | 10 | 00 | 3F | 05 | No | |
| WaveForm | FilmstripKnob | 10 | 00 | 3F | 06 | No | |
| LowCut | FilmstripKnob | 10 | 00 | 3F | 07 | No | |
| HighCut | FilmstripKnob | 10 | 00 | 3F | 08 | No | |
| Direct Level | FilmstripKnob | 10 | 00 | 3F | 09 | No | |
| BPM | FilmstripKnob | 10 | 00 | 3F | 18 | No | |
| Rate1 | FilmstripKnob | 10 | 00 | 3F | 0A | Yes | Dual mode (mode ≥ 3) |
| Depth1 | FilmstripKnob | 10 | 00 | 3F | 0B | Yes | Dual mode |
| PreDelay1 | FilmstripKnob | 10 | 00 | 3F | 0C | Yes | Dual mode |
| EffectLevel1 | FilmstripKnob | 10 | 00 | 3F | 0D | Yes | Dual mode |
| WaveForm1 | FilmstripKnob | 10 | 00 | 3F | 0E | Yes | Dual mode |
| LowCut1 | FilmstripKnob | 10 | 00 | 3F | 0F | Yes | Dual mode |
| HighCut1 | FilmstripKnob | 10 | 00 | 3F | 10 | Yes | Dual mode |
| Rate2 | FilmstripKnob | 10 | 00 | 3F | 11 | Yes | Dual mode |
| Depth2 | FilmstripKnob | 10 | 00 | 3F | 12 | Yes | Dual mode |
| PreDelay2 | FilmstripKnob | 10 | 00 | 3F | 13 | Yes | Dual mode |
| EffectLevel2 | FilmstripKnob | 10 | 00 | 3F | 14 | Yes | Dual mode |
| WaveForm2 | FilmstripKnob | 10 | 00 | 3F | 15 | Yes | Dual mode |
| LowCut2 | FilmstripKnob | 10 | 00 | 3F | 16 | Yes | Dual mode |
| HighCut2 | FilmstripKnob | 10 | 00 | 3F | 17 | Yes | Dual mode |

---

## FX1 (stompbox_fx1.cpp → Fx1Panel.qml)

- **Power switch:** 10 00 40 00
- **Tabs:** 1 (dynamic content via Fx1TypeData.js)

| Control | Type | hex0 | hex1 | hex2 | hex3 | Conditional? | Notes |
|---------|------|------|------|------|------|-------------|-------|
| FX Type | ComboBox | 10 | 00 | 40 | 01 | No | Loads type-specific sub-view |
| (Type-specific) | Varies | 10 | 00 | 40 | 02+ | Yes | 38 FX types, each with unique controls |

FX type sub-views in `qml/fx1/`: ChorusView.qml, DelayView.qml, HarmonizerView.qml, PitchShifterView.qml, etc.

---

## FX2 (stompbox_fx2.cpp → Fx2Panel.qml)

- **Power switch:** 10 00 41 00
- **Tabs:** 1 (same dynamic structure as FX1, hex2=41)

---

## FX3 (stompbox_fx3.cpp → Fx3Panel.qml)

- **Power switch:** 10 00 42 00
- **Tabs:** 1 (same dynamic structure as FX1, hex2=42)

---

## REVERB (stompbox_rv.cpp → RvPanel.qml)

- **Power switch:** 10 01 32 00
- **Tabs:** 1

| Control | Type | hex0 | hex1 | hex2 | hex3 | Conditional? |
|---------|------|------|------|------|------|-------------|
| Type | ComboBox | 10 | 01 | 32 | 01 | No |
| Time | FilmstripKnob | 10 | 01 | 32 | 02 | No |
| CarryOver | ComboBox | 10 | 01 | 32 | 03 | No |
| PreDelay | FilmstripKnob | 10 | 01 | 32 | 04 | No |
| Effect Level | FilmstripKnob | 10 | 01 | 32 | 08 | No |
| LowCut | FilmstripKnob | 10 | 01 | 32 | 09 | No |
| HighCut | FilmstripKnob | 10 | 01 | 32 | 0A | No |
| Density | FilmstripKnob | 10 | 01 | 32 | 0B | No |
| Direct Level | FilmstripKnob | 10 | 01 | 32 | 0C | No |
| Spring Sense | FilmstripKnob | 10 | 01 | 32 | 0D | No |

---

## SEND/RETURN LOOP (stompbox_lp.cpp → LpPanel.qml)

- **Power switch:** 10 00 12 2E
- **Tabs:** 1

| Control | Type | hex0 | hex1 | hex2 | hex3 | Conditional? |
|---------|------|------|------|------|------|-------------|
| Mode | ComboBox | 10 | 00 | 12 | 2F | No |
| Send Level | FilmstripKnob | 10 | 00 | 12 | 30 | No |
| Return Level | FilmstripKnob | 10 | 00 | 12 | 32 | No |
| Level | FilmstripKnob | 10 | 00 | 12 | 34 | No |
| Output | ComboBox | 10 | 00 | 12 | 35 | No |

---

## FOOT VOLUME 1 (stompbox_fv1.cpp → Fv1Panel.qml)

- **Power switch:** None
- **Tabs:** 1

| Control | Type | hex0 | hex1 | hex2 | hex3 | Conditional? |
|---------|------|------|------|------|------|-------------|
| Level | FilmstripKnob | 10 | 00 | 12 | 28 | No |
| Curve | ComboBox | 10 | 00 | 12 | 29 | No |

---

## FOOT VOLUME 2 (stompbox_fv2.cpp → Fv2Panel.qml)

- **Power switch:** None
- **Tabs:** 1

| Control | Type | hex0 | hex1 | hex2 | hex3 | Conditional? |
|---------|------|------|------|------|------|-------------|
| Level | FilmstripKnob | 10 | 00 | 12 | 2A | No |
| Curve | ComboBox | 10 | 00 | 12 | 2B | No |

---

## MIXER (stompbox_mixer.cpp → MixerPanel.qml)

- **Power switch:** None
- **Tabs:** 1

| Control | Type | hex0 | hex1 | hex2 | hex3 | Conditional? |
|---------|------|------|------|------|------|-------------|
| Input 2 Level | FilmstripKnob | 10 | 00 | 12 | 22 | No |
| Input 1 Level | FilmstripKnob | 10 | 00 | 12 | 23 | No |
| Output Level | FilmstripKnob | 10 | 00 | 12 | 24 | No |

---

## DIVIDER (stompbox_divider.cpp → DividerPanel.qml)

- **Power switch:** None
- **Tabs:** 1

| Control | Type | hex0 | hex1 | hex2 | hex3 | Conditional? |
|---------|------|------|------|------|------|-------------|
| Divider Mode | ComboBox | 10 | 00 | 12 | 20 | No |
| Path B | ComboBox | 10 | 00 | 12 | 21 | No |

---

## BALANCER 1 (stompbox_balancer1.cpp → Balancer1Panel.qml)

- **Power switch:** None
- **Tabs:** 1

| Control | Type | hex0 | hex1 | hex2 | hex3 | Conditional? |
|---------|------|------|------|------|------|-------------|
| Level | FilmstripKnob | 10 | 00 | 12 | 25 | No |
| Balance A | FilmstripKnob | 10 | 00 | 12 | 26 | No |
| Balance B | FilmstripKnob | 10 | 00 | 12 | 27 | No |

---

## BALANCER 2 / BALANCER 3

Same structure as Balancer 1 (shared addresses — the balancers represent the
same logical mixer point at different positions in the signal chain).

---

## MAIN OUTPUT LEFT (stompbox_main_out_left.cpp → MainOutLeftPanel.qml)

- **Power switch:** None
- **Tabs:** 2 (Output Select / Global EQ)

### Tab 1: Output Select

| Control | Type | hex0 | hex1 | hex2 | hex3 | Conditional? |
|---------|------|------|------|------|------|-------------|
| Output Select | ComboBox | 00 | 04 | 00 | 00 | No |
| Level | FilmstripKnob | 00 | 04 | 00 | 2C | No |
| Phase | ComboBox | 00 | 04 | 00 | 34 | No |
| Output Amp | ComboBox | 00 | 04 | 00 | 36 | No |
| Extra Control | ComboBox | 10 | 00 | 12 | 37 | No |

### Tab 2: Global EQ

| Control | Type | hex0 | hex1 | hex2 | hex3 | Conditional? |
|---------|------|------|------|------|------|-------------|
| ParaEQ (4 bands + master) | ParaEqComponent | 00 | 04 | 00 | 02+ | No |

---

## MAIN OUTPUT RIGHT (stompbox_main_out_right.cpp → MainOutRightPanel.qml)

Same structure as Main Output Left.

---

## MAIN OUTPUT STEREO (stompbox_main_out_stereo.cpp → MainOutStereoPanel.qml)

- **Power switch:** None
- **Tabs:** 1 (Global EQ only)

| Control | Type | hex0 | hex1 | hex2 | hex3 | Conditional? | Notes |
|---------|------|------|------|------|------|-------------|-------|
| ParaEQ (4 bands + master) | ParaEqComponent | 00 | 04 | 3A | 02+ | No | hex2=3A guitar, 37 bass |

---

## SUB OUTPUT LEFT / RIGHT

Similar structure to Main Output Left/Right. LSB hardcoded to ("07", "5A").

---

## SUB OUTPUT STEREO (stompbox_sub_out_stereo.cpp)

Empty — `setEditPages()` has no controls.

---

# 2. Instrument/Source Pages

Instrument pages are **type-conditional**. The Instrument Type dropdown
(hex3="01" at the instrument's base address) selects which tabs appear.

| Type ID | Name | Base hex2 (Guitar) | Tabs |
|---------|------|---------------------|------|
| 0 | DYNAMIC SYNTH | 0x15 | Common, AltTune, OSC, Filter, Amp, LFO1, LFO2, Seq, Layer |
| 1 | OSC SYNTH | 0x18 | Common, AltTune, OSC, Filter/Amp, LFO |
| 2 | GR-300 | 0x19 | Common, AltTune, GR-300 |
| 3 | E.GUITAR | 0x1A | Common, AltTune, Guitar, Amp, NS, EQ |
| 4 | ACOUSTIC | 0x1B | Common, AltTune, Acoustic, Amp, EQ |
| 5 | E.BASS | 0x1C | Common, AltTune, E.Bass, Amp, EQ |
| 6 | VIO GUITAR | 0x1D | Common, AltTune, VioGuitar, Harmony, EQ |
| 7 | POLY FX | 0x1E (guitar) / 0x1D (bass) | Common, AltTune, PolyFX |
| — | NORMAL INPUT | 0x12 | Single panel (no tabs) |

**Instance offsets (INST 1/2/3):**
- INST 1: hex2 base as shown above
- INST 2: hex2 base + offset (guitar: 0x20 area, bass: 0x1F area)
- INST 3: hex2 base + offset (guitar: 0x2B area, bass: 0x29 area)

---

## NORMAL INPUT (soundSource_normal.cpp → NormalInputPanel.qml)

| Control | Type | hex0 | hex1 | hex2 | hex3 | Conditional? |
|---------|------|------|------|------|------|-------------|
| Power | SySwitch | 10 | 00 | 12 | 01 | No |
| Normal Level | FilmstripKnob | 10 | 00 | 12 | 02 | No |
| Cable Sim | SyComboBox | 10 | 00 | 12 | 04 | No |
| Phase | FilmstripKnob | 10 | 00 | 12 | 05 | No |

---

## COMMON TAB (CommonTab.qml)

Shared across all instrument types. hex2 = instrument base hex2.

| Control | Type | hex3 | Purpose |
|---------|------|------|---------|
| Inst Level | FilmstripKnob | 02 | Instrument output level |
| Nor Mix SW | SySwitch | 03 | Normal pickup mix enable |
| Nor Mix Level | FilmstripKnob | 04 | Normal pickup mix level |
| String Level 6–1 | FilmstripKnob | 06–0B | Per-string output level (6 knobs) |
| String Pan 6–1 | FilmstripKnob | 0C–11 | Per-string pan (6 knobs) |

---

## ALT TUNE TAB (AltTuneTab.qml)

Shared across Types 0–6. hex2 = hex2Base + 0x10 (guitar) or + 0x09 (bass).

| Control | Type | hex3 | Purpose |
|---------|------|------|---------|
| Tuning Type | SyComboBox | 01 | Preset selector |
| Pitch 6–1 | FilmstripKnob | 02–07 | Per-string pitch offset |
| Fine 6–1 | FilmstripKnob | 08–0D | Per-string fine tune |
| 12-String SW | SySwitch | 0E | 12-string enable |
| 12-String Type | SyComboBox | 0F | 12-string model |
| Str Bend SW | SySwitch | 28 | String bend enable |
| Bend Control | FilmstripKnob | 2F | Bend depth |
| Bend Depth 6–1 | FilmstripKnob | 29–2E | Per-string bend depth |

---

## TYPE 0: DYNAMIC SYNTH

### OSC Tab (OscTab.qml) — hex2 = base+1

| Control | Type | hex3 | Purpose |
|---------|------|------|---------|
| Wave Type | SyComboBox | 04 | Oscillator waveform |
| Pitch, Fine, Detune, Sharpness | FilmstripKnob | 05–08 | OSC parameters |
| Feedback, Harmonics, S-Saw, PWM | FilmstripKnob | 09–0E | OSC parameters |
| PWM Attack/Depth | FilmstripKnob | 08–09 | Envelope |
| P Env Attack/Depth | FilmstripKnob | 0A–0B | Pitch envelope |
| Bend Depth/Ctrl | FilmstripKnob | 0C–0D | |
| P Bend Ctl SW | SySwitch | 13 | |
| P Bend Ctl Source | SyComboBox | 17 | MIDI CC/LFO assignment |

### Filter Tab (FilterTab.qml) — hex2 = base+1

| Control | Type | hex3 | Purpose |
|---------|------|------|---------|
| Filter SW | SySwitch | 1C | Enable |
| Filter Type | SyComboBox | 1D–1E | Filter model |
| Cutoff | FilmstripKnob | 1F | |
| Resonance | FilmstripKnob | 20 | |
| Env Attack | FilmstripKnob | 21 | |
| Env Depth | FilmstripKnob | 22 | |

### Amp Tab (AmpTab.qml) — hex2 = base+1

| Control | Type | hex3 | Purpose |
|---------|------|------|---------|
| Amp Env Attack | FilmstripKnob | 23 | Attack time |
| Low Cut | FilmstripKnob | 25 | HPF |
| High Cut | FilmstripKnob | 26 | LPF |

### LFO1 Tab (Lfo1Tab.qml) — hex2 = base+1

| Control | Type | hex3 | Purpose |
|---------|------|------|---------|
| LFO SW | SySwitch | 27 | Enable |
| Shape | SyComboBox | 28 | Waveform |
| Rate | FilmstripKnob | 29 | |
| Depth (Dyn/Pitch/Flt/Amp/PWM) | FilmstripKnob | 2A–2E | 5 modulation depths |
| Fade Time | FilmstripKnob | 2F | |
| Sync | FilmstripKnob | 30 | |

### LFO2 Tab (Lfo2Tab.qml) — hex2 = base+1

| Control | Type | hex3 | Purpose |
|---------|------|------|---------|
| LFO SW | SySwitch | 31 | Enable |
| Shape | SyComboBox | 32 | Waveform |
| Rate | FilmstripKnob | 33 | |
| Depth (5 targets) | FilmstripKnob | 34–38 | |
| Fade Time | FilmstripKnob | 39 | |
| Sync | FilmstripKnob | 3A | |

### Seq Tab (SeqTab.qml) — hex2a = base+1, hex2b = base+2

Complex multi-section:
- Target selectors: hex2a hex3 3B–3D (3 combos)
- BPM: hex2 "12" hex3 3E
- SEQ1/SEQ2 controls: hex2b hex3 1E–23, 34–39
- Step grids (MAX/MIN values per step): hex2a 3E+ and hex2b 7E+
- Curve cells: hex2b hex3 24+step (SEQ1), 3A+step (SEQ2)

### Layer Tab (LayerTab.qml) — hex2 = base+2

| Control | Type | hex3 | Purpose |
|---------|------|------|---------|
| Lower Layer | SyComboBox | 4B | Instrument type selector |
| Upper Layer | SyComboBox | 4C | Instrument type selector |
| Lower Fade | FilmstripKnob | 4A | Crossfade |
| Upper Fade | FilmstripKnob | 4D | Crossfade |

---

## TYPE 1: OSC SYNTH

### OSC Tab (OscSynthOscTab.qml) — hex2 = 0x18

| Control | Type | hex3 | Purpose |
|---------|------|------|---------|
| Wave Category | SyComboBox | 00 | Wave family |
| Wave Type | SyComboBox | 01 | Specific wave |
| OSC1 (Pitch/Fine/PW/PWM/Env/Level) | FilmstripKnob | 02–09 | 8 parameters |
| OSC2 Type | SyComboBox | 0A | |
| OSC2 (Pitch/Fine/PW/PWM/Env/Level) | FilmstripKnob | 0B–12 | 8 parameters |
| Mono/Poly | SyComboBox | 13 | |
| Glide SW | SySwitch | 14 | |
| Legato SW | SySwitch | 15 | |
| Legato Time | FilmstripKnob | 16 | |
| Retrig | SyComboBox | 17 | |
| Hold Mode | SyComboBox | 18 | |
| Glide Time | FilmstripKnob | 19 | |

### Filter/Amp Tab (OscSynthFilterAmpTab.qml) — hex2 = 0x18

| Control | Type | hex3 | Purpose |
|---------|------|------|---------|
| Filter SW | SySwitch | 1A | Enable |
| Filter Type | SyComboBox | 1B | |
| Cutoff/Res/Env Attack/Depth | FilmstripKnob | 1C–22 | Filter controls |
| Filter Env ADSR + Depth | FilmstripKnob | 21–25 | |
| Amp Env ADSR + Vel | FilmstripKnob | 27–2C | 7 parameters |

### LFO Tab (OscSynthLfoTab.qml) — hex2 = 0x18

| Control | Type | hex3 | Purpose |
|---------|------|------|---------|
| LFO1 SW/Shape/Rate/Depths/Ctl | Various | 2D–36 | 10 parameters |
| LFO2 SW/Shape/Rate/Depths/Ctl | Various | 37–40 | 10 parameters |

---

## TYPE 2: GR-300

### GR-300 Tab (Gr300Tab.qml) — hex2 = 0x19

| Control | Type | hex3 | Purpose |
|---------|------|------|---------|
| Mode | SyComboBox | 00 | |
| Comp | SySwitch | 01 | Compressor enable |
| Mode-specific params | FilmstripKnob | 02–06, 13–14 | 7 parameters |
| Pitch Selector | SyComboBox | 07 | |
| Pitch controls | FilmstripKnob | 08–0F | 8 parameters |
| Vibrato SW/Depth/Speed | FilmstripKnob | 10–12 | 3 controls |

---

## TYPE 3: E.GUITAR

### Guitar Tab (EGuitarTab.qml) — hex2 = 0x1A

| Control | Type | hex3 | Purpose |
|---------|------|------|---------|
| Pickups (3 dropdowns) | SyComboBox | 00–02 | Pickup modeling |
| Sense/Depth/Attack/Resonance/Direct | FilmstripKnob | 03–07 | 5 parameters |
| Volume/Tone | FilmstripKnob | 08–09 | |

### Amp Tab (EGuitarAmpTab.qml) — hex2 = 0x1A

| Control | Type | hex3 | Purpose |
|---------|------|------|---------|
| Amp SW | SySwitch | 0A | Enable |
| Preamp Type 1 | SyComboBox | 0B | |
| Preamp params (7) | FilmstripKnob | 0C–0E, 10–13 | |
| Master | FilmstripKnob | 0F | |
| Mid SW | SySwitch | 14 | |
| Preamp Type 2 | SyComboBox | 15 | |
| Solo SW | SySwitch | 16 | |
| Solo Level | FilmstripKnob | 17 | |
| Cabinet Type 1/2 | SyComboBox | 18–19 | |
| Mic | SyComboBox | 1A | |
| Mic params (3) | FilmstripKnob | 1B–1D | |

### NS Tab (EGuitarNsTab.qml) — hex2 = 0x1A

| Control | Type | hex3 | Purpose |
|---------|------|------|---------|
| NS SW | SySwitch | 1E | Enable |
| Threshold | FilmstripKnob | 1F | |
| Release | FilmstripKnob | 20 | |

### EQ Tab (EGuitarEqTab.qml) — hex2 = 0x1A

| Control | Type | hex3 | Purpose |
|---------|------|------|---------|
| EQ SW | SySwitch | 21 | Enable |
| ParaEQ Bands (4) | FilmstripKnob | 22–25 | |

---

## TYPE 4: ACOUSTIC

### Acoustic Tab (AcousticTab.qml) — hex2 = 0x1B

| Control | Type | hex3 | Purpose |
|---------|------|------|---------|
| Type | SyComboBox | 00 | Acoustic model |
| Pickup params (8) | FilmstripKnob | 01–08 | |
| Cabinet Type | SyComboBox | 03 | |
| Cabinet params (2) | FilmstripKnob | 0A–0B | |
| NS SW | SySwitch | 20 | |
| Threshold/Release | FilmstripKnob | 21–22 | |

### Amp Tab (AcousticAmpTab.qml) — hex2 = 0x1B

| Control | Type | hex3 | Purpose |
|---------|------|------|---------|
| Amp SW | SySwitch | 0C | Enable |
| Preamp Type 1/2 | SyComboBox | 0D, 17 | |
| Preamp params (7) | FilmstripKnob | 0E–15 | |
| Mid SW | SySwitch | 16 | |
| Master | FilmstripKnob | 11 | |
| Solo SW/Level | SySwitch+Knob | 18–19 | |
| Cabinet Type 1/2 | SyComboBox | 1A–1B | |
| Mic + params (3) | SyComboBox+Knob | 1C–1F | |

### EQ Tab (AcousticEqTab.qml) — hex2 = 0x1B

| Control | Type | hex3 | Purpose |
|---------|------|------|---------|
| EQ SW | SySwitch | 23 | Enable |
| ParaEQ Bands (4) | FilmstripKnob | 24–27 | |

---

## TYPE 5: E.BASS

### E.Bass Tab (EBassTab.qml) — hex2 = 0x1C

| Control | Type | hex3 | Purpose |
|---------|------|------|---------|
| Bass Type | SyComboBox | 00 | Bass model |
| Bass params (7) | FilmstripKnob | 01–08 | |
| Body Type | SyComboBox | 03 | |
| EQ (2) | FilmstripKnob | 09–0A | |
| NS SW | SySwitch | 1F | |
| Threshold/Release | FilmstripKnob | 20–21 | |

### Amp Tab (EBassAmpTab.qml) — hex2 = 0x1C

Same structure as E.Guitar Amp, with hex3 offsets: SW=0B, Preamp1=0C, Preamp2=16, Master=10, Mid=15, Solo=17–18, Cabinet=19–1A, Mic=1B–1E.

### EQ Tab (EBassEqTab.qml) — hex2 = 0x1C

| Control | Type | hex3 | Purpose |
|---------|------|------|---------|
| EQ SW | SySwitch | 22 | Enable |
| ParaEQ Bands (4) | FilmstripKnob | 23–26 | |

---

## TYPE 6: VIO GUITAR (Guitar mode only)

### VioGuitar Tab (VioGuitarTab.qml) — hex2 = 0x1D

| Control | Type | hex3 | Purpose |
|---------|------|------|---------|
| Pickups (3 dropdowns) | SyComboBox | 00–02 | |
| Pickup params (7) | FilmstripKnob | 03–09 | |
| Filter Type | SyComboBox | 0E | |
| Filter params (4) | FilmstripKnob | 0F–12 | |
| Filter Cutoff Type | SyComboBox | 12 | |
| Filter detail (3) | FilmstripKnob | 13–15 | |
| NS SW | SySwitch | 16 | |
| Threshold/Release | FilmstripKnob | 17–18 | |

### Harmony Tab (VioGuitarHarmonyTab.qml) — hex2 = 0x1D

| Control | Type | hex3 | Purpose |
|---------|------|------|---------|
| Harmony params (4) | FilmstripKnob | 0A–0D | |

### EQ Tab (VioGuitarEqTab.qml) — hex2 = 0x1D

| Control | Type | hex3 | Purpose |
|---------|------|------|---------|
| EQ SW | SySwitch | 19 | Enable |
| ParaEQ Bands (4) | FilmstripKnob | 1A–1D | |

---

## TYPE 7: POLY FX

### PolyFX Tab (PolyFxTab.qml) — hex2 = 0x1E (guitar) / 0x1D (bass)

| Control | Type | hex3 | Purpose |
|---------|------|------|---------|
| Type | SyComboBox | 00 | Effect type |
| Effect params (5) | FilmstripKnob | 01–05 | |
| Filter Type 1/2 | SyComboBox | 06–07 | |
| Filter params (5) | FilmstripKnob | 08–0C | |
| Comp SW | SySwitch | 0D | |
| Ratio/Time | FilmstripKnob | 0E–0F | |
| NS SW | SySwitch | 10 | |
| Threshold/Release | FilmstripKnob | 11–12 | |

---

# 3. Left Navigation Pages

---

## MASTER (menuPage_master.cpp → MasterPanel.qml)

**Rendering:** QML
**MIDI Area:** hex0=10, hex1=00/02

### Section: PATCH (hex2=12)

| Control | Type | hex3 | Purpose |
|---------|------|------|---------|
| Tempo | FilmstripKnob | 3C | BPM |
| Swing | FilmstripKnob | 3E | |
| Chain Reverb | ComboBox | 43 | |
| Chain Mode | ComboBox | 42 | |
| Inside Mode | ComboBox | 00 | |

### Section: GK / NORMAL SET (hex2=02)

| Control | Type | hex3 | Purpose |
|---------|------|------|---------|
| GK Set | ComboBox | 00 | GK setting |
| Normal Set | ComboBox | 01 | Normal mode setting |

### Section: PATCH MIDI 1–4 (hex2=00)

32 ComboBox controls for MIDI channel assignments across 4 groups.
Addresses: hex3 = 39,3A,3C,3E,40,42,43,45 (group 1) through 60,61,63,65,67,69,6A,6C (group 4).

### Section: MIDI CC CONTROLLERS (hex2=00)

| Control | Type | hex3 | Purpose |
|---------|------|------|---------|
| CC 1–4 | ComboBox | 6D–70 | CC controller mappings |

### Section: GUITAR TO MIDI (hex2=14)

| Control | Type | hex3 | Purpose |
|---------|------|------|---------|
| Guitar-to-MIDI 1–7 | ComboBox | 00–06 | String-to-MIDI assignments |

### Section: POLY MIDI PATCH (hex2=00)

| Control | Type | hex3 | Purpose |
|---------|------|------|---------|
| Poly MIDI Patch 1–3 | ComboBox | 71,73,75 | |

### Section: STRING PATCHES (hex2=00–01)

18 ComboBox controls for string-specific patch assignments.

---

## SYSTEM (menuPage_system.cpp → SystemPanel.qml)

**Rendering:** QML
**Sidebar:** 14 sub-pages

| # | Page | QML File | MIDI hex1 |
|---|------|----------|-----------|
| 1 | Knob Setting | SysKnobSetting.qml | 01/08 |
| 2 | Input Setting | SysInputSetting.qml | 01/08 |
| 3 | Output Setting | SysOutputSetting.qml | 04/0B |
| 4 | Total Setting | SysTotalSetting.qml | 04/0B |
| 5 | Play Option | SysPlayOption.qml | 01/08 |
| 6 | Hardware Setting | SysHardwareSetting.qml | 01/08 |
| 7 | USB Setting | SysUsbSetting.qml | 04/0B |
| 8 | MIDI Setting | SysMidiSetting.qml | 04/0B |
| 9 | Tuner | TunerPanel.qml | 00/02 |
| 10 | GK Set Select | SysGkSetSelect.qml | 01/08 |
| 11 | Normal Set Select | SysNormalSetSelect.qml | 01/08 |
| 12 | Guitar To MIDI | SysGuitarToMidi.qml | 01/08 |
| 13 | Auto Power Off | SysAutoPowerOff.qml | 01/08 |
| 14 | Other | SysPlaceholder.qml | — |

---

## TUNER (menuPage_tuner.cpp → TunerPanel.qml)

**Rendering:** QML
Strobe tuner display (demo mode simulation when not connected to device).

---

## SETUP (menuPage_setup.cpp → SetupPanel.qml)

**Rendering:** QML
System configuration page.

---

## PEDAL / GK (menuPage_pdl.cpp → PedalPanel.qml)

**Rendering:** QML
**MIDI Area:** hex0=00/10, hex1=01/08 (system) or 00/02 (patch), hex2=10

**Sidebar:** 6 tabs

| # | Tab | QML File | Purpose |
|---|-----|----------|---------|
| 1 | Numbers 1–4 | pedal/PdlNumbers.qml | Pedal 1–4 memory/manual mode |
| 2 | Bank / Current | pedal/PdlBanks.qml | Bank Up/Down/Current |
| 3 | CTL 1–6 | pedal/PdlCtl.qml | Controller pedal assignments |
| 4 | EXP / Hold | pedal/PdlExp.qml | Expression pedal assignments |
| 5 | GK / Switch | pedal/PdlGk.qml | GK volume + switch assignments |
| 6 | LEDs | pedal/PdlLeds.qml | LED indicators |

---

## ASSIGN 1–16 (menuPage_assign.cpp + assign4–16.cpp)

**Rendering:** Legacy C++ widgets
**MIDI Area:** hex0=0B/0C, hex1=00

Per-assign page structure (repeated for each assign slot):

| Control | Type | hex2 (example Assign 1) | Purpose |
|---------|------|------------------------|---------|
| Enable SW | Switch | varies per slot | Assign enable |
| Target | Target widget | 21 | Parameter target selector |
| Source | ComboBox | 27 | Source selector |
| Source Mode | ComboBox | 28 | Source mode |
| Source Range | Range widget | 29 | Min/Max range |
| Int Pedal Trigger | ComboBox | 2B | Internal pedal trigger |
| Int Pedal Time | Knob | 2C | |
| Int Pedal Curve | ComboBox | 2D | |
| Wave Rate | Knob | 2E | |
| Wave Shape | ComboBox | 2F | |

---

## MIDI TABLES (menuPage_midi.cpp)

**Rendering:** Legacy C++ widgets
**MIDI Area:** hex0=00/10, hex1=01/08, hex2=10

5 pages within the Pedal menu for detailed MIDI control assignments.

---

# 4. Top Navigation & Status Bar

---

## Menu Bar (mainWindow.cpp)

| Menu | Key Items |
|------|-----------|
| **File** | Load Patch (Ctrl+O), Export SYX (Ctrl+Shift+S), Save TSL (Ctrl+Shift+G), Export TSL (Ctrl+Shift+E), Load System (Ctrl+L), Save System (Ctrl+D), Bulk Load (Ctrl+B), Bulk Save |
| **Tools** | Preferences, Upload to Device, Notes |
| **Settings** | Guitar Mode / Bass Mode toggle |
| **Help** | Help (F1), What's This (Shift+F1), Manual, Homepage, Donate, License, About, About Qt |

## Toolbar (consoletoolbar.cpp/ui)

Console toolbar for MIDI device communication.

## Status Bar (statusBarWidget.cpp)

| Component | Width | Purpose |
|-----------|-------|---------|
| Progress Bar | 80×13 px | Load/save progress |
| MIDI Activity | symbol | MIDI message indicator |
| Status Message | 150 px | User feedback messages |
| Debug Message | 950 px | Debug output (timeout: 10s debug / 3s normal) |

---

# 5. Signal Chain Floor Layout

## Floor Board (floorBoard.h/cpp)

Central canvas with:
- Background floor image
- Draggable stomp box tiles in signal chain order
- Drop indicators during drag operations
- Menu page button signals (System, Pedal, Assign, Tuner, Setup)

## Signal Chain Structure (customStructure.cpp/h)

Routing visualization showing:
- Path A / Path B parallel routing
- Divider → Effect chain → Mixer merge points
- Insert loop position
- Main/Sub output routing

## Stomp Box Tiles

Each tile shows:
- Effect name label
- On/off LED (power switch address)
- Click opens edit window with QML panel

**Key signals:**
- `system_buttonSignal(bool)` → opens System page
- `pedal_buttonSignal(bool)` → opens Pedal/GK page
- `assign1_buttonSignal(bool)` → opens Assign page
- `tuner_buttonSignal(bool)` → opens Tuner page
- `setup_buttonSignal(bool)` → opens Setup page

---

# Summary Statistics

| Category | Pages | Controls (approx) | Rendering |
|----------|-------|-------------------|-----------|
| Stomp/Effect blocks | 26 | ~250+ | All QML |
| Instrument types × 3 instances | 8 types × 3 | ~400+ | All QML |
| Left nav pages | 6 major | ~200+ | QML (Master/System/Tuner/Setup/Pedal), Legacy (Assign/MIDI) |
| Top nav | Menu bar + toolbar + status | N/A | C++ widgets |
| **Total** | **~40+ distinct page layouts** | **~850+ controls** | **~85% QML** |
