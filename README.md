# SY-1000 FloorBoard

Qt desktop editor for the Boss SY-1000 guitar synthesizer.

## Credits

This project is a fork of **SY1000FloorBoard** by **Colin Willcocks** (gumtownbassman@yahoo.com), originally published on SourceForge:

> [https://sourceforge.net/projects/sy1000floorboard/files/SY1000FloorBoard/](https://sourceforge.net/projects/sy1000floorboard/files/SY1000FloorBoard/)

Colin is the originator of the entire application: MIDI SysEx communication, patch storage, signal-chain rendering, custom widget library, and Qt project structure. This fork extends his work with UI accuracy improvements, a QML migration, and a signal-chain layout rewrite.

See [`ABOUT.md`](ABOUT.md) for a full description of the technology stack and what has changed.

---

## Current status

### What is working

- **Full patch editor** — load, edit, and write patches to the SY-1000 via USB MIDI
- **Patch browser** — scrollable patch list with live selection
- **All effect panels** — every effect block has a working detail editor: INST 1/2/3 (all 9 instrument types), AMP, CS, DS, NS, CE, DD1/DD2/DD3, RV, FX1/FX2/FX3, EQ1/EQ2, FV1/FV2, LP, all output blocks (MAIN OUT, SUB OUT, DIVIDER, MIXER, MASTER)
- **System pages** — MIDI, USB, Input/Output, Hardware, Knob, Play Option, Total, Auto Power Off, Guitar to MIDI, GK/Normal Set Select
- **Pedal/GK pages** — all pedal and GK assignment controls
- **Assign pages** — all 16 assign slots
- **Tuner** — accessible from System sidebar without needing a device connection
- **Parameter sync** — knobs, switches, and dropdowns update in real time when parameters change externally (MIDI receive, bulk load, tile toggle)
- **5 themes** — aqua, black, blue, green, white
- **Boss Tone Studio design tokens** — colors and typography match the official Boss software

### What is NOT working — signal chain layout

> **This is the primary outstanding bug. Do not consider the editor release-ready until this is resolved.**

The signal-chain visualizer (the graphical block diagram at the top of the main window showing how effects are routed) has a **layout instability problem**. Specifically:

- **Blocks do not reliably hold their positions.** After patch changes, bulk loads, or window resizes, blocks may shift from their correct columns. The 3-zone compaction algorithm (`src/floorBoard.cpp`) is implemented and mostly correct, but edge cases exist where blocks drift.
- **Branch wires can stretch incorrectly.** When FX blocks move between rows (e.g. FX3 compacted into Zone B or C), the branch wire endpoints follow the block's type-ID position rather than its actual rendered row, causing polygon points to stretch far to the right. Y-row guards were added to mitigate this but the fix is not complete under all patch configurations.
- **Balancer placement is fragile.** BAL2 and BAL3 column caps are computed from zone block counts, but patches with unusual block distributions can cause BAL2 or BAL3 to be placed behind their correct column, or to jump forward inconsistently on patch changes.
- **Wire rendering is not frame-stable.** The signal chain can visually flicker or re-lay on consecutive refreshes of the same patch state.

The layout spec is fully documented in `docs/ui/sy1000_boss_layout_notes.md` §5a. The code is in `src/floorBoard.cpp` (`update_structure()`) and `src/chainLayout.cpp`. The algorithm is understood; what remains is hardening the edge cases and making the refresh path idempotent.

### Other known issues

- User preset names show as "init patch" until the device connects and sends a bulk SysEx dump
- MIDI port conflict if Boss Tone Studio is open simultaneously — close BTS before launching FloorBoard
- 13 Assign pages + menuPage_midi are still implemented as C++ widgets (not yet migrated to QML)

---

## Why QML — agentic development context

This project is being developed using **Claude Code** (Anthropic's AI coding agent) as the primary development tool. The QML migration is a direct result of that workflow.

### The problem with C++ widgets and AI agents

The original editor is built entirely from Qt Widgets — imperative C++ code that constructs controls, lays them out, and wires signals manually. Files like `src/stompbox_fx1.cpp` are 1,000+ lines of widget construction code. For a human developer reading it in an IDE, this is manageable. For an AI agent working via file reads and edits, it has serious problems:

- **Hard to read declaratively.** To understand what a panel looks like, an agent must mentally execute dozens of `addWidget`, `setGeometry`, and `setLayout` calls in sequence. The visual structure is not visible in the source.
- **Hard to edit safely.** Changing a layout means tracking pixel offsets, row indices, and parent/child widget hierarchies across hundreds of lines. An agent making a small change can silently break an adjacent row.
- **Hard to verify.** There is no single source of truth for what a panel should contain — the code, the running app, and the Boss reference manual all need to be cross-referenced manually.

### How QML fixes this for agents

QML is a declarative language. A panel is described as a tree of items with explicit properties — position, size, binding to data, visual state. An agent can:

- **Read a QML file and immediately understand the layout** — the structure mirrors the visual hierarchy
- **Add a new control by adding one block** — no cascading widget constructor changes
- **Bind to a parameter in one line** — `value: paramBridge.getValue(hex1, hex2, hex3)` replaces 20 lines of signal/slot wiring
- **Verify correctness against the Boss spec** — label names and parameter order are visible at a glance

This is why the migration was done panel by panel, not as a single rewrite. Each migrated panel makes the agent's next task easier and less risky. The end goal is a codebase where any panel can be read, understood, and modified by an AI agent in a single focused pass — without needing to understand the full C++ widget tree.

### What was migrated

Every major panel has been moved from C++ widget code to QML:

| Area | Status |
|---|---|
| INST 1 / 2 / 3 (all 9 instrument type sub-panels) | QML |
| All stomp boxes (NS, CS, DS, AMP, FX1/2/3, CE, DD1/2/3, RV, EQ1/2, FV1/2, LP) | QML |
| Output blocks (Main Out, Sub Out, Divider, Mixer, Master) | QML |
| System pages (all 14 sub-pages) | QML |
| Pedal / GK pages | QML |
| Tuner | QML |
| Master, Setup pages | QML |
| All 16 Assign pages | QML |
| Patch browser | QML |
| Assign pages 1–16 | QML |
| menuPage_midi + menuPage_assign (C++ wrappers) | **Pending** |

### QML component library

A shared component library lives in `qml/`:

| Component | Purpose |
|---|---|
| `SyTheme.qml` | Boss-exact design tokens — colors, type scale, spacing |
| `SyKnob.qml` | Filmstrip knob with external parameter sync |
| `SySwitch.qml` | On/off toggle |
| `SyComboBox.qml` | Styled dropdown bound to parameter bridge |
| `FilmstripKnob.qml` | Frame-animated knob from PNG filmstrip |
| `SySettingRow.qml` | Standard label + control row |
| `SySectionLabel.qml` | Section divider label |
| `SyTabBar.qml` | Tab navigation bar |
| `SyPanelBase.qml` | Base layout for all panels |
| `SyModeSelector.qml` | Mode/type selector header |

### C++ bridge

All QML controls communicate through `src/parameterBridge.cpp` — a `QObject` singleton exposed to QML. It translates Qt property bindings into SY-1000 MIDI SysEx hex addresses and back. External changes (MIDI receive, bulk load, stompbox tile clicks) propagate to QML via `parameterChanged` signals, so panels stay in sync without polling.

---

## Repository structure

```
src/          C++ application source
qml/          QML panels and component library
qss/          Qt Style Sheets (5 themes)
images/       PNG assets for widgets and signal-chain tiles
i18n/         Translation source (.ts) and compiled (.qm) files
patches/      Default patch files (.syx, .tsl)
data/         MIDI XML data, XSD schemas
docs/         Manual mappings, UI audit docs, signal-chain spec, handoff notes
tools/        Build tools (filmstrip generator, version stamp)
fonts/        Bundled fonts
build/        Generated output (not committed)
```

Qt project files at root: `SY-1000FloorBoard.pro`, `.pri`, `.qrc`

---

## Build and install

### What you need

| Tool | Version | Where to get it |
|---|---|---|
| Qt | 6.5 or later (LTS recommended) | https://www.qt.io/download — install the open-source edition |
| C++ compiler | see platform notes below | bundled with Qt installer on Windows; Xcode on macOS |
| Git | any recent version | https://git-scm.com |

Qt is the only significant dependency. RtMidi, XML parsing, and the QML runtime are all bundled or included with Qt.

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

Add this line to your `~/.zshrc` to make it permanent.

**Step 4 — Build**

```bash
git clone https://github.com/bsewell/SY-1000.git
cd SY-1000
./build.sh
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

Then follow the same steps as macOS using a Linux Qt kit.

---

### Connecting the SY-1000

1. Connect the SY-1000 to your computer via USB
2. Launch the app
3. Go to **Preferences** and select the SY-1000 as both the MIDI input and output device
4. The app will send a bulk request and populate the patch list

**Note:** Close Boss Tone Studio before launching FloorBoard — both apps use the same MIDI port and will conflict if open simultaneously.

---

## For developers and AI agents

### Docs topic map

| Topic | Doc |
|---|---|
| Agent/AI conventions & build | `CLAUDE.md` |
| Repo structure | `docs/repo_layout.md` |
| Active migration tasks | `HANDOFF.md` |
| Visual patterns & Boss layout rules | `docs/ui/sy1000_boss_layout_notes.md` |
| Signal-chain layout spec (§5a) | `docs/ui/sy1000_boss_layout_notes.md` |
| Impl vs Boss spec (block-by-block) | `docs/ui/sy1000_claude_compare_matrix.md` |
| Dropdown typography & shared combo path | `docs/ui/dropdown_inventory.md` |
| Setting label typography | `docs/ui/setting_label_inventory.md` |
| Design tokens (Boss CSS extraction) | `docs/ui/ux_improvement_report.md` |
| Signal-chain architecture & known bugs | `docs/agent_handoff.md` |
| Panel parameter audit (block-level) | `docs/panel_audit_report.md` |
| Official manual index | `docs/sy1000_manuals.md` |
| Parameter inventory (instrument + FX types) | `docs/sy1000_parameter_inventory.md` |
| Refactor direction | `docs/refactor_audit.md` |
| Diagnostics & log triage | `docs/diagnostics.md` |

### Working conventions

- Source code lives under `src/`
- Build output lives under `build/` — do not commit
- Generated files (`Makefile`, `.qmake.stash`, `qrc_*.cpp`, `moc_*.cpp`) — do not commit
- When changing effect-page layouts, preserve a consistent left-edge on/off rail and consistent row alignment
- UI behavior must stay synchronized between signal-chain tiles, on/off switches, and parameter enabled state
- Keep functional changes separate from repo-structure changes
- When in doubt, compare against Boss reference docs before redesigning behavior
