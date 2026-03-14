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

            // Filter Section
            Text { text: "FILTER"; color: "#00ccff"; font.pixelSize: 12; font.family: "Roboto Condensed"; font.bold: true }
            Row {
                spacing: 14
                SySwitch   { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "1A" }
                SyComboBox { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "1B" }
            }
            Grid {
                columns: 8
                columnSpacing: 14
                rowSpacing: 10
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "1C" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "1D" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "1E" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "1F" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "20" }
            }

            Rectangle { width: 600; height: 1; color: "#333" }

            // Filter Envelope: A, D, S, R, Depth
            Text { text: "FILTER ENV"; color: "#888"; font.pixelSize: 10; font.family: "Roboto Condensed" }
            Grid {
                columns: 5
                columnSpacing: 14
                rowSpacing: 10
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "21" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "22" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "23" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "24" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "25" }
            }

            Rectangle { width: 600; height: 1; color: "#333" }

            // Amp Envelope: A, D, S, R, Vel Sense
            Text { text: "AMP ENV"; color: "#888"; font.pixelSize: 10; font.family: "Roboto Condensed" }
            Grid {
                columns: 7
                columnSpacing: 14
                rowSpacing: 10
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "27" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "28" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "29" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "2A" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "26" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "2B" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "2C" }
            }
        }
    }
}
