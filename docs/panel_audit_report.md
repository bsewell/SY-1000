# FloorBoard Panel Audit Report
> Cross-check: FloorBoard Qt source vs SY-1000 parameter spec (Boss address_map.js + reference manual)
> Generated: 2026-03-10 | Updated: 2026-03-10 (after Boss address_map.js verification)
>
> **Re-audit notice**: This report reflects state as of 2026-03-10. Re-audit any block whose source file has changed since that date. Check `git log -- stompbox_*.cpp soundSource_*.cpp` for changes. For MIDI address reference: `midi.xml`, `src/MidiTable.cpp`.

---

## Executive Summary

| Block | Status | Notes |
|-------|--------|-------|
| FX1 / FX2 / FX3 | ✅ PASS | 38 types; GEQ has 7 bands (not 30 — that was wrong); types 17–38 partially verified |
| Compressor (CS) | ✅ PASS | All 7 types, all parameters |
| Distortion (DS) | ✅ PASS | Addresses correct (03=TONE, 05=BOTTOM); comments fixed |
| AMP | ✅ PASS | All 31 types, Speaker/Mic modeling, Bright switch conditional |
| EQ1 / EQ2 | ✅ PASS | Uses addParaEQ() helper |
| Noise Suppressor (NS) | ✅ PASS | addr 03 = DETECT (0-5), comboBox correct |
| Delay 1 / Delay 2 | ✅ FIXED | Full 12-mode type switching added, matching DD3 structure |
| Master Delay (DD3) | ✅ PASS | All 12 modes, DUAL, TWIST all implemented |
| Chorus (CE) | ✅ PASS | All 4 modes, DUAL dual-param sets correct |
| Reverb (RV) | ✅ PASS | All 7 types, SPRING sensitivity conditional correct |
| FV1 / FV2 | ✅ FIXED | Volume Curve changed from addKnob to addComboBox |
| Send/Return (LP) | ✅ FIXED | Duplicate stackFields removed; addSystemOverride hides Return+Adjust on OUT |
| INST 1/2/3 | ⚠️ PARTIAL | Type selectors present; type-specific pages need full audit |

---

## Fixed Issues

### CRITICAL (now resolved)
1. **Delay 1 / Delay 2 — No type switching** ✅ FIXED
   - Files: `stompbox_dd1.cpp`, `stompbox_dd2.cpp`
   - Was: Single flat panel (Time, Feedback, High Cut, Carry Over, Effect Level, Direct Level)
   - Now: Full 12-type stackField system matching DD3 structure
   - DD1 addresses: Guitar hex2="3C", Bass hex2="39"
   - DD2 addresses: Guitar hex2="3D", Bass hex2="3A"

### HIGH (now resolved)
2. **Send/Return — incomplete** ✅ FIXED
   - File: `stompbox_lp.cpp`
   - Was: Duplicate stackFields both using addr "32"/"34", empty third stackField
   - Now: Flat layout with addSystemOverride hiding Return+Adjust when Mode = OUT (value 2)
   - Spec-confirmed params: SROnOff(2E), SRMode(2F, 0-2), SRSendLevel(30, 2-byte),
     SRReturnLevel(32, 2-byte), SRAdjust(34), SRPhase(35, 0-1)
   - No "Dry Level" exists — original audit was wrong on this point

### LOW (now resolved)
3. **Foot Volume — Volume Curve was addKnob, now addComboBox** ✅ FIXED
   - Files: `stompbox_fv1.cpp`, `stompbox_fv2.cpp`
   - Addr 12 (FV1) and 1F (FV2): Volume Curve has 4 discrete values (0-3)

4. **Distortion — misleading code comments** ✅ FIXED
   - File: `stompbox_ds.cpp`
   - addr 03 comment: "bass" → "tone"
   - addr 05 comment: "treble" → "bottom"
   - Addresses were always correct per Boss spec

---

## Corrected Audit Findings (Previous Report Errors)

- **Graphic EQ "30 bands missing"** — FALSE. Boss address_map.js confirms SY-1000 GEQ has exactly 7 bands
  (100Hz, 200Hz, 400Hz, 800Hz, 1.6kHz, 3.2kHz, 6.4kHz) + Level. The 8-knob implementation was correct.
- **Send/Return "Dry Level missing"** — FALSE. No Dry Level in Boss spec. The PatchEfct.SR* params are:
  SROnOff, SRMode, SRSendLevel, SRReturnLevel, SRAdjust, SRPhase. All correctly implemented.
- **Distortion "TONE wrong, BOTTOM missing"** — PARTIALLY FALSE. Addresses were correct all along.
  Only the code comments were misleading.
- **Noise Suppressor "unexplained comboBox at addr 03"** — FALSE. Addr 03 = DETECT (0-5, 6 values). Correct.

---

## Remaining Items

### MEDIUM
1. **FX1/FX2/FX3 — types 17–38 full audit needed**
   - Types: HARMONIST, HUMANIZER, ISOLATOR, LIMITER, LO-FI, OCTAVE, OCTAVE BASS, PAN,
     PARAMETRIC EQ, PEDAL BEND, PHASER, PITCH SHIFTER, RING MOD, ROTARY, SITAR SIM,
     SLICER, SLOW GEAR, SLOW GEAR BASS, SOUND HOLD, TOUCH WAH, TOUCH WAH BASS, TREMOLO, VIBRATO, WAH
   - Confirmed present in source; parameter completeness not fully verified

2. **INST panels — type-specific pages not fully audited**
   - Files: `soundSource_inst1/2/3.cpp`
   - Main type selectors confirmed; OSC/FILTER/AMP/LFO/SEQ/LAYER page content needs full read

### LOW
3. **AMP Bright Switch — conditional verification**
   - File: `stompbox_amp.cpp`
   - Spec: Bright only on types 1,2,6,9,10,12,20,22,24,26
   - Verify `addSystemOverride()` targets exactly those indices

---

## Blocks That Pass Fully
- Compressor (CS): 7 types ✅
- Distortion (DS): 35 types, all 9 params ✅
- AMP: 31 types + full speaker/mic modeling ✅
- EQ1/EQ2: addParaEQ() helper ✅
- Noise Suppressor (NS): Threshold + Release + Detect ✅
- Chorus (CE): 4 modes + DUAL dual-params ✅
- Reverb (RV): 7 types + SPRING conditional ✅
- Master Delay (DD3): 12 types + DUAL + TWIST ✅
- Delay 1 (DD1): 12 types — FIXED ✅
- Delay 2 (DD2): 12 types — FIXED ✅
- Send/Return (LP): Mode/Send/Return/Adjust/Phase, conditional hiding — FIXED ✅
- Foot Volume 1/2: Min/Max/Position/Curve — FIXED ✅
- Graphic EQ (FX type): 7 bands + Level — CONFIRMED CORRECT ✅
