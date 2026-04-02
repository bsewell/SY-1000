# SY-1000 Complete Parameter Inventory
> Source: Boss Tone Studio JS (editor_inst_guitar.js, editor_inst_bass.js, editor_fx.js, editor_amp.js, editor_chorus.js, editor_rev.js, editor_mst_delay.js), resource.js, reference manual
> Generated: 2026-03-10 — re-verify against `midi.xml` and `src/MidiTable.cpp` if parameter ranges or types are in question
>
> **Purpose**: Human-readable reference for all instrument and effect parameter types/ranges. Use when:
> - Verifying a control's value range before editing `stompbox_*.cpp` or `soundSource_*.cpp`
> - Cross-checking parameter names against Boss reference manual terminology
> - Planning QML migration of a panel (what controls are needed)
>
> **Live code anchors**: `midi.xml` (SysEx address map), `src/MidiTable.cpp` (runtime address lookup)

---

## Instruments (INST 1, INST 2, INST 3)

### Guitar Mode — Instrument Types (8)

**0. DYNAMIC SYNTH**
- Pages: COMMON, ALT TUNE, OSC, FILTER, AMP, LFO1, LFO2, SEQ, LAYER
- OSC Waveform: SIN, SAW, TRI, SQR, PWM, DETUNE SAW, NOISE, FEEDBACK OSC, SUPER SAW, INPUT
- OSC Mode: SINGLE, DUAL, SYNC, RING
- OSC2 Waveform: SIN, SAW, TRI, SQR, PWM, NOISE
- Oscillator Pitch: A0–E6 (note range)
- Fine Tune: -12 to +12
- Pulse Width: -12 to +12
- PWM Env Attack / Depth: 0–89, 0–178
- Pitch Env Attack / Depth: 0–89, 0–178
- Pitch Bend Depth: 0–24 | Control: OFF, A, B
- Feedback: 0–63 | Harmonics: 0–63
- Supersaw: 0–63 | Detune: 0–63
- Sharpness: 0–63
- Sync Switch / Ring Mod Switch: OFF, ON, INV
- Portamento Mode: MONO, POLY

**1. OSC SYNTH**
- Pages: COMMON, ALT TUNE, OSC, FILTER, AMP, LFO1, LFO2
- Similar to DYNAMIC SYNTH, fewer pages (no SEQ, LAYER)

**2. GR-300**
- Pages: COMMON, ALT TUNE, GR-300
- Comp Switch: ON/OFF
- Pitch A / Pitch B: semitone + fine
- Duet Switch: ON/OFF
- Sweep Switch: ON/OFF | Rise / Fall Time
- Vibrato Switch: ON/OFF | Rate: 0–89 | Depth: 0–178
- Pitch Switch: ON/OFF

**3. E.GUITAR**
- Pages: COMMON, ALT TUNE, GUITAR, AMP, NS, EQ
- Guitar Type: CLA-ST, MOD-ST, TE, LP, P-90, 335, L4, RICK, LIPS, WIDE RANGE, BRIGHT HUM, FRETLESS
- Pickup Select (varies by type): REAR, R+C, CENTER, C+F, R+F, FRONT, ALL
- Tone Type: MILD, STANDARD, BRIGHT1, BRIGHT2
- Fretless-only: Depth, Sensitivity, Attack, Resonance, Direct Level (0–63 each)

**4. ACOUSTIC**
- Pages: COMMON, ALT TUNE, ACOUSTIC, AMP, NS, EQ
- Acoustic Type: MA28, TRP-0, GB45, GB SML, GLD 40, NYLON, RESO, BANJO, SITAR
- Pickup Select: FRONT, R+F, REAR, PIEZO
- Tone Type: MILD, STANDARD, BRIGHT
- Type-conditional: Body, Resonance, Attack, Sustain, Buzz, Decay, Color, Sensitivity

**5. E.BASS**
- Pages: COMMON, ALT TUNE, BASS, AMP, NS, EQ
- Bass Type: VINT JB, JB, VINT PB, PB, M-MAN, RICK, T-BIRD, ACTIVE, VIOLIN, FRETLESS
- Pickup Select (varies by type)
- Tone/Treble/Bass/Volume (type-dependent)
- Rhythm/Solo Select (some types)
- Fretless parameters when applicable

**6. VIO GUITAR**
- Pages: COMMON, ALT TUNE, GUITAR, HARMO, FILTER, NS, EQ
- Same Guitar Type list as E.GUITAR
- Pickup Select: REAR, R+C, CENTER, C+F, R+F, FRONT (+ALL for LIPS type)

