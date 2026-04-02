import QtQuick
import ".."

Item {
    anchors.fill: parent
    property string panelHex1: parent ? parent.panelHex1 : "01"
    property string panelHex1b: parent ? parent.panelHex1b : "04"

    Column {
        width: parent.width
        spacing: 0

        SySectionLabel { text: "GUITAR TO MIDI"; isHeader: true; showDivider: false }

        // On/Off switch
        SySettingRow {
            label: "GUITAR TO MIDI"; tall: true
            SySwitch {
                hex0: "00"; hex1: panelHex1b; hex2: "21"; hex3: "00"
            }
        }

        Rectangle { width: parent.width - 32; height: 1; color: SyTheme.dividerLight; x: 16 }

        Repeater {
            model: [
                { label: "PARAMETER 1", hex3: "01" },
                { label: "PARAMETER 2", hex3: "02" },
                { label: "PARAMETER 3", hex3: "03" },
                { label: "PARAMETER 4", hex3: "04" },
                { label: "PARAMETER 5", hex3: "05" }
            ]

            SyComboBox {
                x: 16; width: 420
                hex0: "00"; hex1: panelHex1b; hex2: "21"; hex3: modelData.hex3
            }
        }
    }
}
