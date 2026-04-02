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

            SySectionLabel { text: "BANK"; isHeader: true; showDivider: false }

            SySettingRow {
                label: "BANK CHANGE MODE"; tall: true
                SySwitch { hex0: "00"; hex1: panelHex1; hex2: "00"; hex3: "05" }
            }

            SySettingRow {
                label: "BANK EXTENT MIN"
                SyComboBox { hex0: "00"; hex1: panelHex1; hex2: "00"; hex3: "06" }
            }

            SySettingRow {
                label: "BANK EXTENT MAX"
                SyComboBox { hex0: "00"; hex1: panelHex1; hex2: "00"; hex3: "07" }
            }

            Rectangle { width: parent.width - 32; height: 1; color: SyTheme.dividerLight; x: 16 }

            SySectionLabel { text: "BANK UP/DOWN SW"; isHeader: true; showDivider: false }

            SySettingRow {
                label: "BANK UP/DOWN SW"; tall: true
                SySwitch { hex0: "00"; hex1: panelHex1; hex2: "00"; hex3: "08" }
            }
        }
    }
}
