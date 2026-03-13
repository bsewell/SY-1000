import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root

    property string hex0: ""
    property string hex1: ""
    property string hex2: ""
    property string hex3: ""
    property real knobScale: 1.0

    property string label: ""
    property int value: 0
    property int minValue: 0
    property int maxValue: 127
    property string displayValue: ""

    implicitWidth: 90 * knobScale
    implicitHeight: 90 * knobScale

    Component.onCompleted: {
        if (hex0 !== "") {
            label = paramBridge.getLabel(hex0, hex1, hex2, hex3)
            minValue = paramBridge.getMin(hex0, hex1, hex2, hex3)
            maxValue = paramBridge.getMax(hex0, hex1, hex2, hex3)
            value = paramBridge.getValue(hex0, hex1, hex2, hex3)
            displayValue = paramBridge.getDisplayValue(hex0, hex1, hex2, hex3, value)
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 2

        Dial {
            id: dial
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredWidth: 52 * root.knobScale
            Layout.preferredHeight: 52 * root.knobScale
            from: root.minValue
            to: root.maxValue
            value: root.value
            stepSize: 1
            inputMode: Dial.Circular

            background: Rectangle {
                x: dial.width / 2 - width / 2
                y: dial.height / 2 - height / 2
                width: Math.min(dial.width, dial.height)
                height: width
                radius: width / 2
                color: "#2a2a2a"
                border.color: "#555"
                border.width: 1
            }

            handle: Rectangle {
                id: handleItem
                x: dial.width / 2 - width / 2
                y: dial.height / 2 - height / 2
                width: 8 * root.knobScale
                height: 8 * root.knobScale
                radius: width / 2
                color: "#00ccff"
                antialiasing: true

                transform: [
                    Translate {
                        y: -Math.min(dial.width, dial.height) * 0.38
                    },
                    Rotation {
                        angle: dial.angle
                        origin.x: handleItem.width / 2
                        origin.y: handleItem.height / 2 + Math.min(dial.width, dial.height) * 0.38
                    }
                ]
            }

            onMoved: {
                root.value = Math.round(value)
                root.displayValue = paramBridge.getDisplayValue(
                    root.hex0, root.hex1, root.hex2, root.hex3, root.value)
                paramBridge.setValue(root.hex0, root.hex1, root.hex2, root.hex3, root.value)
            }
        }

        Text {
            Layout.alignment: Qt.AlignHCenter
            text: root.displayValue
            color: "#00ccff"
            font.pixelSize: 10 * root.knobScale
            font.family: "Roboto Condensed"
        }

        Text {
            Layout.alignment: Qt.AlignHCenter
            text: root.label
            color: "#aaaaaa"
            font.pixelSize: 9 * root.knobScale
            font.family: "Roboto Condensed"
            elide: Text.ElideRight
            Layout.maximumWidth: root.implicitWidth
            horizontalAlignment: Text.AlignHCenter
        }
    }
}
