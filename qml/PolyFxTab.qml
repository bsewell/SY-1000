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

            // Type
            SyComboBox { hex0: "10"; hex1: root.hex1; hex2: "1E"; hex3: "00" }

            Grid {
                columns: 5
                columnSpacing: 14
                rowSpacing: 10
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1E"; hex3: "01" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1E"; hex3: "02" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1E"; hex3: "03" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1E"; hex3: "04" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1E"; hex3: "05" }
            }

            Rectangle { width: 600; height: 1; color: "#333" }

            // Filter
            Text { text: "FILTER"; color: "#00ccff"; font.pixelSize: 12; font.family: "Roboto Condensed"; font.bold: true }
            Row {
                spacing: 14
                SyComboBox { hex0: "10"; hex1: root.hex1; hex2: "1E"; hex3: "06" }
                SyComboBox { hex0: "10"; hex1: root.hex1; hex2: "1E"; hex3: "07"; implicitWidth: 100 }
            }
            Grid {
                columns: 5
                columnSpacing: 14
                rowSpacing: 10
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1E"; hex3: "08" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1E"; hex3: "09" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1E"; hex3: "0A" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1E"; hex3: "0B" }
                SyComboBox    { hex0: "10"; hex1: root.hex1; hex2: "1E"; hex3: "0C"; implicitWidth: 100 }
            }

            Rectangle { width: 600; height: 1; color: "#333" }

            // Compressor
            Text { text: "COMPRESSOR"; color: "#00ccff"; font.pixelSize: 12; font.family: "Roboto Condensed"; font.bold: true }
            Row {
                spacing: 14
                SySwitch      { hex0: "10"; hex1: root.hex1; hex2: "1E"; hex3: "0D" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1E"; hex3: "0E" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1E"; hex3: "0F" }
            }

            Rectangle { width: 600; height: 1; color: "#333" }

            // Noise Suppressor
            Text { text: "NOISE SUPPRESSOR"; color: "#00ccff"; font.pixelSize: 12; font.family: "Roboto Condensed"; font.bold: true }
            Row {
                spacing: 14
                SySwitch      { hex0: "10"; hex1: root.hex1; hex2: "1E"; hex3: "10" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1E"; hex3: "11" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1E"; hex3: "12" }
            }
        }
    }
}
