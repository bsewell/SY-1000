import QtQuick
import ".."

Item {
    anchors.fill: parent
    property string panelHex1: parent ? parent.panelHex1 : "01"
    property string panelHex1b: parent ? parent.panelHex1b : "04"

    Flickable {
        anchors.fill: parent
        contentHeight: col.height + 32
        clip: true

        Column {
            id: col
            width: parent.width
            spacing: 0

            // BANK section header
            Rectangle {
                width: parent.width; height: 32
                color: "#313A47"
                Text {
                    x: 16; anchors.verticalCenter: parent.verticalCenter
                    text: "BANK"
                    color: "#ffffff"; font.pixelSize: 14
                    font.family: "Roboto Condensed"
                }
            }

            // BANK CHANGE MODE
            Item {
                width: parent.width; height: 50; clip: true
                Text {
                    x: 32; anchors.verticalCenter: parent.verticalCenter
                    text: "BANK CHANGE MODE"
                    color: "#ccffffff"; font.pixelSize: 12
                    font.family: "Roboto Condensed"
                }
                SySwitch {
                    x: 240; y: 4
                    hex0: "00"; hex1: panelHex1; hex2: "00"; hex3: "05"
                }
            }

            // BANK EXTENT MIN
            Item {
                width: parent.width; height: 44
                Text {
                    x: 32; anchors.verticalCenter: parent.verticalCenter
                    text: "BANK EXTENT MIN"
                    color: "#ccffffff"; font.pixelSize: 12
                    font.family: "Roboto Condensed"
                }
                SyComboBox {
                    x: 240; y: 8
                    hex0: "00"; hex1: panelHex1; hex2: "00"; hex3: "06"
                }
            }

            // BANK EXTENT MAX
            Item {
                width: parent.width; height: 44
                Text {
                    x: 32; anchors.verticalCenter: parent.verticalCenter
                    text: "BANK EXTENT MAX"
                    color: "#ccffffff"; font.pixelSize: 12
                    font.family: "Roboto Condensed"
                }
                SyComboBox {
                    x: 240; y: 8
                    hex0: "00"; hex1: panelHex1; hex2: "00"; hex3: "07"
                }
            }

            Rectangle { width: parent.width - 32; height: 1; color: "#26ffffff"; x: 16 }

            // BANK UP/DOWN SW header
            Rectangle {
                width: parent.width; height: 32
                color: "#313A47"
                Text {
                    x: 16; anchors.verticalCenter: parent.verticalCenter
                    text: "BANK UP/DOWN SW"
                    color: "#ffffff"; font.pixelSize: 14
                    font.family: "Roboto Condensed"
                }
            }

            Item {
                width: parent.width; height: 50; clip: true
                Text {
                    x: 32; anchors.verticalCenter: parent.verticalCenter
                    text: "BANK UP/DOWN SW"
                    color: "#ccffffff"; font.pixelSize: 12
                    font.family: "Roboto Condensed"
                }
                SySwitch {
                    x: 240; y: 4
                    hex0: "00"; hex1: panelHex1; hex2: "00"; hex3: "08"
                }
            }
        }
    }
}
