import QtQuick

Item {
    id: root

    property string hex0: ""
    property string hex1: ""
    property string hex2: ""
    property string hex3: ""

    property string label: ""
    property int value: 0
    property int minValue: 0
    property int maxValue: 127
    property string displayValue: ""

    // Filmstrip config — set filmstrip + frameSize to match the PNG
    property string filmstrip: "knobs/knob_48.png"
    property int frameCount: 128
    property int frameSize: 48   // logical display size (PNG is 2x for Retina)

    implicitWidth: frameSize + 24
    implicitHeight: frameSize + 28

    Component.onCompleted: {
        if (hex0 !== "") {
            label = paramBridge.getLabel(hex0, hex1, hex2, hex3)
            minValue = paramBridge.getMin(hex0, hex1, hex2, hex3)
            maxValue = paramBridge.getMax(hex0, hex1, hex2, hex3)
            value = paramBridge.getValue(hex0, hex1, hex2, hex3)
            displayValue = paramBridge.getDisplayValue(hex0, hex1, hex2, hex3, value)
        }
    }

    Column {
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 2

        Item {
            width: root.frameSize
            height: root.frameSize
            anchors.horizontalCenter: parent.horizontalCenter
            clip: true

            Image {
                id: knobImage
                source: root.filmstrip
                // Native 2x pixels, displayed at logical frameSize
                width: root.frameSize
                height: root.frameSize * root.frameCount

                property int currentFrame: {
                    var range = root.maxValue - root.minValue
                    if (range <= 0) return 0
                    var ratio = (root.value - root.minValue) / range
                    return Math.round(ratio * (root.frameCount - 1))
                }

                y: -currentFrame * root.frameSize

                fillMode: Image.Stretch
                smooth: true
                mipmap: true
            }

            MouseArea {
                anchors.fill: parent
                preventStealing: true
                property real lastY: 0

                onPressed: function(mouse) { lastY = mouse.y }
                onPositionChanged: function(mouse) {
                    var delta = (lastY - mouse.y) * 0.5
                    lastY = mouse.y
                    var range = root.maxValue - root.minValue
                    var step = Math.max(1, Math.round(range / 100))
                    var newVal = root.value + delta * step
                    newVal = Math.max(root.minValue, Math.min(root.maxValue, Math.round(newVal)))
                    if (newVal !== root.value) {
                        root.value = newVal
                        root.displayValue = paramBridge.getDisplayValue(
                            root.hex0, root.hex1, root.hex2, root.hex3, root.value)
                        paramBridge.setValue(root.hex0, root.hex1, root.hex2, root.hex3, root.value)
                    }
                }
            }
        }

        // Value display
        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: root.displayValue
            color: "#00ccff"
            font.pixelSize: 11
            font.family: "Roboto Condensed"
        }

        // Label
        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: root.label
            color: "#aaaaaa"
            font.pixelSize: 10
            font.family: "Roboto Condensed"
            width: root.implicitWidth
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignHCenter
        }
    }
}
