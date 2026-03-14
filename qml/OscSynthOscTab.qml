import QtQuick

Rectangle {
    id: root
    color: "#1a1a1a"
    property string hex1: "00"
    property string hex2: "18"

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

            // Wave Category + Wave Type
            Row {
                spacing: 14
                SyComboBox { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "00" }
                SyComboBox { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "01" }
            }

            // OSC 1: Pitch, Fine, PW, PWM Env, Env A/B/C, Level
            Text { text: "OSC 1"; color: "#00ccff"; font.pixelSize: 12; font.family: "Roboto Condensed"; font.bold: true }
            Grid {
                columns: 8
                columnSpacing: 14
                rowSpacing: 10
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "02" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "03" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "04" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "05" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "06" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "07" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "08" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "09" }
            }

            Rectangle { width: 600; height: 1; color: "#333" }

            // Mono/Poly, Hold Mode, Porta, Chromatic, Low Vel Cut
            Row {
                spacing: 14
                SyComboBox { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "13" }
                SyComboBox { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "18" }
                SySwitch   { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "15" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "16" }
                SySwitch   { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "14" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "19" }
            }

            // Porta Mode (only when mono)
            SyComboBox { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "17" }

            // OSC 2: Wave Type, Pitch, Fine, PW, PWM Env, Env A/B/C, Level
            Text { text: "OSC 2"; color: "#00ccff"; font.pixelSize: 12; font.family: "Roboto Condensed"; font.bold: true }
            Row {
                spacing: 14
                SyComboBox { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0A" }
            }
            Grid {
                columns: 8
                columnSpacing: 14
                rowSpacing: 10
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0B" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0C" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0D" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0E" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0F" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "10" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "11" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "12" }
            }
        }
    }
}
