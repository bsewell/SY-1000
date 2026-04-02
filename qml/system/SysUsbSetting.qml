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

            // Header
            SySectionLabel { text: "USB SETTING"; isHeader: true; showDivider: false }

            // AUDIO ROUTING
            SySettingRow {
                label: "AUDIO ROUTING"
                SyComboBox { hex0: "00"; hex1: panelHex1b; hex2: "00"; hex3: "43" }
            }

            // IN POSITION
            SySettingRow {
                label: "IN POSITION"
                SyComboBox { hex0: "00"; hex1: panelHex1b; hex2: "00"; hex3: "44" }
            }

            // OUT POSITION
            SySettingRow {
                label: "OUT POSITION"
                SyComboBox { hex0: "00"; hex1: panelHex1b; hex2: "00"; hex3: "45" }
            }

            Rectangle { width: parent.width - 32; height: 1; color: SyTheme.dividerLight; x: 16 }
            Item { width: 1; height: 16 }

            // Level knobs row
            Row {
                x: 32
                spacing: 32

                FilmstripKnob {
                    hex0: "00"; hex1: panelHex1b; hex2: "00"; hex3: "41"
                    filmstrip: "knobs/knob_56.png"; frameSize: 56
                }

                FilmstripKnob {
                    hex0: "00"; hex1: panelHex1b; hex2: "00"; hex3: "42"
                    filmstrip: "knobs/knob_56.png"; frameSize: 56
                }
            }
        }
    }
}
