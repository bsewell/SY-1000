import QtQuick

Rectangle {
    id: root
    color: SyTheme.bgPanel

    property string hex1: "00"
    property string hex2: "16"

    Flickable {
        anchors.fill: parent
        anchors.margins: SyTheme.panelPadding
        contentWidth: grid.width
        contentHeight: grid.height
        clip: true
        interactive: contentWidth > width || contentHeight > height

        Grid {
            id: grid
            columns: 3
            columnSpacing: SyTheme.gridColSpacing
            rowSpacing: SyTheme.gridRowSpacing

            // Amp Env Attack, Low Cut, High Cut
            FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "23" }
            FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "25" }
            FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "26" }
        }
    }
}
