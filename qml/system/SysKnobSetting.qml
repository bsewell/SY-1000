import QtQuick
import ".."

Item {
    id: knobRoot
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

            // KNOB LOCK section
            SySectionLabel { text: "KNOB LOCK 1~6"; isHeader: true; showDivider: false }

            SySettingRow {
                label: "KNOB LOCK"
                SyComboBox {
                    hex0: "00"; hex1: knobRoot.panelHex1; hex2: "00"; hex3: "0C"
                }
            }

            // Divider
            Rectangle { width: parent.width - 32; height: 1; color: SyTheme.dividerLight; x: 16 }

            // KNOB FUNCTION section
            SySectionLabel { text: "KNOB FUNCTION 1~6"; isHeader: true; showDivider: false }

            // 6 knob function rows — each on its own line to avoid overlap
            Repeater {
                model: [
                    { label: "KNOB 1 SETTING", hex3: "0E" },
                    { label: "KNOB 2 SETTING", hex3: "12" },
                    { label: "KNOB 3 SETTING", hex3: "16" },
                    { label: "KNOB 4 SETTING", hex3: "1A" },
                    { label: "KNOB 5 SETTING", hex3: "1E" },
                    { label: "KNOB 6 SETTING", hex3: "22" }
                ]

                SyComboBox {
                    x: 16; width: 420
                    hex0: "00"; hex1: knobRoot.panelHex1; hex2: "00"; hex3: modelData.hex3
                }
            }
        }
    }
}
