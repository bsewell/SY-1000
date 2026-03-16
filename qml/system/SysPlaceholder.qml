import QtQuick
import ".."

Item {
    anchors.fill: parent
    property string panelHex1: parent ? parent.panelHex1 : "01"
    property string panelHex1b: parent ? parent.panelHex1b : "04"

    Text {
        anchors.centerIn: parent
        text: "Not yet implemented"
        color: "#666666"
        font.pixelSize: 14
        font.family: "Roboto Condensed"
    }
}
