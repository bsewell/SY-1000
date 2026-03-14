import QtQuick

Rectangle {
    id: root
    color: "#1a1a1a"
    property string hex1: "00"
    property string hex2: "1A"

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

            // Amp SW
            SySwitch { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0A" }

            // PreAmp
            Text { text: "PREAMP"; color: "#00ccff"; font.pixelSize: 12; font.family: "Roboto Condensed"; font.bold: true }
            Row {
                spacing: 14
                SyComboBox { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0B" }
                SyComboBox { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "15"; implicitWidth: 100 }
            }
            Grid {
                columns: 8
                columnSpacing: 14
                rowSpacing: 10
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0C" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0D" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0E" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "10" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "11" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "12" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "13" }
                SySwitch      { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "14" }
            }
            FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0F" }

            Rectangle { width: 600; height: 1; color: "#333" }

            // Speaker
            Text { text: "SPEAKER"; color: "#00ccff"; font.pixelSize: 12; font.family: "Roboto Condensed"; font.bold: true }
            Row {
                spacing: 14
                SyComboBox { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "18" }
                SyComboBox { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "19"; implicitWidth: 100 }
                SyComboBox { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "1A"; implicitWidth: 100 }
            }
            Grid {
                columns: 3
                columnSpacing: 14
                rowSpacing: 10
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "1B" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "1C" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "1D" }
            }

            Rectangle { width: 600; height: 1; color: "#333" }

            // Solo
            Text { text: "SOLO"; color: "#00ccff"; font.pixelSize: 12; font.family: "Roboto Condensed"; font.bold: true }
            Row {
                spacing: 14
                SySwitch      { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "16" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "17" }
            }
        }
    }
}
