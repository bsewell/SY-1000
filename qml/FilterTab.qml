import QtQuick

Rectangle {
    id: root
    color: SyTheme.bgPanel
    property string hex1: "00"
    property string hex2: "16"

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

            // Stacked dropdowns — same width, left-aligned
            SyComboBox { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "1D"; implicitWidth: root.comboWidth }
            SyComboBox { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "1E"; implicitWidth: root.comboWidth }

            Item { width: 1; height: 4 }

            Grid {
                columns: 5
                columnSpacing: SyTheme.gridColSpacing
                rowSpacing: SyTheme.gridRowSpacing

                // SW, Cutoff, Resonance, Flt Env Attack, Flt Env Depth
                SySwitch     { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "1C" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "1F" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "20" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "21" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "22" }
            }
        }
    }
}
