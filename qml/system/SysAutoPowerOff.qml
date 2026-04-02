import QtQuick
import ".."

Item {
    anchors.fill: parent
    property string panelHex1: parent ? parent.panelHex1 : "01"
    property string panelHex1b: parent ? parent.panelHex1b : "04"

    Column {
        width: parent.width
        spacing: 0

        SySectionLabel { text: "AUTO POWER OFF"; isHeader: true; showDivider: false }

        SyComboBox {
            x: 16; width: 420
            hex0: "00"; hex1: "00"; hex2: "00"; hex3: "00"
        }
    }
}
