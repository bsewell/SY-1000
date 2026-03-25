# SY-1000 Repository Layout

This repository now uses a conservative layout intended to make review and handoff easier without breaking the Qt project structure.

## Top-level

- `src/` - application source code, widgets, pages, stomp boxes, and support classes.
- `docs/` - manual notes, UI comparison notes, deploy logs, and handoff documentation.
- `images/` - image assets used by the application.
- `qss/` - theme stylesheets.
- `build/` - generated build output, app bundle output, and archived local-only artifacts.

## Root file policy

The repository root is intentionally not empty. These root items fall into three categories.

### 1. Must stay at root

These are part of the active Qt project setup and should stay where they are unless the build system is being deliberately redesigned:

- `SY-1000FloorBoard.pro`
- `SY-1000FloorBoard.pri`
- `SY-1000FloorBoard.qrc`
- `SY-1000FloorBoard.rc`
- `MyInfo.plist`
- `README.md`
- `AGENTS.md`
- `CLAUDE.md`
- `.gitignore`
- `deploy.sh`
- `build-and-deploy.sh`

### 2. Intentionally left at root for now

These files and folders are wired into the current Qt resource file, project file, platform setup, or runtime expectations. They can be moved later, but only as a dedicated refactor that updates all related paths together:

- `SY-1000FloorBoard.icns`
- `SY-1000FloorBoard.ico`
- `about`
- `license.txt`
- `preferences.xml.dist`
- `default_B.tsl`
- `default_G.tsl`
- `default_bass.syx`
- `default_guitar.syx`
- `default_vg99.syx`
- `system.syx`
- `midi.xml`
- `midi.xsd`
- `VG99_midi.xsd`
- `vg99_midi.xml`
- `language_*.ts`
- `language_*.qm`
- `windows/`
- `win64/`

### 3. Generated or local-only noise

These are not part of the intended repository structure and should not be committed:

- `Makefile`
- `.qmake.stash`
- `qrc_*.cpp`
- `moc_*.cpp`
- `*.orig`
- `.qtcreator/`
- `.qtc_clangd/`

## Project files kept at the root

These stay at the root because qmake and Qt Creator expect them there and moving them would add avoidable churn:

- `SY-1000FloorBoard.pro`
- `SY-1000FloorBoard.pri`
- `SY-1000FloorBoard.qrc`
- `SY-1000FloorBoard.rc`
- `MyInfo.plist`

## Generated and local-only files

Generated files are ignored and should not be committed:

- `build/`
- `Makefile`
- `.qmake.stash`
- `qrc_*.cpp`
- `moc_*.cpp`
- `*.orig`

Local legacy artifacts that were previously left in the repo root were moved under:

- `build/_legacy_root_artifacts/`

## Current organization policy

- Keep all real source files under `src/`.
- Keep build output under `build/`.
- Do not move resource files or project files again unless there is a clear build reason.
- If a future cleanup is needed, do it as a dedicated pass so functional work and repo-structure work do not mix.
