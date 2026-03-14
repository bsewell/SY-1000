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

            // Wave Type dropdown
            SyComboBox {
                hex0: "10"; hex1: root.hex1; hex2: "16"; hex3: "04"
            }

            Grid {
                columns: 8
                columnSpacing: 14
                rowSpacing: 10

                // Row 0: Pitch, Fine, Detune, Sharpness, Feedback, Harmonics, S-Saw Detune, Pulse Width
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "16"; hex3: "05" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "16"; hex3: "06" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "16"; hex3: "0A" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "16"; hex3: "0B" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "16"; hex3: "0C" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "16"; hex3: "0D" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "16"; hex3: "0E" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "16"; hex3: "07" }

                // Row 1: PWM Env Attack, PWM Env Depth, P Env Attack, P Env Depth, P Bend Depth, P Bend Ctl
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "16"; hex3: "08" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "16"; hex3: "09" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "16"; hex3: "0F" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "16"; hex3: "10" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "16"; hex3: "11" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "16"; hex3: "12" }
                SySwitch { hex0: "10"; hex1: root.hex1; hex2: "16"; hex3: "13" }
                SyComboBox { hex0: "10"; hex1: root.hex1; hex2: "16"; hex3: "17"; implicitWidth: 100 }
            }
        }
    }
}