**7. POLY FX**
- Pages: COMMON, ALT TUNE, POLY FX, NS
- Poly Type: DISTORTION, CRYSTAL, RICH MOD, SLOW PAD, TOUCH WAH
- TOUCH WAH only: Sensitivity, Frequency, Comp Attack, Comp Sustain, Comp Switch, Tone Type (LPF/BPF), Peak, Decay, Filter Polarity (DOWN/UP)
- Others: Guitar Volume, Color, Tone
- DISTORTION only: Gain, Gain Balance

---

### Bass Mode — Instrument Types (7)

**0. DYNAMIC SYNTH** — same as Guitar mode
**1. OSC SYNTH** — same as Guitar mode

**2. ANALOG GR**
- Pages: COMMON, ALT TUNE, ANALOG GR
- Similar to GR-300 with ANALOG GR-specific parameters

**3. E.BASS** — same as Guitar mode E.BASS

**4. AC BASS**
- Pages: COMMON, ALT TUNE, ACBASS, AMP, NS, EQ

**5. E.GTR** (Bass mode only)
- Pages: COMMON, ALT TUNE, GUITAR, AMP, NS, EQ
- Guitar Types: ST, LP, FRETLESS (3 types only in Bass mode)
- Pickup Select: REAR, R+C, CENTER, C+F, R+F, FRONT

**6. POLY FX** — same as Guitar mode

---

## FX Blocks — FX1, FX2, FX3 (Identical Structure)
### 38 FX Types

1. **AC RESONANCE** — Depth, Resonance Frequency, Level
2. **AUTO WAH** — Sensitivity, Frequency, Peak Level, Decay, Filter Type, Effect Level, Direct Level
3. **CHORUS** — Mode (MONO/STEREO1/STEREO2/DUAL), Rate, Depth, Pre-delay (0–40ms), Waveform (TRI/SINE), Low Cut, High Cut, Effect Level, Direct Level
4. **CLASSIC-VIBE** — Depth, Rate, Effect Level
5. **COMPRESSOR** — Type (BOSS COMP, Hi-BAND, LIGHT, D-COMP, ORANGE, FAT, MILD), Sustain, Attack, Tone, Level, Direct Level
6. **DEFRETTER** — Sensitivity, Attack, Decay, Tone controls
7. **DEFRETTER BASS** — same as DEFRETTER
8. **DELAY** — Type (STEREO1, STEREO2, PAN, DUAL-S, DUAL-P, DUAL L/R, REVERSE, ANALOG, TAPE, MOD, WARP, TWIST), Time (0ms–2000ms or BPM-synced), Feedback, High Cut, Effect Level, Direct Level; MOD/WARP: Mode Depth + Rate; TWIST: Rise Mode (RISE->FALL / RISE->FADE), Rise/Fall/Fade Times
9. **FLANGER** — Rate, Depth, Pre-delay, Waveform, Low Cut, High Cut, Effect Level, Direct Level
10. **FLANGER BASS** — same as FLANGER
11. **FOOT VOLUME** — Min Level, Max Level
12. **GRAPHIC EQ** — 30 bands (20Hz–16kHz), each ±12dB
13. **HARMONIST** — Voice (1VOICE/2MONO/2STEREO), Master Key (C–B), HR1 Pitch (-2oct to +2oct + USER), HR1 Level, HR2 Pitch + Level + Pre-delay (multi-voice only), Effect Level
14. **HUMANIZER** — Mode (AUTO/MANUAL), Sensitivity (AUTO), vowel/manual controls
15. **ISOLATOR** — Filter Type (LPF/BPF/HPF), Frequency, Resonance
16. **LIMITER** — Type (BOSS LIMITER/RACK160D/VTG RACK U), Ratio (1:1 to INF:1), Release, Makeup Gain
17. **LO-FI** — Bit reduction, Sample rate reduction, Mix
18. **OCTAVE** — Voice levels, Direct Level, Effect Level
19. **OCTAVE BASS** — same as OCTAVE
20. **PAN** — Mode (AUTO/MANUAL); AUTO: Wave Shape, Rate, Depth; MANUAL: Position (L50–R50)
21. **PARAMETRIC EQ** — Low/Mid/High Frequency + Gain + Q, Level, Direct Level
22. **PEDAL BEND** — Bend Range (semitones), Bend Mode
23. **PHASER** — Stage (4STAGE/8STAGE/12STAGE/BiPHASE), Rate, Depth, Feedback, Manual (D:E ratio)
24. **PITCH SHIFTER** — Voice (1VOICE/2MONO/2STEREO), V1 Pitch (-2oct to +2oct + USER), V1 Level, V2 Pitch + Level + Pre-delay (multi-voice), Effect Level
25. **REVERB** — Type (AMBIENCE/ROOM/HALL1/HALL2/PLATE/SPRING/MOD), Time (0.1s–10.0s), Pre-delay, Spring Sensitivity (SPRING only), Effect Level, Direct Level
26. **RING MOD** — Frequency (80Hz–10kHz), Depth, Mix
27. **ROTARY** — Speed Select (FAST/MEDIUM/SLOW/MONO), Depth, Effect Level
28. **SITAR SIM** — Pitch, Decay, Sustain
29. **SLICER** — Pattern (P1–P20), Rate (BPM-synced or manual), Depth, Trigger (PICKING/AUTO)
30. **SLOW GEAR** — Rise Time, Sensitivity
31. **SLOW GEAR BASS** — same as SLOW GEAR
32. **SOUND HOLD** — Hold Time, Mix
33. **TOUCH WAH** — Sensitivity, Frequency, Comp Attack, Comp Sustain, Comp Switch, Tone Type (LPF/BPF), Peak, Decay, Filter Polarity (DOWN/UP), Direct Level, Effect Level
34. **TOUCH WAH BASS** — same as TOUCH WAH
35. **TREMOLO** — Rate (BPM or manual), Depth, Wave Shape, Effect Level
36. **VIBRATO** — Rate, Depth, Pre-delay, Waveform (TRI/SINE), Effect Level
37. **WAH** — Type (CRY WAH, VO WAH, FAT WAH, LIGHT WAH, 7STRING WAH, RESO WAH, BASS WAH), Frequency, Response (SHARP/SLOW/SHORT-SHARP/SHORT-SLOW/SUPER-SLOW), Direct Level, Effect Level
38. **[AC RESONANCE through WAH = 38 types confirmed]**

