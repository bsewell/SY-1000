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

            SySwitch { hex0: "10"; hex1: root.hex1; hex2: "1C"; hex3: "0B" }

            Text { text: "PREAMP"; color: "#00ccff"; font.pixelSize: 12; font.family: "Roboto Condensed"; font.bold: true }
            Row {
                spacing: 14
                SyComboBox { hex0: "10"; hex1: root.hex1; hex2: "1C"; hex3: "0C" }
                SyComboBox { hex0: "10"; hex1: root.hex1; hex2: "1C"; hex3: "16"; implicitWidth: 100 }
            }
            Grid {
                columns: 8
                columnSpacing: 14
                rowSpacing: 10
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1C"; hex3: "0D" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1C"; hex3: "0E" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1C"; hex3: "0F" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1C"; hex3: "11" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1C"; hex3: "12" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1C"; hex3: "13" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1C"; hex3: "14" }
                SySwitch      { hex0: "10"; hex1: root.hex1; hex2: "1C"; hex3: "15" }
            }
            FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1C"; hex3: "10" }

            Rectangle { width: 600; height: 1; color: "#333" }

            Text { text: "SPEAKER"; color: "#00ccff"; font.pixelSize: 12; font.family: "Roboto Condensed"; font.bold: true }
            Row {
                spacing: 14
                SyComboBox { hex0: "10"; hex1: root.hex1; hex2: "1C"; hex3: "19" }
                SyComboBox { hex0: "10"; hex1: root.hex1; hex2: "1C"; hex3: "1A"; implicitWidth: 100 }
                SyComboBox { hex0: "10"; hex1: root.hex1; hex2: "1C"; hex3: "1B"; implicitWidth: 100 }
            }
            Grid {
                columns: 3
                columnSpacing: 14
                rowSpacing: 10
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1C"; hex3: "1C" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1C"; hex3: "1D" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1C"; hex3: "1E" }
            }

            Rectangle { width: 600; height: 1; color: "#333" }

            Text { text: "SOLO"; color: "#00ccff"; font.pixelSize: 12; font.family: "Roboto Condensed"; font.bold: true }
            Row {
                spacing: 14
                SySwitch      { hex0: "10"; hex1: root.hex1; hex2: "1C"; hex3: "17" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1C"; hex3: "18" }
            }
        }
    }
}
