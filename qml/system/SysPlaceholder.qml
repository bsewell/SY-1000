import QtQuick
import ".."

Item {
    anchors.fill: parent
    property string panelHex1: parent ? parent.panelHex1 : "01"
    property string panelHex1b: parent ? parent.panelHex1b : "04"

    Text {
        anchors.centerIn: parent
        text: "No parameters for this page"
        color: SyTheme.textSection
        font.pixelSize: SyTheme.fontSectionHead
        font.family: SyTheme.fontFamily
    }
}
