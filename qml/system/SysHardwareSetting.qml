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

            // ASSIGN HOLD section
            Rectangle {
                width: parent.width; height: 32
                color: "#313A47"
                Text {
                    x: 16; anchors.verticalCenter: parent.verticalCenter
                    text: "ASSIGN HOLD"
                    color: "#ffffff"; font.pixelSize: 14
                    font.family: "Roboto Condensed"
                }
            }

            // GK VOL HOLD
            Item {
                width: parent.width; height: 50; clip: true
                Text {
                    x: 32; anchors.verticalCenter: parent.verticalCenter
                    text: "GK VOL HOLD"
                    color: "#ccffffff"; font.pixelSize: 12
                    font.family: "Roboto Condensed"
                }
                SySwitch {
                    x: 160; y: 4
                    hex0: "00"; hex1: panelHex1; hex2: "00"; hex3: "09"
                }
            }

            // EXP1 HOLD
            Item {
                width: parent.width; height: 50; clip: true
                Text {
                    x: 32; anchors.verticalCenter: parent.verticalCenter
                    text: "EXP1 HOLD"
                    color: "#ccffffff"; font.pixelSize: 12
                    font.family: "Roboto Condensed"
                }
                SySwitch {
                    x: 160; y: 4
                    hex0: "00"; hex1: panelHex1; hex2: "00"; hex3: "0A"
                }
            }

            // EXP2 HOLD
            Item {
                width: parent.width; height: 50; clip: true
                Text {
                    x: 32; anchors.verticalCenter: parent.verticalCenter
                    text: "EXP2 HOLD"
                    color: "#ccffffff"; font.pixelSize: 12
                    font.family: "Roboto Condensed"
                }
                SySwitch {
                    x: 160; y: 4
                    hex0: "00"; hex1: panelHex1; hex2: "00"; hex3: "0B"
                }
            }

            Rectangle { width: parent.width - 32; height: 1; color: "#26ffffff"; x: 16 }

            // GROUND LIFT section
            Rectangle {
                width: parent.width; height: 32
                color: "#313A47"
                Text {
                    x: 16; anchors.verticalCenter: parent.verticalCenter
                    text: "GROUND LIFT"
                    color: "#ffffff"; font.pixelSize: 14
                    font.family: "Roboto Condensed"
                }
            }

            Item {
                width: parent.width; height: 50; clip: true
                Text {
                    x: 32; anchors.verticalCenter: parent.verticalCenter
                    text: "GROUND LIFT"
                    color: "#ccffffff"; font.pixelSize: 12
                    font.family: "Roboto Condensed"
                }
                SySwitch {
                    x: 160; y: 4
                    hex0: "00"; hex1: panelHex1b; hex2: "10"; hex3: "00"
                }
            }

            Rectangle { width: parent.width - 32; height: 1; color: "#26ffffff"; x: 16 }

            // OUTPUT LEVEL section
            Rectangle {
                width: parent.width; height: 32
                color: "#313A47"
                Text {
                    x: 16; anchors.verticalCenter: parent.verticalCenter
                    text: "OUTPUT LEVEL"
                    color: "#ffffff"; font.pixelSize: 14
                    font.family: "Roboto Condensed"
                }
            }

            Item {
                width: parent.width; height: 44
                Text {
                    x: 32; anchors.verticalCenter: parent.verticalCenter
                    text: "OUTPUT LEVEL"
                    color: "#ccffffff"; font.pixelSize: 12
                    font.family: "Roboto Condensed"
                }
                SyComboBox {
                    x: 160; y: 8
                    hex0: "00"; hex1: panelHex1; hex2: "00"; hex3: "0D"
                }
            }
        }
    }
}
