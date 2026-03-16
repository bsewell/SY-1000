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

            // GK VOLUME
            Rectangle {
                width: parent.width; height: 32; color: "#313A47"
                Text { x: 16; anchors.verticalCenter: parent.verticalCenter; text: "GK VOLUME"; color: "#ffffff"; font.pixelSize: 14; font.family: "Roboto Condensed" }
            }
            Item {
                width: parent.width; height: 56; clip: true
                Text { x: 32; anchors.verticalCenter: parent.verticalCenter; text: "SOURCE"; color: "#ccffffff"; font.pixelSize: 12; font.family: "Roboto Condensed" }
                SySwitch { x: 240; y: 6; hex0: "00"; hex1: panelSysHex1; hex2: "10"; hex3: "30" }
            }
            Item {
                width: parent.width; height: 44
                Text { x: 32; anchors.verticalCenter: parent.verticalCenter; text: "PATCH FUNCTION"; color: "#ccffffff"; font.pixelSize: 12; font.family: "Roboto Condensed" }
                SyComboBox { x: 240; y: 8; hex0: "10"; hex1: panelHex1; hex2: "00"; hex3: "2C" }
            }
            Item {
                width: parent.width; height: 44
                Text { x: 32; anchors.verticalCenter: parent.verticalCenter; text: "SYSTEM FUNCTION"; color: "#ccffffff"; font.pixelSize: 12; font.family: "Roboto Condensed" }
                SyComboBox { x: 240; y: 8; hex0: "00"; hex1: panelSysHex1; hex2: "10"; hex3: "1C" }
            }

            Rectangle { width: parent.width - 32; height: 1; color: "#26ffffff"; x: 16 }

            // GK VOLUME ASSIGN HOLD
            Rectangle {
                width: parent.width; height: 32; color: "#313A47"
                Text { x: 16; anchors.verticalCenter: parent.verticalCenter; text: "GK VOLUME ASSIGN HOLD"; color: "#ffffff"; font.pixelSize: 14; font.family: "Roboto Condensed" }
            }
            Item {
                width: parent.width; height: 56; clip: true
                Text { x: 32; anchors.verticalCenter: parent.verticalCenter; text: "GLOBAL"; color: "#ccffffff"; font.pixelSize: 12; font.family: "Roboto Condensed" }
                SySwitch { x: 240; y: 6; hex0: "00"; hex1: panelSysHex1; hex2: "00"; hex3: "09" }
            }

            Rectangle { width: parent.width - 32; height: 1; color: "#26ffffff"; x: 16 }

            // GK SWITCH 1
            Rectangle {
                width: parent.width; height: 32; color: "#313A47"
                Text { x: 16; anchors.verticalCenter: parent.verticalCenter; text: "GK SWITCH 1"; color: "#ffffff"; font.pixelSize: 14; font.family: "Roboto Condensed" }
            }
            Item {
                width: parent.width; height: 56; clip: true
                Text { x: 32; anchors.verticalCenter: parent.verticalCenter; text: "SOURCE"; color: "#ccffffff"; font.pixelSize: 12; font.family: "Roboto Condensed" }
                SySwitch { x: 240; y: 6; hex0: "00"; hex1: panelSysHex1; hex2: "10"; hex3: "31" }
            }
            Item {
                width: parent.width; height: 44
                Text { x: 32; anchors.verticalCenter: parent.verticalCenter; text: "PATCH FUNCTION"; color: "#ccffffff"; font.pixelSize: 12; font.family: "Roboto Condensed" }
                SyComboBox { x: 240; y: 8; hex0: "10"; hex1: panelHex1; hex2: "00"; hex3: "2D" }
            }
            Item {
                width: parent.width; height: 44
                Text { x: 32; anchors.verticalCenter: parent.verticalCenter; text: "PATCH MODE"; color: "#ccffffff"; font.pixelSize: 12; font.family: "Roboto Condensed" }
                SyComboBox { x: 240; y: 8; hex0: "10"; hex1: panelHex1; hex2: "00"; hex3: "2E" }
            }
            Item {
                width: parent.width; height: 44
                Text { x: 32; anchors.verticalCenter: parent.verticalCenter; text: "SYSTEM FUNCTION"; color: "#ccffffff"; font.pixelSize: 12; font.family: "Roboto Condensed" }
                SyComboBox { x: 240; y: 8; hex0: "00"; hex1: panelSysHex1; hex2: "10"; hex3: "1D" }
            }
            Item {
                width: parent.width; height: 44
                Text { x: 32; anchors.verticalCenter: parent.verticalCenter; text: "SYSTEM MODE"; color: "#ccffffff"; font.pixelSize: 12; font.family: "Roboto Condensed" }
                SyComboBox { x: 240; y: 8; hex0: "00"; hex1: panelSysHex1; hex2: "10"; hex3: "1E" }
            }

            Rectangle { width: parent.width - 32; height: 1; color: "#26ffffff"; x: 16 }

            // GK SWITCH 2
            Rectangle {
                width: parent.width; height: 32; color: "#313A47"
                Text { x: 16; anchors.verticalCenter: parent.verticalCenter; text: "GK SWITCH 2"; color: "#ffffff"; font.pixelSize: 14; font.family: "Roboto Condensed" }
            }
            Item {
                width: parent.width; height: 56; clip: true
                Text { x: 32; anchors.verticalCenter: parent.verticalCenter; text: "SOURCE"; color: "#ccffffff"; font.pixelSize: 12; font.family: "Roboto Condensed" }
                SySwitch { x: 240; y: 6; hex0: "00"; hex1: panelSysHex1; hex2: "10"; hex3: "32" }
            }
            Item {
                width: parent.width; height: 44
                Text { x: 32; anchors.verticalCenter: parent.verticalCenter; text: "PATCH FUNCTION"; color: "#ccffffff"; font.pixelSize: 12; font.family: "Roboto Condensed" }
                SyComboBox { x: 240; y: 8; hex0: "10"; hex1: panelHex1; hex2: "00"; hex3: "2F" }
            }
            Item {
                width: parent.width; height: 44
                Text { x: 32; anchors.verticalCenter: parent.verticalCenter; text: "PATCH MODE"; color: "#ccffffff"; font.pixelSize: 12; font.family: "Roboto Condensed" }
                SyComboBox { x: 240; y: 8; hex0: "10"; hex1: panelHex1; hex2: "00"; hex3: "30" }
            }
            Item {
                width: parent.width; height: 44
                Text { x: 32; anchors.verticalCenter: parent.verticalCenter; text: "SYSTEM FUNCTION"; color: "#ccffffff"; font.pixelSize: 12; font.family: "Roboto Condensed" }
                SyComboBox { x: 240; y: 8; hex0: "00"; hex1: panelSysHex1; hex2: "10"; hex3: "1F" }
            }
            Item {
                width: parent.width; height: 44
                Text { x: 32; anchors.verticalCenter: parent.verticalCenter; text: "SYSTEM MODE"; color: "#ccffffff"; font.pixelSize: 12; font.family: "Roboto Condensed" }
                SyComboBox { x: 240; y: 8; hex0: "00"; hex1: panelSysHex1; hex2: "10"; hex3: "20" }
            }
        }
    }
}
