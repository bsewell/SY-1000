import QtQuick
import ".."

Item {
    property string panelHex1: parent ? parent.panelHex1 : "01"
    property string panelHex1b: parent ? parent.panelHex1b : "04"

    Column {
        anchors.fill: parent
        spacing: 0
        topPadding: 16

        // BANK section
        Text {
            x: 16
            text: "BANK"
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
                hex0: "00"; hex1: panelHex1; hex2: "00"; hex3: "05"
            }

            SyComboBox {
                hex0: "00"; hex1: panelHex1; hex2: "00"; hex3: "06"
            }

            SyComboBox {
                hex0: "00"; hex1: panelHex1; hex2: "00"; hex3: "07"
            }
        }

        Item { width: 1; height: 24 }

        Rectangle { width: parent.width - 32; height: 1; color: "#2a2a2a"; x: 16 }

        Item { width: 1; height: 16 }

        Text {
            x: 16
            text: "BANK UP/DOWN SW"
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
                hex0: "00"; hex1: panelHex1; hex2: "00"; hex3: "08"
            }
        }
    }
}
