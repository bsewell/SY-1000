# SY-1000 UI Compare Matrix for Claude

Purpose: give Claude a deterministic, code-anchored checklist to compare BOSS SY-1000 screenshots against this FloorBoard implementation.

## Compare Method (strict)

1. Open one screenshot and identify the block name and control labels.
2. Open the mapped source file(s) below.
3. Verify each expected control exists as an `addComboBox`, `addKnob`, `addDataKnob`, `addSwitch`, or `addParaEQ`.
4. For any page with power state, verify the first row starts with a left-edge `addSwitch(...)` and that the same address is also used by the signal-chain tile via `setButton(...)`.
5. Mark status:
   - `Match`: all controls and page role exist.
   - `Partial`: controls exist but layout/header/state/selector behavior differs.
   - `Gap`: controls or behavior are missing.
6. Record exact file+line evidence for each mismatch.

## Block-to-Code Mapping

| Screenshot block | Expected controls from screenshot | Code anchors | Status |
|---|---|---|---|
| INST 1 (COMMON / ALT TUNE / GUITAR / AMP / NS / EQ) | INST TYPE, level/mix, per-string levels/pan, alt tune page set | `src/soundSource_inst1.cpp:95`, `src/soundSource_inst1.cpp:100`, `src/soundSource_inst1.cpp:102`, `src/soundSource_inst1.cpp:1527` | Partial |
| NORMAL INPUT | ON/OFF, CABLE SIM, NORMAL LEVEL, PHASE | `src/soundSource_normal.cpp:72`, `src/soundSource_normal.cpp:73`, `src/soundSource_normal.cpp:74`, `src/soundSource_normal.cpp:75` | Match |
| MASTER | GK SET, PATCH LEVEL, BPM, KEY, TEMPO HOLD | `src/stompbox_master.cpp:64`, `src/stompbox_master.cpp:65`, `src/stompbox_master.cpp:66`, `src/stompbox_master.cpp:68`, `src/stompbox_master.cpp:72` | Match |
| MAIN OUT | OUTPUT SELECT, GLOBAL EQ, PHASE | `src/stompbox_main_out_left.cpp:82`, `src/stompbox_main_out_left.cpp:84`, `src/stompbox_main_out_left.cpp:103` | Partial |
| SUB OUT | OUTPUT SELECT, GLOBAL EQ, PHASE | `src/stompbox_sub_out_left.cpp:81`, `src/stompbox_sub_out_left.cpp:83`, `src/stompbox_sub_out_left.cpp:102` | Partial |
| MIXER | A/B BALANCE, A:PAN, B:PAN | `src/stompbox_mixer.cpp:56`, `src/stompbox_mixer.cpp:57`, `src/stompbox_mixer.cpp:58` | Match |
| DIVIDER | MODE, CH SELECT | `src/stompbox_divider.cpp:57`, `src/stompbox_divider.cpp:58` | Match |
| BALANCER 1/2/3 | A/B BALANCE, A:PAN, B:PAN | `src/stompbox_balancer1.cpp:57`, `src/stompbox_balancer2.cpp:57`, `src/stompbox_balancer3.cpp:57` | Match |
| SEND RETURN | MODE, SEND LEVEL, RETURN LEVEL, ADJUST, PHASE | `src/stompbox_lp.cpp:69`, `src/stompbox_lp.cpp:74`, `src/stompbox_lp.cpp:80`, `src/stompbox_lp.cpp:81`, `src/stompbox_lp.cpp:76` | Match |
| FOOT VOLUME 1/2 | VOLUME MIN, VOLUME MAX, VOLUME CURVE, PEDAL POSITION | `src/stompbox_fv1.cpp:57`, `src/stompbox_fv1.cpp:58`, `src/stompbox_fv1.cpp:59`, `src/stompbox_fv1.cpp:60`, `src/stompbox_fv2.cpp:57` | Match |
| EQ 1 / EQ 2 | full para EQ strip (low cut, gains, mids, q, high cut, level) | `src/stompbox_eq1.cpp:69`, `src/stompbox_eq2.cpp:69`, `src/customControlParaEQ.cpp` | Match |
| FX 1 (Pitch Shifter view) | FX TYPE, VOICE selector, PS1/PS2 controls, DIRECT, BPM | `src/stompbox_fx1.cpp:84`, `src/stompbox_fx1.cpp:1000` | Partial |
| FX 2 (Flanger view) | FX TYPE, flanger params, DIRECT, BPM | `src/stompbox_fx2.cpp:84`, `src/stompbox_fx2.cpp:580` | Partial |
| FX 3 (Octave view) | FX TYPE, octave params, DIRECT | `src/stompbox_fx3.cpp:84`, `src/stompbox_fx3.cpp:874` | Partial |
| MASTER DELAY | TYPE, MODE, D1/D2 groups, trigger envelope, effect/direct, carry over, BPM | `src/stompbox_dd3.cpp:72`, `src/stompbox_dd3.cpp:136`, `src/stompbox_dd3.cpp:142`, `src/stompbox_dd3.cpp:248` | Partial |

## Known High-Value Gaps To Verify First

1. Variation selector parity:
   - BOSS screenshots show `VARIATION` on multiple blocks.
   - No `Variation` string appears in the main block files (`stompbox_fx*.cpp`, `stompbox_amp.cpp`, `stompbox_eq*.cpp`, `soundSource_inst*.cpp`).
2. Header/row layout parity:
   - Functionality exists, but spacing and header composition differ from BOSS panel screenshots.
   - Use `docs/ui/sy1000_boss_layout_notes.md` as the style baseline.
3. Main/Sub out parity:
   - Active implementation is split left/right editors.
   - Stereo classes are not the active path in current chain wiring (`floorBoard.cpp` creates left/right output blocks).
4. Delay page parity:
   - Core controls exist, but visible grouping and some per-mode presentation can differ from screenshot state.

## Related Docs (for cross-check)

- `docs/sy1000_reference_manual_ui_map.md`
- `docs/sy1000_owner_manual_ui_map.md`
- `docs/sy1000_midi_implementation_ui_map.md`
- `docs/ui/sy1000_boss_layout_notes.md`