---

## Compressor Block

- Type: BOSS COMP, Hi-BAND, LIGHT, D-COMP, ORANGE, FAT, MILD (7)
- Sustain: 0–63 | Attack: 0–63 | Tone | Level | Direct Level

---

## Distortion Block (DS)

35 types: MID BOOST, CLEAN BOOST, TREBLE BOOST, CRUNCH, NATURAL OD, WARM OD, FAT DS, LEAD DS, METAL DS, OCT FUZZ, A-DIST, X-OD, X-DIST, BLUES OD, OD-1, T-SCREAM, TURBO OD, DIST, CENTA OD, RAT, GUV DS, DIST+, METAL ZONE, HM-2, METAL CORE, '60S FUZZ, MUFF FUZZ, BASS OD, BASS DS, BASS MT, BASS FUZZ, HI BAND DRIVE, X-BASS OD, BASS DRV, BASS DI

- Gain: 0–120 | Tone: -10 to +10 | Bottom: LOW/MIDDLE/HIGH | Level | Direct Level

---

## AMP Block

31 types: TRANSPARENT, NATURAL, BOUTIQUE, SUPREME, MAXIMUM, JUGGERNAUT, X-CRUNCH, X-HI GAIN, X-MODDED, JC-120, TWIN COMBO, DELUXE COMBO, TWEED COMBO, DIAMOND AMP, BRIT STACK, RECTI STACK, MATCH COMBO, BG COMBO, ORNG STACK, BGNR UB METAL, NATURAL BASS, X-DRIVE BASS, CONCERT, SUPER FLAT, FLIP TOP, B MAN, BASS 360, SW-TOUR, AC BASS, GK BASS, MARK

- Gain: 0–120
- Bass / Middle / Treble / Presence: -10 to +10
- Bright Switch: ON/OFF (types 1,2,6,9,10,12,20,22,24,26)
- Master: 0–63 | Level | Direct Level
- Speaker Type: OFF, ORIGINAL, 1x8", 1x10", 1x12", 2x12", 4x10", 4x12", 8x12", B1x15", B1x18", B2x15", B4x10", B8x10"
- Mic Type: DYN57, DYN421, CND451, CND87, FLAT
- Mic Position: SHORT, MEDIUM, LONG
- Mic Distance: CENTER, 1cm–10cm

---

## EQ1 / EQ2 Blocks

