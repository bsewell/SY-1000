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
                    { label: "BANK DOWN",      src: "26", patchFunc: "18", patchMode: "19", sysFunc: "08", sysMode: "09" },
                    { label: "BANK UP",        src: "27", patchFunc: "1A", patchMode: "1B", sysFunc: "0A", sysMode: "0B" },
                    { label: "CURRENT NUMBER", src: "2D", patchFunc: "28", patchMode: "29", sysFunc: "18", sysMode: "19" }
                ]

                Column {
                    width: col.width
                    spacing: 0

                    Rectangle {
                        width: parent.width; height: 28; color: SyTheme.bgSectionHead
                        Text { x: 16; anchors.verticalCenter: parent.verticalCenter; text: modelData.label; color: SyTheme.textPrimary; font.pixelSize: 13; font.family: SyTheme.fontFamily; font.bold: true }
                    }

                    Item {
                        width: parent.width; height: 44
                        Text { x: 32; anchors.verticalCenter: parent.verticalCenter; text: "SOURCE"; color: SyTheme.textSysLabel; font.pixelSize: 11; font.family: SyTheme.fontFamily }
                        SySwitch { x: 160; y: 6; hex0: "00"; hex1: panelSysHex1; hex2: "10"; hex3: modelData.src }
                    }
                    Item { width: parent.width; height: 36; SyComboBox { x: 16; width: 420; hex0: "10"; hex1: panelHex1; hex2: "00"; hex3: modelData.patchFunc } }
                    Item { width: parent.width; height: 36; SyComboBox { x: 16; width: 420; hex0: "10"; hex1: panelHex1; hex2: "00"; hex3: modelData.patchMode } }
                    Item { width: parent.width; height: 36; SyComboBox { x: 16; width: 420; hex0: "00"; hex1: panelSysHex1; hex2: "10"; hex3: modelData.sysFunc } }
                    Item { width: parent.width; height: 36; SyComboBox { x: 16; width: 420; hex0: "00"; hex1: panelSysHex1; hex2: "10"; hex3: modelData.sysMode } }

                    Rectangle { width: parent.width - 32; height: 1; color: SyTheme.dividerLight; x: 16 }
                }
            }

            Rectangle {
                width: parent.width; height: 28; color: SyTheme.bgSectionHead
                Text { x: 16; anchors.verticalCenter: parent.verticalCenter; text: "BANK UP + BANK DOWN"; color: SyTheme.textPrimary; font.pixelSize: 13; font.family: SyTheme.fontFamily; font.bold: true }
            }
            Item {
                width: parent.width; height: 44
                Text { x: 32; anchors.verticalCenter: parent.verticalCenter; text: "FUNCTION"; color: SyTheme.textSysLabel; font.pixelSize: 11; font.family: SyTheme.fontFamily }
                SySwitch { x: 160; y: 6; hex0: "00"; hex1: panelSysHex1; hex2: "00"; hex3: "08" }
            }
        }
    }
}
