import QtQuick

Item {
    id: root

    property string hex0: ""
    property string hex1: ""
    property string hex2: ""
    property string hex3: ""

    property string label: ""
    property int value: 0
    property var options: []

    implicitWidth: 220
    implicitHeight: 28

    Component.onCompleted: {
        if (hex0 !== "") {
            label = paramBridge.getLabel(hex0, hex1, hex2, hex3)
            value = paramBridge.getValue(hex0, hex1, hex2, hex3)
            options = paramBridge.getOptions(hex0, hex1, hex2, hex3)
        }
    }

    property string displayText: {
        for (var i = 0; i < options.length; i++) {
            if (Number(options[i].value) == Number(value))
                return options[i].label
        }
        return String(value)
    }

    Row {
        anchors.verticalCenter: parent.verticalCenter
        spacing: 8

        Text {
            text: root.label
            color: "#888888"
            font.pixelSize: 11
            font.family: "Roboto Condensed"
            font.capitalization: Font.AllUppercase
            anchors.verticalCenter: parent.verticalCenter
        }

        Rectangle {
            id: comboButton
            width: 160
            height: 26
            radius: 4
            color: "#2a2a2a"
            border.color: popup.visible ? "#00ccff" : "#555"
            border.width: 1
            anchors.verticalCenter: parent.verticalCenter

            Text {
                anchors.fill: parent
                anchors.leftMargin: 8
                anchors.rightMargin: 20
                text: root.displayText
                color: "#dddddd"
                font.pixelSize: 11
                font.family: "Roboto Condensed"
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }

            Text {
                anchors.right: parent.right
                anchors.rightMargin: 6
                anchors.verticalCenter: parent.verticalCenter
                text: "\u25BC"
                color: "#888"
                font.pixelSize: 8
            }

            MouseArea {
                anchors.fill: parent
                onClicked: popup.visible = !popup.visible
            }
        }
    }

    // Dropdown popup
    Rectangle {
        id: popup
        visible: false
        x: comboButton.x
        y: root.height + 2
        width: comboButton.width
        height: Math.min(optionsList.contentHeight + 4, 300)
        radius: 4
        color: "#2a2a2a"
        border.color: "#555"
        border.width: 1
        z: 100
        clip: true

        ListView {
            id: optionsList
            anchors.fill: parent
            anchors.margins: 2
            model: root.options
            delegate: Rectangle {
                width: optionsList.width
                height: 24
                color: Number(modelData.value) == Number(root.value) ? "#00ccff" : optionMouse.containsMouse ? "#3a3a3a" : "transparent"
                radius: 2

                Text {
                    anchors.fill: parent
                    anchors.leftMargin: 8
                    text: modelData.label
                    color: Number(modelData.value) == Number(root.value) ? "#1a1a1a" : "#dddddd"
                    font.pixelSize: 11
                    font.family: "Roboto Condensed"
                    verticalAlignment: Text.AlignVCenter
                }

                MouseArea {
                    id: optionMouse
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
                        root.value = Number(modelData.value)
                        paramBridge.setValue(root.hex0, root.hex1, root.hex2, root.hex3, root.value)
                        popup.visible = false
                    }
                }
            }
        }
    }
}
