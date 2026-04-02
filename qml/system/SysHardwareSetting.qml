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
            SySectionLabel { text: "ASSIGN HOLD"; isHeader: true; showDivider: false }

            // GK VOL HOLD
            SySettingRow {
                label: "GK VOL HOLD"; tall: true
                SySwitch {
                    hex0: "00"; hex1: panelHex1; hex2: "00"; hex3: "09"
                }
            }

            // EXP1 HOLD
            SySettingRow {
                label: "EXP1 HOLD"; tall: true
                SySwitch {
                    hex0: "00"; hex1: panelHex1; hex2: "00"; hex3: "0A"
                }
            }

            // EXP2 HOLD
            SySettingRow {
                label: "EXP2 HOLD"; tall: true
                SySwitch {
                    hex0: "00"; hex1: panelHex1; hex2: "00"; hex3: "0B"
                }
            }

            Rectangle { width: parent.width - 32; height: 1; color: SyTheme.dividerLight; x: 16 }

            // GROUND LIFT section
            SySectionLabel { text: "GROUND LIFT"; isHeader: true; showDivider: false }

            SySettingRow {
                label: "GROUND LIFT"; tall: true
                SySwitch {
                    hex0: "00"; hex1: panelHex1b; hex2: "10"; hex3: "00"
                }
            }

            Rectangle { width: parent.width - 32; height: 1; color: SyTheme.dividerLight; x: 16 }

            // OUTPUT LEVEL section
            SySectionLabel { text: "OUTPUT LEVEL"; isHeader: true; showDivider: false }

            SyComboBox {
                x: 16; width: 420
                hex0: "00"; hex1: panelHex1; hex2: "00"; hex3: "0D"
            }
        }
    }
}
