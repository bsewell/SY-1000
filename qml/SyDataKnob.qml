import QtQuick

// Data knob for multi-nibble parameters (e.g. delay time 0-2000ms).
// Uses paramBridge.getDataValue/setDataValue instead of the single-byte API.
Item {
    id: root

    property string hex0: ""
    property string hex1: ""
    property string hex2: ""
    property string hex3: ""
    property string dataType: "DELAY2000"  // DELAY2000, DELAY1300, PREDELAY, BPM, etc.

    property string label: ""
    property int value: 0
    property int minValue: 0
    property int maxValue: 127
    property string displayValue: ""

    property string filmstrip: "knobs/knob_48.png"
    property int frameCount: 128
    property int frameSize: 48

    implicitWidth: frameSize + 24
    implicitHeight: frameSize + 28

    Component.onCompleted: {
        if (hex0 !== "") {
            minValue = paramBridge.getDataMin(dataType)
            maxValue = paramBridge.getDataMax(dataType)
            value = paramBridge.getDataValue(hex0, hex1, hex2, hex3, dataType)
            displayValue = paramBridge.getDataDisplayValue(dataType, value)
            // Use the parameter's own label (e.g. "Time") rather than the Tables entry
            var paramLabel = paramBridge.getLabel(hex0, hex1, hex2, hex3)
            if (paramLabel !== "") label = paramLabel
        }
    }

    Connections {
        target: paramBridge
        function onDataRefreshed() {
            if (root.hex0 !== "") {
                root.value = paramBridge.getDataValue(root.hex0, root.hex1, root.hex2, root.hex3, root.dataType)
                root.displayValue = paramBridge.getDataDisplayValue(root.dataType, root.value)
            }
        }
        function onParameterChanged(h0, h1, h2, h3, val) {
            if (h0 === root.hex0 && h1 === root.hex1 &&
                    h2 === root.hex2 && h3 === root.hex3) {
                root.value = val
                root.displayValue = paramBridge.getDataDisplayValue(root.dataType, val)
            }
        }
    }

    function _handleDrag(lastY, mouseY) {
        var delta = (lastY - mouseY) * 0.5
        var range = root.maxValue - root.minValue
        var step = Math.max(1, Math.round(range / 200))
        var newVal = root.value + delta * step
        newVal = Math.max(root.minValue, Math.min(root.maxValue, Math.round(newVal)))
        if (newVal !== root.value) {
            root.value = newVal
            root.displayValue = paramBridge.getDataDisplayValue(root.dataType, root.value)
            paramBridge.setDataValue(root.hex0, root.hex1, root.hex2, root.hex3, root.dataType, root.value)
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
                source: root.filmstrip
                width: root.frameSize
                height: root.frameSize * root.frameCount
                property int currentFrame: {
                    var range = root.maxValue - root.minValue
                    if (range <= 0) return 0
                    var ratio = (root.value - root.minValue) / range
                    return Math.round(ratio * (root.frameCount - 1))
                }
                y: -currentFrame * root.frameSize
                fillMode: Image.Stretch; smooth: true; mipmap: true
            }

            MouseArea {
                anchors.fill: parent
                preventStealing: true
                property real lastY: 0
                onPressed: function(mouse) { lastY = mouse.y }
                onPositionChanged: function(mouse) { root._handleDrag(lastY, mouse.y); lastY = mouse.y }
            }
        }

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: root.displayValue
            color: SyTheme.textPrimary
            font.pixelSize: SyTheme.fontBody
            font.bold: true
            font.family: SyTheme.fontFamily
        }

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: root.label
            color: SyTheme.textLabel
            font.pixelSize: SyTheme.fontSmall
            font.family: SyTheme.fontFamily
            width: root.frameSize + 24
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignHCenter
        }
    }
}
