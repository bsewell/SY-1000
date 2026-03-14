import QtQuick

Rectangle {
    id: root
    color: "#1a1a1a"
    property string hex1: "00"
    property string hex2: "17"

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

            Row {
                spacing: 14

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
                spacing: 14

                // Lower Fade knob
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "4A" }

                // Upper Fade knob
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "4D" }
            }
        }
    }
}
