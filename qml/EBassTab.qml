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

            // Bass Type
            SyComboBox { hex0: "10"; hex1: root.hex1; hex2: "1C"; hex3: "00" }

            Grid {
                columns: 8
                columnSpacing: 14
                rowSpacing: 10
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1C"; hex3: "01" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1C"; hex3: "02" }
                SyComboBox    { hex0: "10"; hex1: root.hex1; hex2: "1C"; hex3: "03"; implicitWidth: 100 }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1C"; hex3: "04" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1C"; hex3: "05" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1C"; hex3: "06" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1C"; hex3: "07" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1C"; hex3: "08" }
            }

            Row {
                spacing: 14
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1C"; hex3: "09" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1C"; hex3: "0A" }
            }

            Rectangle { width: 600; height: 1; color: "#333" }

            // Noise Suppressor
            Text { text: "NOISE SUPPRESSOR"; color: "#00ccff"; font.pixelSize: 12; font.family: "Roboto Condensed"; font.bold: true }
            Row {
                spacing: 14
                SySwitch      { hex0: "10"; hex1: root.hex1; hex2: "1C"; hex3: "1F" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1C"; hex3: "20" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1C"; hex3: "21" }
            }
        }
    }
}
