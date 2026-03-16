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

            // BANK DOWN, BANK UP, CURRENT NUMBER — each with source + patch/system combos
            Repeater {
                model: [
                    { label: "BANK DOWN",      src: "26", patchFunc: "18", patchMode: "19", sysFunc: "08", sysMode: "09" },
                    { label: "BANK UP",        src: "27", patchFunc: "1A", patchMode: "1B", sysFunc: "0A", sysMode: "0B" },
                    { label: "CURRENT NUMBER", src: "2D", patchFunc: "28", patchMode: "29", sysFunc: "18", sysMode: "19" }
                ]

                Column {
                    width: col.width
                    spacing: 0

                    Rectangle {
                        width: parent.width; height: 32
                        color: "#313A47"
                        Text {
                            x: 16; anchors.verticalCenter: parent.verticalCenter
                            text: modelData.label
                            color: "#ffffff"; font.pixelSize: 14; font.family: "Roboto Condensed"
                        }
                    }

                    Item {
                        width: parent.width; height: 56; clip: true
                        Text { x: 32; anchors.verticalCenter: parent.verticalCenter; text: "SOURCE"; color: "#ccffffff"; font.pixelSize: 12; font.family: "Roboto Condensed" }
                        SySwitch { x: 160; y: 6; hex0: "00"; hex1: panelSysHex1; hex2: "10"; hex3: modelData.src }
                    }
                    Item {
                        width: parent.width; height: 44
                        Text { x: 32; anchors.verticalCenter: parent.verticalCenter; text: "PATCH FUNCTION"; color: "#ccffffff"; font.pixelSize: 12; font.family: "Roboto Condensed" }
                        SyComboBox { x: 160; y: 8; hex0: "10"; hex1: panelHex1; hex2: "00"; hex3: modelData.patchFunc }
                    }
                    Item {
                        width: parent.width; height: 44
                        Text { x: 32; anchors.verticalCenter: parent.verticalCenter; text: "PATCH MODE"; color: "#ccffffff"; font.pixelSize: 12; font.family: "Roboto Condensed" }
                        SyComboBox { x: 160; y: 8; hex0: "10"; hex1: panelHex1; hex2: "00"; hex3: modelData.patchMode }
                    }
                    Item {
                        width: parent.width; height: 44
                        Text { x: 32; anchors.verticalCenter: parent.verticalCenter; text: "SYSTEM FUNCTION"; color: "#ccffffff"; font.pixelSize: 12; font.family: "Roboto Condensed" }
                        SyComboBox { x: 160; y: 8; hex0: "00"; hex1: panelSysHex1; hex2: "10"; hex3: modelData.sysFunc }
                    }
                    Item {
                        width: parent.width; height: 44
                        Text { x: 32; anchors.verticalCenter: parent.verticalCenter; text: "SYSTEM MODE"; color: "#ccffffff"; font.pixelSize: 12; font.family: "Roboto Condensed" }
                        SyComboBox { x: 160; y: 8; hex0: "00"; hex1: panelSysHex1; hex2: "10"; hex3: modelData.sysMode }
                    }

                    Rectangle { width: parent.width - 32; height: 1; color: "#26ffffff"; x: 16 }
                }
            }

            // BANK UP + BANK DOWN (tuner/off toggle)
            Rectangle {
                width: parent.width; height: 32
                color: "#313A47"
                Text {
                    x: 16; anchors.verticalCenter: parent.verticalCenter
                    text: "BANK UP + BANK DOWN"
                    color: "#ffffff"; font.pixelSize: 14; font.family: "Roboto Condensed"
                }
            }
            Item {
                width: parent.width; height: 56; clip: true
                Text { x: 32; anchors.verticalCenter: parent.verticalCenter; text: "FUNCTION"; color: "#ccffffff"; font.pixelSize: 12; font.family: "Roboto Condensed" }
                SySwitch { x: 160; y: 6; hex0: "00"; hex1: panelSysHex1; hex2: "00"; hex3: "08" }
            }
        }
    }
}
