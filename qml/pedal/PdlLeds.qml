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
                    { label: "NUMBER 1 LED",      prefHex3: "12", patchOn: "01", patchOff: "00", sysOn: "01", sysOff: "00" },
                    { label: "NUMBER 2 LED",      prefHex3: "13", patchOn: "03", patchOff: "02", sysOn: "03", sysOff: "02" },
                    { label: "NUMBER 3 LED",      prefHex3: "14", patchOn: "05", patchOff: "04", sysOn: "05", sysOff: "04" },
                    { label: "NUMBER 4 LED",      prefHex3: "15", patchOn: "07", patchOff: "06", sysOn: "07", sysOff: "06" },
                    { label: "BANK DOWN LED",     prefHex3: "16", patchOn: "09", patchOff: "08", sysOn: "09", sysOff: "08" },
                    { label: "BANK UP LED",       prefHex3: "17", patchOn: "0B", patchOff: "0A", sysOn: "0B", sysOff: "0A" },
                    { label: "CTL 1 LED",         prefHex3: "18", patchOn: "0D", patchOff: "0C", sysOn: "0D", sysOff: "0C" },
                    { label: "CTL 2 LED",         prefHex3: "19", patchOn: "0F", patchOff: "0E", sysOn: "0F", sysOff: "0E" },
                    { label: "CURRENT NUMBER LED", prefHex3: "1A", patchOn: "11", patchOff: "10", sysOn: "11", sysOff: "10" }
                ]

                Column {
                    width: col.width
                    spacing: 0

                    Rectangle {
                        width: parent.width; height: 32; color: "#313A47"
                        Text { x: 16; anchors.verticalCenter: parent.verticalCenter; text: modelData.label; color: "#ffffff"; font.pixelSize: 14; font.family: "Roboto Condensed" }
                    }

                    // Patch/System preference
                    Item {
                        width: parent.width; height: 56; clip: true
                        Text { x: 32; anchors.verticalCenter: parent.verticalCenter; text: "SOURCE"; color: "#ccffffff"; font.pixelSize: 12; font.family: "Roboto Condensed" }
                        SySwitch { x: 240; y: 6; hex0: "00"; hex1: panelSysHex1; hex2: "20"; hex3: modelData.prefHex3 }
                    }

                    // Patch LED ON/OFF
                    Item {
                        width: parent.width; height: 44
                        Text { x: 32; anchors.verticalCenter: parent.verticalCenter; text: "PATCH LED ON"; color: "#ccffffff"; font.pixelSize: 12; font.family: "Roboto Condensed" }
                        SyComboBox { x: 240; y: 8; hex0: "10"; hex1: panelHex1; hex2: "03"; hex3: modelData.patchOn }
                    }
                    Item {
                        width: parent.width; height: 44
                        Text { x: 32; anchors.verticalCenter: parent.verticalCenter; text: "PATCH LED OFF"; color: "#ccffffff"; font.pixelSize: 12; font.family: "Roboto Condensed" }
                        SyComboBox { x: 240; y: 8; hex0: "10"; hex1: panelHex1; hex2: "03"; hex3: modelData.patchOff }
                    }

                    // System LED ON/OFF
                    Item {
                        width: parent.width; height: 44
                        Text { x: 32; anchors.verticalCenter: parent.verticalCenter; text: "SYSTEM LED ON"; color: "#ccffffff"; font.pixelSize: 12; font.family: "Roboto Condensed" }
                        SyComboBox { x: 240; y: 8; hex0: "00"; hex1: panelSysHex1; hex2: "20"; hex3: modelData.sysOn }
                    }
                    Item {
                        width: parent.width; height: 44
                        Text { x: 32; anchors.verticalCenter: parent.verticalCenter; text: "SYSTEM LED OFF"; color: "#ccffffff"; font.pixelSize: 12; font.family: "Roboto Condensed" }
                        SyComboBox { x: 240; y: 8; hex0: "00"; hex1: panelSysHex1; hex2: "20"; hex3: modelData.sysOff }
                    }

                    Rectangle { width: parent.width - 32; height: 1; color: "#26ffffff"; x: 16 }
                }
            }
        }
    }
}
