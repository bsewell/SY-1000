import QtQuick
import ".."

Item {
    anchors.fill: parent
    property string panelHex1: parent ? parent.panelHex1 : "01"
    property string panelHex1b: parent ? parent.panelHex1b : "04"

    Column {
        width: parent.width
        spacing: 0

        Rectangle {
            width: parent.width; height: 32
            color: "#313A47"
            Text {
                x: 16; anchors.verticalCenter: parent.verticalCenter
                text: "NORMAL SET SELECT"
                color: "#ffffff"; font.pixelSize: 14
                font.family: "Roboto Condensed"
            }
        }

        Item {
            width: parent.width; height: 44
            Text {
                x: 32; anchors.verticalCenter: parent.verticalCenter
                text: "NORMAL SET SELECT"
                color: "#ccffffff"; font.pixelSize: 12
                font.family: "Roboto Condensed"
            }
            SyComboBox {
                x: 240; y: 8
                hex0: "00"; hex1: panelHex1; hex2: "00"; hex3: "27"
            }
        }
    }
}
