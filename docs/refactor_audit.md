# SY-1000 FloorBoard Refactor Audit (2026-03-06)

> **Staleness note**: Baseline metrics and migration status reflect 2026-03-06. For current migration state, run:
> `git log --oneline -- src/appservices.cpp src/mainWindow.cpp src/floorBoardDisplay.cpp src/initPatchListMenu.cpp`
> Service facade implementation: `src/appservices.h`, `src/appservices.cpp`

## Current baseline
- Approximate file count: 1838
- Legacy `SIGNAL()/SLOT()` macro connects: 725
- Direct singleton fetches (`::Instance()`): 703
- Heavy coupling concentrated in:
  - `SysxIO.cpp`
  - `floorBoardDisplay.cpp`
  - `mainWindow.cpp`
  - `stompBox.cpp`
  - `bankTreeList.cpp`

## Communication risk points
1. **Global singleton fan-out**
   - `Preferences`, `SysxIO`, `midiIO`, `MidiTable` are accessed from most UI modules.
   - Any lifecycle/state issue in one service propagates everywhere.
2. **String-based signal/slot connections**
   - Runtime-only validation increases risk of silent disconnects after signature edits.
3. **UI modules handling transport concerns**
   - Several UI classes call `SysxIO` directly instead of going through a single interaction layer.
4. **Threaded MIDI + UI coupling**
   - `midiIO` thread state is read/written across modules; synchronization assumptions are spread.

## Refactor direction
1. **Service facade introduced**
   - Added `AppServices` (`appservices.h/.cpp`) as a single access point for core services.
   - Goal: remove direct singleton knowledge from UI classes over time.
2. **Phase migration started**
   - `mainWindow.cpp`, `floorBoardDisplay.cpp`, `initPatchListMenu.cpp` now use `AppServices`.
3. **Next migration targets**
   - Convert high-traffic runtime paths to typed connects:
     - Startup + main routing paths in `mainWindow.cpp`
     - Device/session routing in `floorBoardDisplay.cpp`
     - Patch browser flows in `bankTreeList.cpp`
4. **Behavior lock checks before each batch**
   - Build
   - Deploy/sign
   - Launch + startup log scan
   - Online/offline connect toggle
   - User/preset switch + patch load/write smoke checks

## Definition of done for "talks to each other properly"
- No string-based connects in core runtime path (main + floorboard + sysx + midi).
- No direct singleton calls in UI layer (all through service facade and interaction layer).
- Startup and online/offline transitions are deterministic in logs.
- Preset/user operations and patch import/write pass repeated smoke tests.
