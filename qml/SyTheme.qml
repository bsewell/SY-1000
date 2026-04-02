pragma Singleton
import QtQuick

QtObject {
    // ── Background colors ──
    // Boss TONE STUDIO exact values (bg-base, bg-surface, bg-elevated, bg-subheader)
    readonly property color bgPanel:       "#1A1C1F"
    readonly property color bgControl:     "#292929"
    readonly property color bgElevated:    "#252B32"
    readonly property color bgSectionHead: "#313A47"

    // ── Accent / brand colors ──
    // Boss accent-primary: #33D6FF
    readonly property color accent:        "#33D6FF"

    // ── Text colors ──
    // Boss uses opacity-based white: 100%=#FFF, 80%=#ccffffff, 64%=#a3ffffff, 32%=#52ffffff
    readonly property color textPrimary:   "#ffffff"
    readonly property color textSecondary: "#ccffffff"   // 80% white
    readonly property color textLabel:     "#a3ffffff"   // 64% white
    readonly property color textDimmed:    "#52ffffff"   // 32% white
    readonly property color textSection:   "#52ffffff"   // 32% white (same as dimmed)
    readonly property color textSysLabel:  "#ccffffff"   // system-page label (80% white)

    // ── Line / divider colors ──
    // Boss border-hairline: rgba(255,255,255,0.32)  border-prominent: rgba(255,255,255,0.64)
    readonly property color divider:       "#52ffffff"   // hairline (32% white)
    readonly property color dividerLight:  "#26ffffff"   // subtle (15% white)
    readonly property color border:        "#52ffffff"   // hairline (32% white)
    readonly property color borderProminent: "#a3ffffff" // prominent (64% white)

    // ── Spacing ──
    readonly property int panelPadding:    12
    readonly property int sectionPadding:  16
    readonly property int flowSpacing:     16
    readonly property int flowSpacingSm:   12
    readonly property int gridColSpacing:  14
    readonly property int gridRowSpacing:  10

    // ── Sizing ──
    readonly property int headerHeight:    40
    readonly property int modeSelectorH:   32
    readonly property int settingRowH:     44
    readonly property int settingRowTallH: 50
    readonly property int panelWidth:      800
    readonly property int panelHeight:     480

    // ── Positioning (system pages) ──
    readonly property int sysLabelX:       32
    readonly property int sysControlX:     160
    readonly property int sysSectionLabelX: 16

    // ── Font defaults ──
    readonly property string fontFamily:   "Roboto Condensed"
    readonly property int fontTitle:       16
    readonly property int fontSectionHead: 14
    readonly property int fontBody:        12
    readonly property int fontLabel:       11
    readonly property int fontSmall:       10
    readonly property int fontTiny:        9

    // ── Knob sizes ──
    readonly property int knobLarge:       56
    readonly property int knobSmall:       48
    readonly property string knobLargeSrc: "knobs/knob_56.png"
    readonly property string knobSmallSrc: "knobs/knob_48.png"
}
