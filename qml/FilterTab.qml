import QtQuick

Rectangle {
    id: root
    color: "#1a1a1a"
    property string hex1: "00"

    Flickable {
        anchors.fill: parent
        anchors.margins: 12
        contentWidth: col.width
        contentHeight: col.height
        clip: true
        interactive: contentWidth > width || contentHeight > height

        Column {
            id: col
            spacing: 10

            Row {
                spacing: 14

                // Filter Type dropdown
                SyComboBox {
                    hex0: "10"; hex1: root.hex1; hex2: "16"; hex3: "1D"
                }

                // Slope dropdown
                SyComboBox {
                    hex0: "10"; hex1: root.hex1; hex2: "16"; hex3: "1E"
                }
            }

            Grid {
                columns: 5
                columnSpacing: 14
                rowSpacing: 10

                // SW, Cutoff, Resonance, Flt Env Attack, Flt Env Depth
                SySwitch     { hex0: "10"; hex1: root.hex1; hex2: "16"; hex3: "1C" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "16"; hex3: "1F" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "16"; hex3: "20" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "16"; hex3: "21" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "16"; hex3: "22" }
            }
        }
    }
}
