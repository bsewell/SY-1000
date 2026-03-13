import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root

    property string hex0: ""
    property string hex1: ""
    property string hex2: ""
    property string hex3: ""

    property string label: ""
    property int value: 0

    implicitWidth: 70
    implicitHeight: 90

    Component.onCompleted: {
        if (hex0 !== "") {
            label = paramBridge.getLabel(hex0, hex1, hex2, hex3)
            value = paramBridge.getValue(hex0, hex1, hex2, hex3)
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 2

        Switch {
            id: sw
            Layout.alignment: Qt.AlignHCenter
            checked: root.value !== 0

            indicator: Rectangle {
                implicitWidth: 36
                implicitHeight: 18
                x: sw.leftPadding
                y: parent.height / 2 - height / 2
                radius: 9
                color: sw.checked ? "#00ccff" : "#444"
                border.color: "#555"

                Rectangle {
                    x: sw.checked ? parent.width - width - 2 : 2
                    y: 2
                    width: 14
                    height: 14
                    radius: 7
                    color: "#eee"
                    Behavior on x { NumberAnimation { duration: 100 } }
                }
            }

            onToggled: {
                root.value = checked ? 1 : 0
                paramBridge.setValue(root.hex0, root.hex1, root.hex2, root.hex3, root.value)
            }
        }

        Text {
            Layout.alignment: Qt.AlignHCenter
            text: root.value !== 0 ? "ON" : "OFF"
            color: root.value !== 0 ? "#00ccff" : "#666"
            font.pixelSize: 10
            font.family: "Roboto Condensed"
        }

        Text {
            Layout.alignment: Qt.AlignHCenter
            text: root.label
            color: "#aaaaaa"
            font.pixelSize: 9
            font.family: "Roboto Condensed"
            elide: Text.ElideRight
            Layout.maximumWidth: root.implicitWidth
            horizontalAlignment: Text.AlignHCenter
        }
    }
}
