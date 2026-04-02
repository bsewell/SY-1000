import QtQuick

Rectangle {
    id: root
    color: SyTheme.bgPanel
    property string hex1: "00"
    property string hex2: "1A"

    // Standard dropdown width for stacked combos
    readonly property int comboWidth: 340

    Flickable {
        anchors.fill: parent
        anchors.margins: SyTheme.panelPadding
        contentWidth: col.width
        contentHeight: col.height
        clip: true
        interactive: contentWidth > width || contentHeight > height

        Column {
            id: col
            spacing: 6

            // Stacked dropdowns — all same width, left-aligned
            SyComboBox { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "00"; implicitWidth: root.comboWidth }
            SyComboBox { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "01"; implicitWidth: root.comboWidth }
            SyComboBox { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "02"; implicitWidth: root.comboWidth }

            Item { width: 1; height: 4 }

            // Sense, Depth, Attack, Resonance, Direct Mix
            Grid {
                columns: 8
                columnSpacing: SyTheme.gridColSpacing
                rowSpacing: SyTheme.gridRowSpacing
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "03" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "04" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "05" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "06" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "07" }
            }

            // Volume, Tone
            Grid {
                columns: 2
                columnSpacing: SyTheme.gridColSpacing
                rowSpacing: SyTheme.gridRowSpacing
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "08" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "09" }
            }
        }
    }
}
