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

            // LFO 1
            Text { text: "LFO 1"; color: "#00ccff"; font.pixelSize: 12; font.family: "Roboto Condensed"; font.bold: true }
            Row {
                spacing: 14
                SySwitch   { hex0: "10"; hex1: root.hex1; hex2: "18"; hex3: "2D" }
                SyComboBox { hex0: "10"; hex1: root.hex1; hex2: "18"; hex3: "2E" }
            }
            Grid {
                columns: 8
                columnSpacing: 14
                rowSpacing: 10
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "18"; hex3: "2F" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "18"; hex3: "30" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "18"; hex3: "31" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "18"; hex3: "32" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "18"; hex3: "33" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "18"; hex3: "34" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "18"; hex3: "35" }
                SyComboBox { hex0: "10"; hex1: root.hex1; hex2: "18"; hex3: "36"; implicitWidth: 100 }
            }

            Rectangle { width: 600; height: 1; color: "#333" }

            // LFO 2
            Text { text: "LFO 2"; color: "#00ccff"; font.pixelSize: 12; font.family: "Roboto Condensed"; font.bold: true }
            Row {
                spacing: 14
                SySwitch   { hex0: "10"; hex1: root.hex1; hex2: "18"; hex3: "37" }
                SyComboBox { hex0: "10"; hex1: root.hex1; hex2: "18"; hex3: "38" }
            }
            Grid {
                columns: 8
                columnSpacing: 14
                rowSpacing: 10
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "18"; hex3: "39" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "18"; hex3: "3A" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "18"; hex3: "3B" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "18"; hex3: "3C" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "18"; hex3: "3D" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "18"; hex3: "3E" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "18"; hex3: "3F" }
                SyComboBox { hex0: "10"; hex1: root.hex1; hex2: "18"; hex3: "40"; implicitWidth: 100 }
            }
        }
    }
}
