# SY-1000 FloorBoard

Qt desktop editor for the Boss SY-1000 guitar synthesizer, with current effort focused on accurate UI mapping, cleaner layout, and reliable sync between signal-chain tiles and effect detail pages.

## Credits

This project is a fork of **SY1000FloorBoard** by **Colin Willcocks** (gumtownbassman@yahoo.com), originally published on SourceForge:

> [https://sourceforge.net/projects/sy1000floorboard/files/SY1000FloorBoard/](https://sourceforge.net/projects/sy1000floorboard/files/SY1000FloorBoard/)

Colin is the originator of the entire application: MIDI SysEx communication, patch storage, signal-chain rendering, custom widget library, and Qt project structure. This fork extends his work with UI accuracy improvements, a QML migration, and a signal-chain layout rewrite.

See [`ABOUT.md`](ABOUT.md) for a full description of the technology stack, what has changed, and why.

## Start here

Read these in order:

1. `CLAUDE.md` — agent conventions, build command, key docs list
2. `docs/repo_layout.md` — repo structure and file placement policy
3. `docs/ui/sy1000_boss_layout_notes.md` — visual patterns and Boss terminology
4. `docs/ui/sy1000_claude_compare_matrix.md` — current implementation vs Boss spec
5. `HANDOFF.md` — active migration tasks (Phase 5 QML migration)

## Docs topic map

| Topic | Doc |
|---|---|
| Agent/AI conventions & build | `CLAUDE.md` |
| Repo structure | `docs/repo_layout.md` |
| Active migration tasks | `HANDOFF.md` |
| Visual patterns & Boss layout rules | `docs/ui/sy1000_boss_layout_notes.md` |
| Impl vs Boss spec (block-by-block) | `docs/ui/sy1000_claude_compare_matrix.md` |
| Dropdown typography & shared combo path | `docs/ui/dropdown_inventory.md` |
| Setting label typography | `docs/ui/setting_label_inventory.md` |
| Design tokens (Boss CSS extraction) | `docs/ui/ux_improvement_report.md` |
| Layout regression tracking | `docs/ui/layout_regression_log.md` |
| Signal-chain architecture & known bugs | `docs/agent_handoff.md` |
| Panel parameter audit (block-level) | `docs/panel_audit_report.md` |
| Page → control → MIDI address index | `docs/ui/page_control_inventory.md` |
| Official manual index | `docs/sy1000_manuals.md` |
| Parameter inventory (instrument + FX types) | `docs/sy1000_parameter_inventory.md` |
| Refactor direction (AppServices migration) | `docs/refactor_audit.md` |
| Diagnostics & log triage | `docs/diagnostics.md` |

## Repository shape

- `src/` - application source code
- `docs/` - manual mapping, UI notes, handoff docs
- `images/` - app assets
- `qss/` - theme stylesheets
- `build/` - generated output and archived local build artifacts

Qt project files remain at the repo root:

- `SY-1000FloorBoard.pro`
- `SY-1000FloorBoard.pri`
- `SY-1000FloorBoard.qrc`

## Build and install

### What you need

| Tool | Version | Where to get it |
|---|---|---|
| Qt | 6.5 or later (LTS recommended) | https://www.qt.io/download — install the open-source edition |
| C++ compiler | see platform notes below | bundled with Qt installer on Windows; Xcode on macOS |
| Git | any recent version | https://git-scm.com |

Qt is the only significant dependency. Everything else (RtMidi, XML parsing, QML runtime) is either bundled in the repo or included with Qt.

---

### Windows (PC)

**Step 1 — Install Qt**

Download the Qt online installer from https://www.qt.io/download-qt-installer and run it. During setup, select:

- **Qt 6.5** (or the latest LTS) → **MSVC 2022 64-bit** (preferred) or **MinGW 64-bit**
- **Qt Creator** (included by default — gives you an IDE if you want one)
- **Qt Quick** and **Qt Quick Controls** modules (required for the QML UI)

If you chose MSVC, also install **Visual Studio 2022** (the free Community edition works) with the "Desktop development with C++" workload. Qt Creator will find the compiler automatically.

**Step 2 — Edit the .pro file for your WinMM.Lib path**

The Windows MIDI subsystem requires `WinMM.Lib`. Open `SY-1000FloorBoard.pro` and find this section:

```
win32:contains(QMAKE_HOST.arch, x86_64):{
    exists("c:\Qt\SY-1000\win64\WinMM.Lib")
    {
        LIBS += c:\Qt\SY-1000\win64\WinMM.Lib
    }
```

Change both paths to where `WinMM.Lib` lives on your machine. It is typically found at:

```
C:\Program Files (x86)\Windows Kits\10\Lib\<version>\um\x64\WinMM.Lib
```

**Step 3 — Open and build in Qt Creator**

1. Open Qt Creator
2. File → Open File or Project → select `SY-1000FloorBoard.pro`
3. Qt Creator will ask you to configure the project — select your Qt 6 kit
4. Click the green Play button (or press Ctrl+R) to build and run

The built executable will appear in `build/packager/`.

**Step 3 (alternative) — Build from the command line**

Open the Qt 6 MinGW or MSVC command prompt (from the Start menu, installed with Qt), then:

```bat
cd path\to\SY-1000
qmake SY-1000FloorBoard.pro
mingw32-make -j4
```

Or with MSVC:

```bat
qmake SY-1000FloorBoard.pro
nmake
```

---

### macOS

**Step 1 — Install Xcode**

Install Xcode from the Mac App Store, then run:

```bash
xcode-select --install
```

**Step 2 — Install Qt**

Download the Qt online installer from https://www.qt.io/download-qt-installer. During setup select:

- **Qt 6.5** (or latest LTS) → **macOS**
- **Qt Quick** and **Qt Quick Controls** modules

**Step 3 — Add Qt to your PATH**

After installing, add the Qt `bin` directory to your shell path. For example, if Qt is at `~/Qt`:

```bash
export PATH="$HOME/Qt/6.5.3/macos/bin:$PATH"
```

Add this line to your `~/.zshrc` (or `~/.bash_profile`) to make it permanent.

**Step 4 — Build**

```bash
git clone https://github.com/bsewell/SY-1000.git
cd SY-1000
qmake SY-1000FloorBoard.pro
make -j$(sysctl -n hw.logicalcpu)
```

Built app bundle: `build/packager/SY-1000FloorBoard.app`

---

### Linux

MIDI uses ALSA. Install the development package before building:

```bash
# Debian/Ubuntu
sudo apt install libasound2-dev

# Fedora
sudo dnf install alsa-lib-devel
```

Then follow the same qmake/make steps as macOS, using a Linux Qt kit.

---

### Connecting the SY-1000

1. Connect the SY-1000 to your computer via USB
2. Launch the app
3. Go to **Preferences** and select the SY-1000 as both the MIDI input and output device
4. The app will send a bulk request and populate the patch list

**Note:** Close Boss Tone Studio before launching FloorBoard — both apps use the same MIDI port and will conflict if open simultaneously.

## Vibe-coding workflow

This project benefits from fast iteration, but only if the iteration is disciplined.

### 1. Pick one surface at a time

Do not mix these in one pass unless the change truly requires it:

- signal-chain layout
- effect-page layout
- power-toggle behavior
- instrumentation or debug capture
- repo-structure cleanup

If you mix them, review gets noisy and regressions get harder to localize.

### 2. Use the docs before changing UI

Before changing labels, spacing, tabs, or controls, compare against:

- `docs/ui/sy1000_boss_layout_notes.md`
- `docs/ui/sy1000_claude_compare_matrix.md`
- `docs/ui/dropdown_inventory.md`
- `docs/sy1000_manuals.md`

The target is not generic cleanup. The target is closer SY-1000 behavior and clearer presentation.

### 3. Treat state sync as non-negotiable

When a block can turn on or off, these must stay aligned:

- signal-chain rectangle color/state
- on/off switch on the detail page
- enabled or disabled parameter controls

If one changes without the others, the fix is incomplete.

### 4. Keep layout rules consistent

When editing effect pages:

- the on/off control should anchor hard left
- the first row should read as a single coherent strip
- spacing under the header should not drift by effect type
- rows should look intentionally aligned, not individually improvised

### 5. Build after each meaningful pass

Run the build command from the `## Build` section above.

Do not stack several speculative edits without rebuilding.

### 6. Commit by intent

Good commit boundaries:

- `signal-chain toggle sync`
- `effect-page left rail alignment`
- `repo layout cleanup`
- `documentation updates`

Bad commit boundaries:

- mixed UI, docs, instrumentation, and deploy changes in one commit

## Working expectations for another developer or agent

- Prefer narrow, explainable changes.
- Keep source under `src/`.
- Do not commit generated output from `build/`, `Makefile`, `.qmake.stash`, or `qrc_*.cpp`.
- If behavior is unclear, inspect the SY-1000 reference docs before redesigning it.
- If the task is handoff-oriented, update the relevant docs as part of the same pass.

## Current handoff goal

Prepare the project so another developer can:

- build it without guessing the structure
- understand where the UI guidance lives
- compare current implementation against Boss references
- work on one surface at a time without tripping over unrelated changes
