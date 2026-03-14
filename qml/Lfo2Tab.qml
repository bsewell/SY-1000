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

            // Shape dropdown
            SyComboBox {
                hex0: "10"; hex1: root.hex1; hex2: "16"; hex3: "32"
            }

            Grid {
                columns: 8
                columnSpacing: 14
                rowSpacing: 10

                // Row 0: SW, Rate, Dynamic Depth, Pitch Depth, Filter Depth, Amp Depth, PWM Depth
                SySwitch      { hex0: "10"; hex1: root.hex1; hex2: "16"; hex3: "31" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "16"; hex3: "33" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "16"; hex3: "34" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "16"; hex3: "35" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "16"; hex3: "36" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "16"; hex3: "37" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "16"; hex3: "38" }

                // Row 1: Fade Time, Sync
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "16"; hex3: "39" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "16"; hex3: "3A" }
            }
        }
    }
}
