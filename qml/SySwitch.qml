import QtQuick

Item {
    id: root

    property string hex0: ""
    property string hex1: ""
    property string hex2: ""
    property string hex3: ""

    property string label: ""
    property int value: 0

    implicitWidth: 70
    implicitHeight: 100

    Component.onCompleted: {
        if (hex0 !== "") {
            label = paramBridge.getLabel(hex0, hex1, hex2, hex3)
            value = paramBridge.getValue(hex0, hex1, hex2, hex3)
        }
    }

    Column {
        anchors.fill: parent
        spacing: 4

        // Toggle switch
        Rectangle {
            anchors.horizontalCenter: parent.horizontalCenter
            width: 40
            height: 22
            radius: 11
            color: root.value !== 0 ? "#00ccff" : "#444"
            border.color: "#555"
            border.width: 1

            Rectangle {
                x: root.value !== 0 ? parent.width - width - 3 : 3
                y: 3
                width: 16
                height: 16
                radius: 8
                color: "#eee"
                Behavior on x { NumberAnimation { duration: 100 } }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    root.value = root.value !== 0 ? 0 : 1
                    paramBridge.setValue(root.hex0, root.hex1, root.hex2, root.hex3, root.value)
                }
            }
        }

        // State text
        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: root.value !== 0 ? "ON" : "OFF"
            color: root.value !== 0 ? "#00ccff" : "#666"
            font.pixelSize: 10
            font.family: "Roboto Condensed"
        }

        // Label
        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: root.label
            color: "#aaaaaa"
            font.pixelSize: 9
            font.family: "Roboto Condensed"
            width: root.implicitWidth
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignHCenter
        }
    }
}
