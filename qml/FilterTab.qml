import QtQuick

Rectangle {
    id: root
    color: SyTheme.bgPanel
    property string hex1: "00"
    property string hex2: "16"

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

            // TYPE and SLOPE dropdowns side by side
            Row {
                spacing: SyTheme.gridColSpacing
                SyComboBox { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "1D" }
                SyComboBox { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "1E" }
            }

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
