import QtQuick

Rectangle {
    id: root
    color: SyTheme.bgPanel
    property string hex1: "00"
    property string hex2: "17"

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

            Row {
                spacing: SyTheme.gridColSpacing

                // Lower Layer dropdown
                SyComboBox {
                    hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "4B"
                }

                // Upper Layer dropdown
                SyComboBox {
                    hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "4C"
                }
            }

            Row {
                spacing: SyTheme.gridColSpacing

                // Lower Fade knob
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "4A" }

                // Upper Fade knob
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "4D" }
            }
        }
    }
}
