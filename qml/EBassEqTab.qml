import QtQuick

Rectangle {
    id: root
    color: "#1a1a1a"
    property string hex1: "00"

    Flickable {
        anchors.fill: parent
        anchors.margins: 12
        contentWidth: grid.width
        contentHeight: grid.height
        clip: true
        interactive: contentWidth > width || contentHeight > height

        Column {
            id: grid
            spacing: 10

            Text { text: "EQUALIZER"; color: "#00ccff"; font.pixelSize: 12; font.family: "Roboto Condensed"; font.bold: true }

            SySwitch { hex0: "10"; hex1: root.hex1; hex2: "1C"; hex3: "22" }

            Grid {
                columns: 4
                columnSpacing: 14
                rowSpacing: 10
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1C"; hex3: "23" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1C"; hex3: "24" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1C"; hex3: "25" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1C"; hex3: "26" }
            }
        }
    }
}
