import QtQuick
import ".."

Item {
    anchors.fill: parent
    property string panelHex1: parent ? parent.panelHex1 : "00"
    property string panelSysHex1: parent ? parent.panelSysHex1 : "01"

    Flickable {
        anchors.fill: parent
        contentHeight: col.height + 32
        clip: true

        Column {
            id: col
            width: parent.width
            spacing: 0

            // EXP 1
            Rectangle { width: parent.width; height: 28; color: "#313A47"; Text { x: 16; anchors.verticalCenter: parent.verticalCenter; text: "EXP 1"; color: "#ffffff"; font.pixelSize: 13; font.family: "Roboto Condensed"; font.bold: true } }
            Item {
                width: parent.width; height: 44
                Text { x: 32; anchors.verticalCenter: parent.verticalCenter; text: "SOURCE"; color: "#ccffffff"; font.pixelSize: 11; font.family: "Roboto Condensed" }
                SySwitch { x: 160; y: 6; hex0: "00"; hex1: panelSysHex1; hex2: "10"; hex3: "2E" }
            }
            Item { width: parent.width; height: 36; SyComboBox { x: 16; width: 420; hex0: "10"; hex1: panelHex1; hex2: "00"; hex3: "2A" } }
            Item { width: parent.width; height: 36; SyComboBox { x: 16; width: 420; hex0: "00"; hex1: panelSysHex1; hex2: "10"; hex3: "1A" } }

            Rectangle { width: parent.width - 32; height: 1; color: "#26ffffff"; x: 16 }

            // EXP 2
            Rectangle { width: parent.width; height: 28; color: "#313A47"; Text { x: 16; anchors.verticalCenter: parent.verticalCenter; text: "EXP 2"; color: "#ffffff"; font.pixelSize: 13; font.family: "Roboto Condensed"; font.bold: true } }
            Item {
                width: parent.width; height: 44
                Text { x: 32; anchors.verticalCenter: parent.verticalCenter; text: "SOURCE"; color: "#ccffffff"; font.pixelSize: 11; font.family: "Roboto Condensed" }
                SySwitch { x: 160; y: 6; hex0: "00"; hex1: panelSysHex1; hex2: "10"; hex3: "2F" }
            }
            Item { width: parent.width; height: 36; SyComboBox { x: 16; width: 420; hex0: "10"; hex1: panelHex1; hex2: "00"; hex3: "2B" } }
            Item { width: parent.width; height: 36; SyComboBox { x: 16; width: 420; hex0: "00"; hex1: panelSysHex1; hex2: "10"; hex3: "1B" } }

            Rectangle { width: parent.width - 32; height: 1; color: "#26ffffff"; x: 16 }

            // EXP 1 ASSIGN HOLD
            Rectangle { width: parent.width; height: 28; color: "#313A47"; Text { x: 16; anchors.verticalCenter: parent.verticalCenter; text: "EXP 1 ASSIGN HOLD"; color: "#ffffff"; font.pixelSize: 13; font.family: "Roboto Condensed"; font.bold: true } }
            Item {
                width: parent.width; height: 44
                Text { x: 32; anchors.verticalCenter: parent.verticalCenter; text: "GLOBAL"; color: "#ccffffff"; font.pixelSize: 11; font.family: "Roboto Condensed" }
                SySwitch { x: 160; y: 6; hex0: "00"; hex1: panelSysHex1; hex2: "00"; hex3: "0A" }
            }

            Rectangle { width: parent.width - 32; height: 1; color: "#26ffffff"; x: 16 }

            // EXP 2 ASSIGN HOLD
            Rectangle { width: parent.width; height: 28; color: "#313A47"; Text { x: 16; anchors.verticalCenter: parent.verticalCenter; text: "EXP 2 ASSIGN HOLD"; color: "#ffffff"; font.pixelSize: 13; font.family: "Roboto Condensed"; font.bold: true } }
            Item {
                width: parent.width; height: 44
                Text { x: 32; anchors.verticalCenter: parent.verticalCenter; text: "GLOBAL"; color: "#ccffffff"; font.pixelSize: 11; font.family: "Roboto Condensed" }
                SySwitch { x: 160; y: 6; hex0: "00"; hex1: panelSysHex1; hex2: "00"; hex3: "0B" }
            }
        }
    }
}
