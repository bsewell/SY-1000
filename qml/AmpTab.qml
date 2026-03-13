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

        Grid {
            id: grid
            columns: 3
            columnSpacing: 14
            rowSpacing: 10

            // Amp Env Attack, Low Cut, High Cut
            FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "16"; hex3: "23" }
            FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "16"; hex3: "25" }
            FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "16"; hex3: "26" }
        }
    }
}
