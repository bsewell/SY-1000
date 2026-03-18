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

            // MEMORY / PATCH MODE global toggle
            Rectangle {
                width: parent.width; height: 28; color: "#313A47"
                Text { x: 16; anchors.verticalCenter: parent.verticalCenter; text: "MEMORY / PATCH MODE"; color: "#ffffff"; font.pixelSize: 13; font.family: "Roboto Condensed"; font.bold: true }
            }
            Item {
                width: parent.width; height: 44
                Text { x: 32; anchors.verticalCenter: parent.verticalCenter; text: "MODE SELECT"; color: "#ccffffff"; font.pixelSize: 11; font.family: "Roboto Condensed" }
                SySwitch { x: 160; y: 6; hex0: "00"; hex1: panelSysHex1; hex2: "10"; hex3: "3F" }
            }

            Rectangle { width: parent.width - 32; height: 1; color: "#26ffffff"; x: 16 }

            Repeater {
                model: [
                    { label: "NUMBER 1",
                      memSrc: "21", memPatchFunc: "10", memPatchMode: "11", memSysFunc: "00", memSysMode: "01",
                      manSrc: "3B", manPatchFunc: "10", manPatchMode: "11", manSysFunc: "33", manSysMode: "34" },
                    { label: "NUMBER 2",
                      memSrc: "22", memPatchFunc: "12", memPatchMode: "13", memSysFunc: "02", memSysMode: "03",
                      manSrc: "3C", manPatchFunc: "10", manPatchMode: "11", manSysFunc: "35", manSysMode: "36" },
                    { label: "NUMBER 3",
                      memSrc: "23", memPatchFunc: "14", memPatchMode: "15", memSysFunc: "04", memSysMode: "05",
                      manSrc: "3D", manPatchFunc: "14", manPatchMode: "15", manSysFunc: "37", manSysMode: "38" },
                    { label: "NUMBER 4",
                      memSrc: "24", memPatchFunc: "16", memPatchMode: "17", memSysFunc: "06", memSysMode: "07",
                      manSrc: "3E", manPatchFunc: "16", manPatchMode: "17", manSysFunc: "39", manSysMode: "3A" }
                ]

                Column {
                    width: col.width
                    spacing: 0

                    Rectangle {
                        width: parent.width; height: 28; color: "#313A47"
                        Text { x: 16; anchors.verticalCenter: parent.verticalCenter; text: modelData.label; color: "#ffffff"; font.pixelSize: 13; font.family: "Roboto Condensed"; font.bold: true }
                    }

                    Text { x: 24; height: 22; verticalAlignment: Text.AlignVCenter; text: "MEMORY MODE"; color: "#88ffffff"; font.pixelSize: 10; font.family: "Roboto Condensed"; font.bold: true }

                    Item {
                        width: parent.width; height: 44
                        Text { x: 32; anchors.verticalCenter: parent.verticalCenter; text: "SOURCE"; color: "#ccffffff"; font.pixelSize: 11; font.family: "Roboto Condensed" }
                        SySwitch { x: 160; y: 6; hex0: "00"; hex1: panelSysHex1; hex2: "10"; hex3: modelData.memSrc }
                    }
                    Item { width: parent.width; height: 36; SyComboBox { x: 16; width: 420; hex0: "10"; hex1: panelHex1; hex2: "00"; hex3: modelData.memPatchFunc } }
                    Item { width: parent.width; height: 36; SyComboBox { x: 16; width: 420; hex0: "10"; hex1: panelHex1; hex2: "00"; hex3: modelData.memPatchMode } }
                    Item { width: parent.width; height: 36; SyComboBox { x: 16; width: 420; hex0: "00"; hex1: panelSysHex1; hex2: "10"; hex3: modelData.memSysFunc } }
                    Item { width: parent.width; height: 36; SyComboBox { x: 16; width: 420; hex0: "00"; hex1: panelSysHex1; hex2: "10"; hex3: modelData.memSysMode } }

                    Rectangle { width: parent.width - 48; height: 1; color: "#1affffff"; x: 24 }

                    Text { x: 24; height: 22; verticalAlignment: Text.AlignVCenter; text: "MANUAL MODE"; color: "#88ffffff"; font.pixelSize: 10; font.family: "Roboto Condensed"; font.bold: true }

                    Item {
                        width: parent.width; height: 44
                        Text { x: 32; anchors.verticalCenter: parent.verticalCenter; text: "SOURCE"; color: "#ccffffff"; font.pixelSize: 11; font.family: "Roboto Condensed" }
                        SySwitch { x: 160; y: 6; hex0: "00"; hex1: panelSysHex1; hex2: "10"; hex3: modelData.manSrc }
                    }
                    Item { width: parent.width; height: 36; SyComboBox { x: 16; width: 420; hex0: "10"; hex1: panelHex1; hex2: "00"; hex3: modelData.manPatchFunc } }
                    Item { width: parent.width; height: 36; SyComboBox { x: 16; width: 420; hex0: "10"; hex1: panelHex1; hex2: "00"; hex3: modelData.manPatchMode } }
                    Item { width: parent.width; height: 36; SyComboBox { x: 16; width: 420; hex0: "00"; hex1: panelSysHex1; hex2: "10"; hex3: modelData.manSysFunc } }
                    Item { width: parent.width; height: 36; SyComboBox { x: 16; width: 420; hex0: "00"; hex1: panelSysHex1; hex2: "10"; hex3: modelData.manSysMode } }

                    Rectangle { width: parent.width - 32; height: 1; color: "#26ffffff"; x: 16 }
                }
            }
        }
    }
}
