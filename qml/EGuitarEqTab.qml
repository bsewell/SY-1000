import QtQuick

Rectangle {
    id: root
    color: SyTheme.bgPanel
    property string hex1: "00"
    property string hex2: "1A"

    Flickable {
        anchors.fill: parent
        anchors.margins: SyTheme.panelPadding
        contentWidth: grid.width
        contentHeight: grid.height
        clip: true
        interactive: contentWidth > width || contentHeight > height

        Column {
            id: grid
            spacing: SyTheme.gridRowSpacing

            Text { text: "EQUALIZER"; color: SyTheme.accent; font.pixelSize: 12; font.family: "Roboto Condensed"; font.bold: true }

            SySwitch { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "21" }

            // ParaEQ bands start at 22
            Grid {
                columns: 4
                columnSpacing: SyTheme.gridColSpacing
                rowSpacing: SyTheme.gridRowSpacing
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "22" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "23" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "24" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "25" }
            }
        }
    }
}
