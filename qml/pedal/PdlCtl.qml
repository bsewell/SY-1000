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

            Repeater {
                model: [
                    { label: "CTL 1", src: "27", patchFunc: "1C", patchMode: "1D", sysFunc: "0C", sysMode: "0D" },
                    { label: "CTL 2", src: "28", patchFunc: "1E", patchMode: "1F", sysFunc: "0E", sysMode: "0F" },
                    { label: "CTL 3", src: "29", patchFunc: "20", patchMode: "21", sysFunc: "10", sysMode: "11" },
                    { label: "CTL 4", src: "2A", patchFunc: "22", patchMode: "23", sysFunc: "12", sysMode: "13" },
                    { label: "CTL 5", src: "2B", patchFunc: "24", patchMode: "25", sysFunc: "14", sysMode: "15" },
                    { label: "CTL 6", src: "2C", patchFunc: "26", patchMode: "27", sysFunc: "16", sysMode: "17" }
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
        }
    }
}
