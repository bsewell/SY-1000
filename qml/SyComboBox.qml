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

    function openPopup() {
        // Find the scene root (top-level item in QQuickWidget)
        var topItem = root
        while (topItem.parent) topItem = topItem.parent

        // Create overlay + popup at scene root level
        var overlay = overlayComponent.createObject(topItem)
        if (!overlay) return

        // Position the dropdown below the comboButton in scene coordinates
        var globalPos = comboButton.mapToItem(topItem, 0, comboButton.height + 2)
        overlay.popupX = globalPos.x
        overlay.popupY = globalPos.y
        overlay.popupWidth = comboButton.width
    }

    Component {
        id: overlayComponent

        Item {
            id: overlay
            anchors.fill: parent
            z: 10000

            property real popupX: 0
            property real popupY: 0
            property real popupWidth: 160

            // Dismiss backdrop — clicking anywhere outside closes the popup
            MouseArea {
                anchors.fill: parent
                onClicked: overlay.destroy()
            }

            Rectangle {
                x: overlay.popupX
                y: overlay.popupY
                width: overlay.popupWidth
                height: Math.min(popupList.contentHeight + 4, 300)
                radius: 4
                color: "#2a2a2a"
                border.color: "#555"
                border.width: 1
                clip: true

                ListView {
                    id: popupList
                    anchors.fill: parent
                    anchors.margins: 2
                    model: root.options
                    delegate: Rectangle {
                        width: popupList.width
                        height: 24
                        color: Number(modelData.value) === Number(root.value) ? "#00ccff"
                             : delegateMouse.containsMouse ? "#3a3a3a" : "transparent"
                        radius: 2

                        Text {
                            anchors.fill: parent
                            anchors.leftMargin: 8
                            text: modelData.label
                            color: Number(modelData.value) === Number(root.value) ? "#1a1a1a" : "#dddddd"
                            font.pixelSize: 11
                            font.family: "Roboto Condensed"
                            verticalAlignment: Text.AlignVCenter
                        }

                        MouseArea {
                            id: delegateMouse
                            anchors.fill: parent
                            hoverEnabled: true
                            onClicked: {
                                root.value = Number(modelData.value)
                                paramBridge.setValue(root.hex0, root.hex1, root.hex2, root.hex3, root.value)
                                overlay.destroy()
                            }
                        }
                    }
                }
            }
        }
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
            border.color: "#555"
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
                onClicked: root.openPopup()
            }
        }
    }
}
