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
    property var options: []

    implicitWidth: 180
    implicitHeight: 90

    Component.onCompleted: {
        if (hex0 !== "") {
            label = paramBridge.getLabel(hex0, hex1, hex2, hex3)
            value = paramBridge.getValue(hex0, hex1, hex2, hex3)
            options = paramBridge.getOptions(hex0, hex1, hex2, hex3)

            var model = []
            var currentIndex = 0
            for (var i = 0; i < options.length; i++) {
                model.push(options[i].label)
                if (options[i].value === value) {
                    currentIndex = i
                }
            }
            combo.model = model
            combo.currentIndex = currentIndex
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 2

        Text {
            Layout.alignment: Qt.AlignLeft
            text: root.label
            color: "#aaaaaa"
            font.pixelSize: 9
            font.family: "Roboto Condensed"
            leftPadding: 4
        }

        ComboBox {
            id: combo
            Layout.fillWidth: true
            Layout.preferredHeight: 28

            background: Rectangle {
                color: "#2a2a2a"
                border.color: combo.pressed ? "#00ccff" : "#555"
                border.width: 1
                radius: 3
            }

            contentItem: Text {
                text: combo.displayText
                color: "#eee"
                font.pixelSize: 11
                font.family: "Roboto Condensed"
                verticalAlignment: Text.AlignVCenter
                leftPadding: 6
                elide: Text.ElideRight
            }

            popup: Popup {
                y: combo.height
                width: combo.width
                implicitHeight: contentItem.implicitHeight
                padding: 1

                contentItem: ListView {
                    clip: true
                    implicitHeight: contentHeight
                    model: combo.popup.visible ? combo.delegateModel : null
                    ScrollBar.vertical: ScrollBar {}
                }

                background: Rectangle {
                    color: "#333"
                    border.color: "#555"
                    radius: 3
                }
            }

            delegate: ItemDelegate {
                width: combo.width
                contentItem: Text {
                    text: modelData
                    color: highlighted ? "#00ccff" : "#eee"
                    font.pixelSize: 11
                    font.family: "Roboto Condensed"
                }
                highlighted: combo.highlightedIndex === index
                background: Rectangle {
                    color: highlighted ? "#444" : "transparent"
                }
            }

            onActivated: function(index) {
                if (index >= 0 && index < root.options.length) {
                    root.value = root.options[index].value
                    paramBridge.setValue(root.hex0, root.hex1, root.hex2, root.hex3, root.value)
                }
            }
        }
    }
}