- Low Freq: 20Hz–1kHz | Low Gain: -12 to +12dB
- Mid Freq: 200Hz–8kHz | Mid Gain: -12 to +12dB
- High Freq: 1kHz–16kHz | High Gain: -12 to +12dB
- Level | Direct Level

---

## Noise Suppressor (NS)

- Threshold: 0–63 | Release: 0–63 | Sense: 0–63

---

## Delay 1 / Delay 2 Blocks

- Type: STEREO1, STEREO2, PAN, DUAL-S, DUAL-P, DUAL L/R, REVERSE, ANALOG, TAPE, MOD, WARP, TWIST (12)
- Time: 0ms–2000ms or BPM-synced (1/32–2/1 including dotted + triplet)
- Feedback: 0–63 | High Cut | Effect Level | Direct Level
- Dual types: D1 + D2 independent Time/Feedback/High Cut/Level
- MOD/WARP: Mode Depth 0–63, Mode Rate (BPM)
- TWIST: Rise Mode (RISE->FALL / RISE->FADE), Rise/Fall/Fade Times (BPM)

---

## Master Delay Block

- Same type list and parameters as Delay 1/2
- Independent from Delay 1/2 (post-signal-chain position)

---

## Chorus Block

- Mode: MONO, STEREO1, STEREO2, DUAL
- Rate: 0–100 or BPM-synced | Depth: 0–63
- Pre-delay: 0.0ms–40.0ms or BPM-synced
- Waveform: TRI, SINE
- Low Cut | High Cut | Effect Level | Direct Level
- DUAL mode: independent Rate1/2, Depth1/2, Pre-delay1/2, Effect Level1/2, Low Cut1/2, High Cut1/2, Waveform1/2

---

## Reverb Block

- Type: AMBIENCE, ROOM, HALL1, HALL2, PLATE, SPRING, MOD (7)
- Time: 0.1s–10.0s | Pre-delay: 0.0ms–40.0ms
- Spring Sensitivity (SPRING only): 0–63
- Effect Level | Direct Level | Low Cut | High Cut

---

## Foot Volume (FV1 / FV2)

- Min Level: 0–127 | Max Level: 0–127
- Direct Level | Effect Level

---

## Send / Return

- Send Level: 0–63 | Dry Level: 0–63
- Return Level: 0–63 | Effect Level: 0–63

---

## BPM / Tempo-Sync Note Values

Used across all delay, chorus, LFO, slicer rate parameters:
`2/1, 1/1D, 1/1, 1/2D, 1/1T, 1/2, 1/4D, 1/2T, 1/4, 1/8D, 1/4T, 1/8, 1/16D, 1/8T, 1/16, 1/32D, 1/16T, 1/32`

---

## Harmonist Key Values

`C(Am), Db(Bbm), D(Bm), Eb(Cm), E(C#m), F(Dm), F#(D#m), G(Em), Ab(Fm), A(F#m), Bb(Gm), B(G#m)`

## Harmonist/Pitch Shifter Pitch Values

`-2oct, -14th, -13th, -12th, -11th, -10th, -9th, -1oct, -7th, -6th, -5th, -4th, -3rd, -2nd, UNISON, +2nd, +3rd, +4th, +5th, +6th, +7th, +1oct, +9th, +10th, +11th, +12th, +13th, +14th, +2oct, USER`

## Alt Tune Presets

`OPEN D, OPEN E, OPEN G, OPEN A, DROP D, DROP Db, DROP C, DROP B, DROP Bb, DROP A, D-MODAL, NASHVL, -12STEP to +12STEP, USER`

---

## Conditional Parameter Notes

- **E.Guitar FRETLESS** type disables Pickup Select; enables Depth/Sensitivity/Attack/Resonance/Direct Level
- **Acoustic type** changes show/hide Body, Resonance, Attack, Sustain, Buzz, Decay, Color, Sensitivity
- **E.Bass** has 10 different parameter configurations per type
- **Delay TWIST** mode controls change based on RISE->FALL vs RISE->FADE selection
- **Chorus DUAL** mode adds second independent parameter set
- **Harmonist** 1VOICE disables HR2 controls; 2MONO/2STEREO enables them
- **PAN AUTO** enables Rate/Depth/Wave; MANUAL enables Position slider
- **AMP Bright** switch only present on specific types (1,2,6,9,10,12,20,22,24,26)
- **POLY FX TOUCH WAH** (type 4) has entirely different parameter set from other Poly FX types
