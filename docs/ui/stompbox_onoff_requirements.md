# Stomp Box On/Off UI Requirements

This document defines the visual states, interaction behavior, and synchronization contract for all stomp box on/off (power) functionality in SY-1000 FloorBoard.

## 1) Signal Chain Tile — On/Off Toggle

Every effect stomp box in the signal chain supports a single-click power toggle:

| State | Visual | Image |
|-------|--------|-------|
| **ON**  | Full-color tile image | `{effect}_on.png` |
| **OFF** | Desaturated/dimmed tile image | `{effect}_off.png` |

**Click behavior** (stompBox.cpp `mouseReleaseEvent`):
- Single click toggles `this->sw` (on/off state).
- Emits `valueChanged(sw)` which sends SysEx to the device.
- Tile repaints with the opposite image.
- Excluded from toggle: output blocks (id 24–27) and blocks with "void" addressing.

**Hover behavior**:
- Hover image shows on mouse-over (`{effect}_hover.png`).
- Hover clears after 2-second timeout.

## 2) Edit Panel Header — No Power Button

The QML StompHeader bar does **not** contain a power button (⏻). Power is controlled exclusively by clicking the signal chain tile. The header shows:

- **Left**: Block title (e.g., "AMP", "DELAY 1", "REVERB")
- **Right**: VARIATION dropdown (optional)
- **Background**: Muted accent color per effect type

This matches the requirement that clicking the signal chain tile is the single power control point. The edit panel header is purely informational.

## 3) Synchronization Contract

The same SysEx address (`hex0/hex1/hex2/hex3` with hex3="00" for power) must drive both:
1. The signal chain tile ON/OFF image
2. The QML panel's power state awareness

**Update paths**:

### Path A: Signal chain tile click → device
1. User clicks tile in signal chain
2. `stompBox::mouseReleaseEvent` toggles `this->sw`
3. `valueChanged(sw)` sends SysEx to device
4. Device acknowledges; `paramBridge.onParameterChanged` fires
5. Any QML panel listening to the same address updates

### Path B: Device change → signal chain + QML
1. Device sends parameter change (external control, MIDI, etc.)
2. `paramBridge.onParameterChanged` fires
3. Signal chain tile updates via `updateSignal()` slot
4. QML panel updates via `Connections` to `paramBridge`

## 4) Effect-Specific Power Addresses

All effect power parameters follow the pattern: `hex0="10", hex1=inst, hex2=block, hex3="00"`

| Effect | Block hex2 (Guitar) | Block hex2 (Bass) |
|--------|---------------------|-------------------|
| CS (Compressor) | 36 | 34 |
| DS (Distortion) | 37 | 35 |
| AMP | 38 | 36 |
| NS (Noise Suppressor) | 39 | 37 |
| EQ1 | 3A | 38 |
| EQ2 | 3B | — |
| FX1 | 48 | 42 |
| FX2 | 49 | 43 |
| FX3 | 4A | 44 |
| DD1 (Delay 1) | 3C | 39 |
| DD2 (Delay 2) | 3D | 3A |
| DD3 (Delay 3) | 3E | 3B |
| CE (Chorus) | 3F | 3C |
| RV (Reverb) | 40 | 3D |
| FV1 (Foot Volume 1) | 41 | 3E |
| FV2 (Foot Volume 2) | 42 | 3F |
| LP (Loop) | 43 | 40 |

## 5) Disabled-State Visual Rules

When an effect is OFF:
- Signal chain tile shows desaturated image (immediate visual feedback)
- Edit panel controls remain visible but dimmed (alpha ~35–45%)
- Controls do not reflow — layout stays stable
- No controls become hidden; only opacity changes

## 6) Stomp Box Inventory — QML Panel Status

All effect stomp boxes now use QML panels via `setQmlPage()`:

| Stomp | QML Panel | Header via SyPanelBase |
|-------|-----------|----------------------|
| AMP | AmpPanel.qml | Yes |
| DS | DsPanel.qml | Yes |
| CS | CsPanel.qml | Yes |
| NS | NsPanel.qml | Yes |
| EQ1 | Eq1Panel.qml | Yes |
| EQ2 | Eq2Panel.qml | Yes |
| FX1 | Fx1Panel.qml | Yes |
| FX2 | Fx2Panel.qml | Yes |
| FX3 | Fx3Panel.qml | Yes |
| DD1 | Dd1Panel.qml | Yes |
| DD2 | Dd2Panel.qml | Yes |
| DD3 | Dd3Panel.qml | Yes |
| CE | CePanel.qml | Yes |
| RV | RvPanel.qml | Yes |
| FV1 | Fv1Panel.qml | Yes |
| FV2 | Fv2Panel.qml | Yes |
| LP | LpPanel.qml | Yes |
| DIV | DividerPanel.qml | Yes |
| MIX | MixerPanel.qml | Yes |
| BAL1 | Balancer1Panel.qml | Yes |
| BAL2 | Balancer2Panel.qml | Yes |
| BAL3 | Balancer3Panel.qml | Yes |
| MST | MasterPanel.qml | Yes |

## 7) QML Panel Structure Contract

Every effect panel must follow this structure:

```
SyPanelBase {
    title: "EFFECT NAME"
    powerHex0/1/2/3: matching the C++ setButton() addresses
    accentColor: per-effect accent

    // Content goes into SyPanelBase's scrollable content area
    Column {
        // Mode selector (if applicable)
        SyModeSelector { ... }
        // Parameter controls
        Flow { ... }
    }
}
```

**Do NOT**:
- Set `showHeader: false` and add a manual StompHeader + Flickable (causes nested Flickable sizing issues)
- Add a separate power button in the QML — power is signal-chain-only
- Use a different header pattern than SyPanelBase provides
