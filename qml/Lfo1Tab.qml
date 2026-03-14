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
                hex0: "10"; hex1: root.hex1; hex2: "16"; hex3: "28"
            }

            Grid {
                columns: 8
                columnSpacing: 14
                rowSpacing: 10

                // Row 0: SW, Rate, Dynamic Depth, Pitch Depth, Filter Depth, Amp Depth, PWM Depth
                SySwitch      { hex0: "10"; hex1: root.hex1; hex2: "16"; hex3: "27" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "16"; hex3: "29" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "16"; hex3: "2A" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "16"; hex3: "2B" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "16"; hex3: "2C" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "16"; hex3: "2D" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "16"; hex3: "2E" }

                // Row 1: Fade Time, Sync
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "16"; hex3: "2F" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "16"; hex3: "30" }
            }
        }
    }
}
