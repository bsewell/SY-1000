import QtQuick
import ".."

Item {
    property string panelHex1: parent ? parent.panelHex1 : "01"
    property string panelHex1b: parent ? parent.panelHex1b : "04"

    Column {
        anchors.fill: parent
        spacing: 0
        topPadding: 16

        // ASSIGN HOLD section
        Text {
            x: 16
            text: "ASSIGN HOLD"
            color: "#888888"
            font.pixelSize: 11
            font.family: "Roboto Condensed"
            font.capitalization: Font.AllUppercase
        }

        Item { width: 1; height: 12 }

        Row {
            x: 24
            spacing: 16

            SySwitch {
                hex0: "00"; hex1: panelHex1; hex2: "00"; hex3: "09"
            }

            SySwitch {
                hex0: "00"; hex1: panelHex1; hex2: "00"; hex3: "0A"
            }

            SySwitch {
                hex0: "00"; hex1: panelHex1; hex2: "00"; hex3: "0B"
            }
        }

        Item { width: 1; height: 24 }
        Rectangle { width: parent.width - 32; height: 1; color: "#2a2a2a"; x: 16 }
        Item { width: 1; height: 16 }

        // GROUND LIFT section
        Text {
            x: 16
            text: "GROUND LIFT"
            color: "#888888"
            font.pixelSize: 11
            font.family: "Roboto Condensed"
            font.capitalization: Font.AllUppercase
        }

        Item { width: 1; height: 12 }

        Row {
            x: 24
            spacing: 16

            SySwitch {
                hex0: "00"; hex1: panelHex1b; hex2: "10"; hex3: "00"
            }
        }

        Item { width: 1; height: 24 }
        Rectangle { width: parent.width - 32; height: 1; color: "#2a2a2a"; x: 16 }
        Item { width: 1; height: 16 }

        // OUTPUT LEVEL section
        Text {
            x: 16
            text: "OUTPUT LEVEL"
            color: "#888888"
            font.pixelSize: 11
            font.family: "Roboto Condensed"
            font.capitalization: Font.AllUppercase
        }

        Item { width: 1; height: 12 }

        Row {
            x: 24
            spacing: 16

            SyComboBox {
                hex0: "00"; hex1: panelHex1; hex2: "00"; hex3: "0D"
            }
        }
    }
}
