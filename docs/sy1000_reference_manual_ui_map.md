# SY-1000 Reference Manual to UI Mapping (All Pages)

This maps every page of the official **SY-1000 Reference Manual** (`SY-1000_reference_eng02_W.pdf`) to the current FloorBoard UI and code anchors.

## Scope

- Source PDF: `docs/manuals/SY-1000_reference_eng02_W.pdf` (99 pages)
- Per-page text: `docs/manuals/text/reference_pages/page_001.txt` ... `page_099.txt`
- Date mapped: 2026-03-07 (local)

## Status Key

- `Implemented`: UI + data flow exist in this app.
- `Partial`: only part of the manual behavior is app-level, or relies on hardware procedure.
- `Implemented/Partial`: mostly implemented with some firmware-side or UX-gap caveats.

## Page-by-Page Crosswalk

| Page | Manual Topic (from page text) | FloorBoard Mapping | Primary Code Anchors | Status |
|---|---|---|---|---|
| 1 | Reference Manual cover | Documentation index; no runtime UI | `docs/sy1000_manuals.md` | Implemented (docs) |
| 2 | Getting Ready. . . . . . . . . . . . . . . . . . . . . . . . . 4 | Documentation index; no runtime UI | `docs/sy1000_manuals.md` | Implemented (docs) |
| 3 | PREFERENCE. . . . . . . . . . . . . . . . . . . . . . . .. . 69 | Documentation index; no runtime UI | `docs/sy1000_manuals.md` | Implemented (docs) |
| 4 | Getting Ready | System/Setup controls (instrument mode, output select, GK/normal input settings) | `menuPage_setup.cpp, menuPage_system.cpp, Preferences.cpp` | Implemented |
| 5 | Getting Ready | System/Setup controls (instrument mode, output select, GK/normal input settings) | `menuPage_setup.cpp, menuPage_system.cpp, Preferences.cpp` | Implemented |
| 6 | Getting Ready | System/Setup controls (instrument mode, output select, GK/normal input settings) | `menuPage_setup.cpp, menuPage_system.cpp, Preferences.cpp` | Implemented |
| 7 | Getting Ready | System/Setup controls (instrument mode, output select, GK/normal input settings) | `menuPage_setup.cpp, menuPage_system.cpp, Preferences.cpp` | Implemented |
| 8 | Getting Ready | System/Setup controls (instrument mode, output select, GK/normal input settings) | `menuPage_setup.cpp, menuPage_system.cpp, Preferences.cpp` | Implemented |
| 9 | Getting Ready | System/Setup controls (instrument mode, output select, GK/normal input settings) | `menuPage_setup.cpp, menuPage_system.cpp, Preferences.cpp` | Implemented |
| 10 | Getting Ready | System/Setup controls (instrument mode, output select, GK/normal input settings) | `menuPage_setup.cpp, menuPage_system.cpp, Preferences.cpp` | Implemented |
| 11 | Basic Operation | Core edit/write workflows (edit screens, assign, patch save, tuner/system access) | `editPage.cpp, menuPage_tuner.cpp, menuPage_assign*.cpp, patchWriteDialog.cpp` | Implemented/Partial |
| 12 | Basic Operation | Core edit/write workflows (edit screens, assign, patch save, tuner/system access) | `editPage.cpp, menuPage_tuner.cpp, menuPage_assign*.cpp, patchWriteDialog.cpp` | Implemented/Partial |
| 13 | Switching Patches (Tone) | Core edit/write workflows (edit screens, assign, patch save, tuner/system access) | `editPage.cpp, menuPage_tuner.cpp, menuPage_assign*.cpp, patchWriteDialog.cpp` | Implemented/Partial |
| 14 | Editing a Patch | Core edit/write workflows (edit screens, assign, patch save, tuner/system access) | `editPage.cpp, menuPage_tuner.cpp, menuPage_assign*.cpp, patchWriteDialog.cpp` | Implemented/Partial |
| 15 | Editing a Patch | Core edit/write workflows (edit screens, assign, patch save, tuner/system access) | `editPage.cpp, menuPage_tuner.cpp, menuPage_assign*.cpp, patchWriteDialog.cpp` | Implemented/Partial |
| 16 | Editing a Patch | Core edit/write workflows (edit screens, assign, patch save, tuner/system access) | `editPage.cpp, menuPage_tuner.cpp, menuPage_assign*.cpp, patchWriteDialog.cpp` | Implemented/Partial |
| 17 | Editing a Patch | Core edit/write workflows (edit screens, assign, patch save, tuner/system access) | `editPage.cpp, menuPage_tuner.cpp, menuPage_assign*.cpp, patchWriteDialog.cpp` | Implemented/Partial |
| 18 | Editing a Patch | Core edit/write workflows (edit screens, assign, patch save, tuner/system access) | `editPage.cpp, menuPage_tuner.cpp, menuPage_assign*.cpp, patchWriteDialog.cpp` | Implemented/Partial |
| 19 | Saving a Patch | Core edit/write workflows (edit screens, assign, patch save, tuner/system access) | `editPage.cpp, menuPage_tuner.cpp, menuPage_assign*.cpp, patchWriteDialog.cpp` | Implemented/Partial |
| 20 | Settings for the Entire SY-1000 (System Parameters) | Core edit/write workflows (edit screens, assign, patch save, tuner/system access) | `editPage.cpp, menuPage_tuner.cpp, menuPage_assign*.cpp, patchWriteDialog.cpp` | Implemented/Partial |
| 21 | The Internal Structure of the SY-1000 | Signal-chain representation and block-level architecture | `floorBoard.cpp, floorBoardDisplay.cpp, stompBox.cpp` | Implemented |
| 22 | INST Parameters | INST parameter editing (type-specific controls and ranges) | `soundSource_inst1.cpp, soundSource_inst2.cpp, soundSource_inst3.cpp, customControl*.cpp` | Implemented |
| 23 | INST Parameters > INST TYPE | INST parameter editing (type-specific controls and ranges) | `soundSource_inst1.cpp, soundSource_inst2.cpp, soundSource_inst3.cpp, customControl*.cpp` | Implemented |
| 24 | INST Parameters > INST TYPE | INST parameter editing (type-specific controls and ranges) | `soundSource_inst1.cpp, soundSource_inst2.cpp, soundSource_inst3.cpp, customControl*.cpp` | Implemented |
| 25 | INST Parameters > DYNAMIC SYNTH Parameters | INST parameter editing (type-specific controls and ranges) | `soundSource_inst1.cpp, soundSource_inst2.cpp, soundSource_inst3.cpp, customControl*.cpp` | Implemented |
| 26 | INST Parameters > DYNAMIC SYNTH Parameters | INST parameter editing (type-specific controls and ranges) | `soundSource_inst1.cpp, soundSource_inst2.cpp, soundSource_inst3.cpp, customControl*.cpp` | Implemented |
| 27 | INST Parameters > DYNAMIC SYNTH Parameters | INST parameter editing (type-specific controls and ranges) | `soundSource_inst1.cpp, soundSource_inst2.cpp, soundSource_inst3.cpp, customControl*.cpp` | Implemented |
| 28 | INST Parameters > DYNAMIC SYNTH Parameters | INST parameter editing (type-specific controls and ranges) | `soundSource_inst1.cpp, soundSource_inst2.cpp, soundSource_inst3.cpp, customControl*.cpp` | Implemented |
| 29 | INST Parameters > OSC SYNTH Parameters | INST parameter editing (type-specific controls and ranges) | `soundSource_inst1.cpp, soundSource_inst2.cpp, soundSource_inst3.cpp, customControl*.cpp` | Implemented |
| 30 | INST Parameters > OSC SYNTH Parameters | INST parameter editing (type-specific controls and ranges) | `soundSource_inst1.cpp, soundSource_inst2.cpp, soundSource_inst3.cpp, customControl*.cpp` | Implemented |
| 31 | INST Parameters > OSC SYNTH Parameters | INST parameter editing (type-specific controls and ranges) | `soundSource_inst1.cpp, soundSource_inst2.cpp, soundSource_inst3.cpp, customControl*.cpp` | Implemented |
| 32 | INST Parameters > GR-300 (In GUITAR MODE) / ANALOG GR (In BASS MODE) Parameters | INST parameter editing (type-specific controls and ranges) | `soundSource_inst1.cpp, soundSource_inst2.cpp, soundSource_inst3.cpp, customControl*.cpp` | Implemented |
| 33 | INST Parameters > E.GUITAR Parameters | INST parameter editing (type-specific controls and ranges) | `soundSource_inst1.cpp, soundSource_inst2.cpp, soundSource_inst3.cpp, customControl*.cpp` | Implemented |
| 34 | INST Parameters > E.GUITAR Parameters | INST parameter editing (type-specific controls and ranges) | `soundSource_inst1.cpp, soundSource_inst2.cpp, soundSource_inst3.cpp, customControl*.cpp` | Implemented |
| 35 | INST Parameters > E.GUITAR Parameters | INST parameter editing (type-specific controls and ranges) | `soundSource_inst1.cpp, soundSource_inst2.cpp, soundSource_inst3.cpp, customControl*.cpp` | Implemented |
| 36 | INST Parameters > ACOUSTIC Parameters | INST parameter editing (type-specific controls and ranges) | `soundSource_inst1.cpp, soundSource_inst2.cpp, soundSource_inst3.cpp, customControl*.cpp` | Implemented |
| 37 | INST Parameters > AC BASS Parameters | INST parameter editing (type-specific controls and ranges) | `soundSource_inst1.cpp, soundSource_inst2.cpp, soundSource_inst3.cpp, customControl*.cpp` | Implemented |
| 38 | INST Parameters > E.BASS Parameters | INST parameter editing (type-specific controls and ranges) | `soundSource_inst1.cpp, soundSource_inst2.cpp, soundSource_inst3.cpp, customControl*.cpp` | Implemented |
| 39 | INST Parameters > VIO GUITAR Parameters | INST parameter editing (type-specific controls and ranges) | `soundSource_inst1.cpp, soundSource_inst2.cpp, soundSource_inst3.cpp, customControl*.cpp` | Implemented |
| 40 | INST Parameters > VIO GUITAR Parameters | INST parameter editing (type-specific controls and ranges) | `soundSource_inst1.cpp, soundSource_inst2.cpp, soundSource_inst3.cpp, customControl*.cpp` | Implemented |
| 41 | INST Parameters > POLY FX Parameters | INST parameter editing (type-specific controls and ranges) | `soundSource_inst1.cpp, soundSource_inst2.cpp, soundSource_inst3.cpp, customControl*.cpp` | Implemented |
| 42 | EFFECT Parameters | Effect parameter editing (FX1-3, amp, EQ, delay, reverb, NS, routing, master) | `stompbox_fx1.cpp, stompbox_fx2.cpp, stompbox_fx3.cpp, stompbox_*.cpp, custom*EQ*.cpp` | Implemented |
| 43 | EFFECT Parameters > FX1–FX3 | Effect parameter editing (FX1-3, amp, EQ, delay, reverb, NS, routing, master) | `stompbox_fx1.cpp, stompbox_fx2.cpp, stompbox_fx3.cpp, stompbox_*.cpp, custom*EQ*.cpp` | Implemented |
| 44 | EFFECT Parameters > FX1–FX3 | Effect parameter editing (FX1-3, amp, EQ, delay, reverb, NS, routing, master) | `stompbox_fx1.cpp, stompbox_fx2.cpp, stompbox_fx3.cpp, stompbox_*.cpp, custom*EQ*.cpp` | Implemented |
| 45 | EFFECT Parameters > FX1–FX3 | Effect parameter editing (FX1-3, amp, EQ, delay, reverb, NS, routing, master) | `stompbox_fx1.cpp, stompbox_fx2.cpp, stompbox_fx3.cpp, stompbox_*.cpp, custom*EQ*.cpp` | Implemented |
| 46 | EFFECT Parameters > FX1–FX3 | Effect parameter editing (FX1-3, amp, EQ, delay, reverb, NS, routing, master) | `stompbox_fx1.cpp, stompbox_fx2.cpp, stompbox_fx3.cpp, stompbox_*.cpp, custom*EQ*.cpp` | Implemented |
| 47 | EFFECT Parameters > FX1–FX3 | Effect parameter editing (FX1-3, amp, EQ, delay, reverb, NS, routing, master) | `stompbox_fx1.cpp, stompbox_fx2.cpp, stompbox_fx3.cpp, stompbox_*.cpp, custom*EQ*.cpp` | Implemented |
| 48 | EFFECT Parameters > FX1–FX3 | Effect parameter editing (FX1-3, amp, EQ, delay, reverb, NS, routing, master) | `stompbox_fx1.cpp, stompbox_fx2.cpp, stompbox_fx3.cpp, stompbox_*.cpp, custom*EQ*.cpp` | Implemented |
| 49 | EFFECT Parameters > FX1–FX3 | Effect parameter editing (FX1-3, amp, EQ, delay, reverb, NS, routing, master) | `stompbox_fx1.cpp, stompbox_fx2.cpp, stompbox_fx3.cpp, stompbox_*.cpp, custom*EQ*.cpp` | Implemented |
| 50 | EFFECT Parameters > FX1–FX3 | Effect parameter editing (FX1-3, amp, EQ, delay, reverb, NS, routing, master) | `stompbox_fx1.cpp, stompbox_fx2.cpp, stompbox_fx3.cpp, stompbox_*.cpp, custom*EQ*.cpp` | Implemented |
| 51 | EFFECT Parameters > FX1–FX3 | Effect parameter editing (FX1-3, amp, EQ, delay, reverb, NS, routing, master) | `stompbox_fx1.cpp, stompbox_fx2.cpp, stompbox_fx3.cpp, stompbox_*.cpp, custom*EQ*.cpp` | Implemented |
| 52 | EFFECT Parameters > FX1–FX3 | Effect parameter editing (FX1-3, amp, EQ, delay, reverb, NS, routing, master) | `stompbox_fx1.cpp, stompbox_fx2.cpp, stompbox_fx3.cpp, stompbox_*.cpp, custom*EQ*.cpp` | Implemented |
| 53 | EFFECT Parameters > FX1–FX3 | Effect parameter editing (FX1-3, amp, EQ, delay, reverb, NS, routing, master) | `stompbox_fx1.cpp, stompbox_fx2.cpp, stompbox_fx3.cpp, stompbox_*.cpp, custom*EQ*.cpp` | Implemented |
| 54 | EFFECT Parameters > AMP (AMP Modeling) | Effect parameter editing (FX1-3, amp, EQ, delay, reverb, NS, routing, master) | `stompbox_fx1.cpp, stompbox_fx2.cpp, stompbox_fx3.cpp, stompbox_*.cpp, custom*EQ*.cpp` | Implemented |
| 55 | EFFECT Parameters > AMP (AMP Modeling) | Effect parameter editing (FX1-3, amp, EQ, delay, reverb, NS, routing, master) | `stompbox_fx1.cpp, stompbox_fx2.cpp, stompbox_fx3.cpp, stompbox_*.cpp, custom*EQ*.cpp` | Implemented |
| 56 | EFFECT Parameters > CHORUS | Effect parameter editing (FX1-3, amp, EQ, delay, reverb, NS, routing, master) | `stompbox_fx1.cpp, stompbox_fx2.cpp, stompbox_fx3.cpp, stompbox_*.cpp, custom*EQ*.cpp` | Implemented |
| 57 | EFFECT Parameters > COMPRESSOR | Effect parameter editing (FX1-3, amp, EQ, delay, reverb, NS, routing, master) | `stompbox_fx1.cpp, stompbox_fx2.cpp, stompbox_fx3.cpp, stompbox_*.cpp, custom*EQ*.cpp` | Implemented |
| 58 | EFFECT Parameters > EQ 1, EQ2 | Effect parameter editing (FX1-3, amp, EQ, delay, reverb, NS, routing, master) | `stompbox_fx1.cpp, stompbox_fx2.cpp, stompbox_fx3.cpp, stompbox_*.cpp, custom*EQ*.cpp` | Implemented |
| 59 | EFFECT Parameters > MASTER DELAY | Effect parameter editing (FX1-3, amp, EQ, delay, reverb, NS, routing, master) | `stompbox_fx1.cpp, stompbox_fx2.cpp, stompbox_fx3.cpp, stompbox_*.cpp, custom*EQ*.cpp` | Implemented |
| 60 | EFFECT Parameters > DELAY 1, DELAY 2 | Effect parameter editing (FX1-3, amp, EQ, delay, reverb, NS, routing, master) | `stompbox_fx1.cpp, stompbox_fx2.cpp, stompbox_fx3.cpp, stompbox_*.cpp, custom*EQ*.cpp` | Implemented |
| 61 | EFFECT Parameters > REVERB | Effect parameter editing (FX1-3, amp, EQ, delay, reverb, NS, routing, master) | `stompbox_fx1.cpp, stompbox_fx2.cpp, stompbox_fx3.cpp, stompbox_*.cpp, custom*EQ*.cpp` | Implemented |
| 62 | EFFECT Parameters > NOISE SUPPRESSOR | Effect parameter editing (FX1-3, amp, EQ, delay, reverb, NS, routing, master) | `stompbox_fx1.cpp, stompbox_fx2.cpp, stompbox_fx3.cpp, stompbox_*.cpp, custom*EQ*.cpp` | Implemented |
| 63 | EFFECT Parameters > SEND/RETURN | Effect parameter editing (FX1-3, amp, EQ, delay, reverb, NS, routing, master) | `stompbox_fx1.cpp, stompbox_fx2.cpp, stompbox_fx3.cpp, stompbox_*.cpp, custom*EQ*.cpp` | Implemented |
| 64 | EFFECT Parameters > DIVIDER | Effect parameter editing (FX1-3, amp, EQ, delay, reverb, NS, routing, master) | `stompbox_fx1.cpp, stompbox_fx2.cpp, stompbox_fx3.cpp, stompbox_*.cpp, custom*EQ*.cpp` | Implemented |
| 65 | EFFECT Parameters > BALANCER 1–BALANCER 3 | Effect parameter editing (FX1-3, amp, EQ, delay, reverb, NS, routing, master) | `stompbox_fx1.cpp, stompbox_fx2.cpp, stompbox_fx3.cpp, stompbox_*.cpp, custom*EQ*.cpp` | Implemented |
| 66 | EFFECT Parameters > MASTER | Effect parameter editing (FX1-3, amp, EQ, delay, reverb, NS, routing, master) | `stompbox_fx1.cpp, stompbox_fx2.cpp, stompbox_fx3.cpp, stompbox_*.cpp, custom*EQ*.cpp` | Implemented |
| 67 | CONTROL ASSIGN Parameters | Control Assign, Wave Pedal, patch MIDI, guitar/bass-to-MIDI, LED color | `menuPage_assign.cpp, menuPage_assign1.cpp, menuPage_assign4.cpp, menuPage_assign5.cpp, menuPage_system.cpp` | Implemented |
| 68 | CONTROL ASSIGN Parameters > CONTROL FUNCTION | Control Assign, Wave Pedal, patch MIDI, guitar/bass-to-MIDI, LED color | `menuPage_assign.cpp, menuPage_assign1.cpp, menuPage_assign4.cpp, menuPage_assign5.cpp, menuPage_system.cpp` | Implemented |
| 69 | CONTROL ASSIGN Parameters > CONTROL FUNCTION | Control Assign, Wave Pedal, patch MIDI, guitar/bass-to-MIDI, LED color | `menuPage_assign.cpp, menuPage_assign1.cpp, menuPage_assign4.cpp, menuPage_assign5.cpp, menuPage_system.cpp` | Implemented |
| 70 | CONTROL ASSIGN Parameters > ASSIGN SETTING | Control Assign, Wave Pedal, patch MIDI, guitar/bass-to-MIDI, LED color | `menuPage_assign.cpp, menuPage_assign1.cpp, menuPage_assign4.cpp, menuPage_assign5.cpp, menuPage_system.cpp` | Implemented |
| 71 | CONTROL ASSIGN Parameters > ASSIGN SETTING | Control Assign, Wave Pedal, patch MIDI, guitar/bass-to-MIDI, LED color | `menuPage_assign.cpp, menuPage_assign1.cpp, menuPage_assign4.cpp, menuPage_assign5.cpp, menuPage_system.cpp` | Implemented |
| 72 | CONTROL ASSIGN Parameters > ASSIGN SETTING | Control Assign, Wave Pedal, patch MIDI, guitar/bass-to-MIDI, LED color | `menuPage_assign.cpp, menuPage_assign1.cpp, menuPage_assign4.cpp, menuPage_assign5.cpp, menuPage_system.cpp` | Implemented |
| 73 | CONTROL ASSIGN Parameters > ASSIGN SETTING | Control Assign, Wave Pedal, patch MIDI, guitar/bass-to-MIDI, LED color | `menuPage_assign.cpp, menuPage_assign1.cpp, menuPage_assign4.cpp, menuPage_assign5.cpp, menuPage_system.cpp` | Implemented |
| 74 | CONTROL ASSIGN Parameters > ASSIGN SETTING | Control Assign, Wave Pedal, patch MIDI, guitar/bass-to-MIDI, LED color | `menuPage_assign.cpp, menuPage_assign1.cpp, menuPage_assign4.cpp, menuPage_assign5.cpp, menuPage_system.cpp` | Implemented |
| 75 | CONTROL ASSIGN Parameters > ASSIGN SETTING | Control Assign, Wave Pedal, patch MIDI, guitar/bass-to-MIDI, LED color | `menuPage_assign.cpp, menuPage_assign1.cpp, menuPage_assign4.cpp, menuPage_assign5.cpp, menuPage_system.cpp` | Implemented |
| 76 | CONTROL ASSIGN Parameters > ASSIGN SETTING | Control Assign, Wave Pedal, patch MIDI, guitar/bass-to-MIDI, LED color | `menuPage_assign.cpp, menuPage_assign1.cpp, menuPage_assign4.cpp, menuPage_assign5.cpp, menuPage_system.cpp` | Implemented |
| 77 | CONTROL ASSIGN Parameters > Virtual Expression Pedal System (Wave Pedal) | Control Assign, Wave Pedal, patch MIDI, guitar/bass-to-MIDI, LED color | `menuPage_assign.cpp, menuPage_assign1.cpp, menuPage_assign4.cpp, menuPage_assign5.cpp, menuPage_system.cpp` | Implemented |
| 78 | CONTROL ASSIGN Parameters > PATCH MIDI | Control Assign, Wave Pedal, patch MIDI, guitar/bass-to-MIDI, LED color | `menuPage_assign.cpp, menuPage_assign1.cpp, menuPage_assign4.cpp, menuPage_assign5.cpp, menuPage_system.cpp` | Implemented |
| 79 | CONTROL ASSIGN Parameters > GUITAR TO MIDI/BASS TO MIDI | Control Assign, Wave Pedal, patch MIDI, guitar/bass-to-MIDI, LED color | `menuPage_assign.cpp, menuPage_assign1.cpp, menuPage_assign4.cpp, menuPage_assign5.cpp, menuPage_system.cpp` | Implemented |
| 80 | CONTROL ASSIGN Parameters > LED COLOR | Control Assign, Wave Pedal, patch MIDI, guitar/bass-to-MIDI, LED color | `menuPage_assign.cpp, menuPage_assign1.cpp, menuPage_assign4.cpp, menuPage_assign5.cpp, menuPage_system.cpp` | Implemented |
| 81 | SYSTEM Parameters | System parameter pages (control mode, I/O, routing, hardware, MIDI, tuner) | `menuPage_system.cpp, menuPage_setup.cpp, menuPage_tuner.cpp` | Implemented |
| 82 | SYSTEM Parameters > IN/OUT SETTING | System parameter pages (control mode, I/O, routing, hardware, MIDI, tuner) | `menuPage_system.cpp, menuPage_setup.cpp, menuPage_tuner.cpp` | Implemented |
| 83 | SYSTEM Parameters > IN/OUT SETTING | System parameter pages (control mode, I/O, routing, hardware, MIDI, tuner) | `menuPage_system.cpp, menuPage_setup.cpp, menuPage_tuner.cpp` | Implemented |
| 84 | SYSTEM Parameters > IN/OUT SETTING | System parameter pages (control mode, I/O, routing, hardware, MIDI, tuner) | `menuPage_system.cpp, menuPage_setup.cpp, menuPage_tuner.cpp` | Implemented |
| 85 | SYSTEM Parameters > IN/OUT SETTING | System parameter pages (control mode, I/O, routing, hardware, MIDI, tuner) | `menuPage_system.cpp, menuPage_setup.cpp, menuPage_tuner.cpp` | Implemented |
| 86 | SYSTEM Parameters > IN/OUT SETTING | System parameter pages (control mode, I/O, routing, hardware, MIDI, tuner) | `menuPage_system.cpp, menuPage_setup.cpp, menuPage_tuner.cpp` | Implemented |
| 87 | SYSTEM Parameters > HARDWARE SETTING | System parameter pages (control mode, I/O, routing, hardware, MIDI, tuner) | `menuPage_system.cpp, menuPage_setup.cpp, menuPage_tuner.cpp` | Implemented |
| 88 | SYSTEM Parameters > PLAY OPTION | System parameter pages (control mode, I/O, routing, hardware, MIDI, tuner) | `menuPage_system.cpp, menuPage_setup.cpp, menuPage_tuner.cpp` | Implemented |
| 89 | SYSTEM Parameters > MIDI | System parameter pages (control mode, I/O, routing, hardware, MIDI, tuner) | `menuPage_system.cpp, menuPage_setup.cpp, menuPage_tuner.cpp` | Implemented |
| 90 | SYSTEM Parameters > MIDI | System parameter pages (control mode, I/O, routing, hardware, MIDI, tuner) | `menuPage_system.cpp, menuPage_setup.cpp, menuPage_tuner.cpp` | Implemented |
| 91 | SYSTEM Parameters > TUNER | System parameter pages (control mode, I/O, routing, hardware, MIDI, tuner) | `menuPage_system.cpp, menuPage_setup.cpp, menuPage_tuner.cpp` | Implemented |
| 92 | SYSTEM Parameters > FACTORY RESET | Factory-reset reference; typically hardware-side operation | `menuPage_system.cpp` | Partial (device-centric) |
| 93 | Saving a Sound (WRITE) | Patch write / initialize flow | `patchWriteDialog.cpp, SysxIO.cpp` | Implemented/Partial |
| 94 | Saving a Sound (WRITE) | Patch write / initialize flow | `patchWriteDialog.cpp, SysxIO.cpp` | Implemented/Partial |
| 95 | Connecting to a Computer | USB/computer connection prerequisites and mode | `midiIO.cpp, SysxIO.cpp, docs/sy1000_manuals.md` | Partial |
| 96 | Troubleshooting | Troubleshooting reference for support workflows | `docs/diagnostics.md, SysxIO.cpp, midiIO.cpp` | Partial |
| 97 | Troubleshooting | Troubleshooting reference for support workflows | `docs/diagnostics.md, SysxIO.cpp, midiIO.cpp` | Partial |
| 98 | Block Diagram | Block diagram parity vs signal-chain canvas | `floorBoard.cpp, floorBoardDisplay.cpp, stompBox.cpp` | Implemented |
| 99 | Block Diagram | Block diagram parity vs signal-chain canvas | `floorBoard.cpp, floorBoardDisplay.cpp, stompBox.cpp` | Implemented |

## Notes

- Pages 22-80 are primarily parameter-definition pages; the app maps these through `midi.xml`/`SysxIO` parameter addressing and related edit widgets.
- Pages 92, 95-97 include hardware/driver/recovery actions that are only partially represented in-app.
- Use this file with `docs/sy1000_owner_manual_ui_map.md` for complete Owner + Reference workflow coverage.
