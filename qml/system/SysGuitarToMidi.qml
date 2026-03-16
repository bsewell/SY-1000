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
                text: "GUITAR TO MIDI"
                color: "#ffffff"; font.pixelSize: 14
                font.family: "Roboto Condensed"
            }
        }

        // On/Off switch
        Item {
            width: parent.width; height: 56; clip: true
            Text {
                x: 32; anchors.verticalCenter: parent.verticalCenter
                text: "GUITAR TO MIDI"
                color: "#ccffffff"; font.pixelSize: 12
                font.family: "Roboto Condensed"
            }
            SySwitch {
                x: 160; y: 6
                hex0: "00"; hex1: panelHex1b; hex2: "21"; hex3: "00"
            }
        }

        Rectangle { width: parent.width - 32; height: 1; color: "#26ffffff"; x: 16 }

        Repeater {
            model: [
                { label: "PARAMETER 1", hex3: "01" },
                { label: "PARAMETER 2", hex3: "02" },
                { label: "PARAMETER 3", hex3: "03" },
                { label: "PARAMETER 4", hex3: "04" },
                { label: "PARAMETER 5", hex3: "05" }
            ]

            Item {
                width: parent.width; height: 44
                Text {
                    x: 32; anchors.verticalCenter: parent.verticalCenter
                    text: modelData.label
                    color: "#ccffffff"; font.pixelSize: 12
                    font.family: "Roboto Condensed"
                }
                SyComboBox {
                    x: 160; y: 8
                    hex0: "00"; hex1: panelHex1b; hex2: "21"; hex3: modelData.hex3
                }
            }
        }
    }
}
