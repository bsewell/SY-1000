import QtQuick

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
    implicitHeight: 100 * knobScale

    Component.onCompleted: {
        if (hex0 !== "") {
            label = paramBridge.getLabel(hex0, hex1, hex2, hex3)
            minValue = paramBridge.getMin(hex0, hex1, hex2, hex3)
            maxValue = paramBridge.getMax(hex0, hex1, hex2, hex3)
            value = paramBridge.getValue(hex0, hex1, hex2, hex3)
            displayValue = paramBridge.getDisplayValue(hex0, hex1, hex2, hex3, value)
        }
    }

    Connections {
        target: paramBridge
        function onDataRefreshed() {
            if (root.hex0 !== "") {
                root.value = paramBridge.getValue(root.hex0, root.hex1, root.hex2, root.hex3)
                root.displayValue = paramBridge.getDisplayValue(root.hex0, root.hex1, root.hex2, root.hex3, root.value)
            }
        }
        function onParameterChanged(h0, h1, h2, h3, val) {
            if (h0 === root.hex0 && h1 === root.hex1 &&
                    h2 === root.hex2 && h3 === root.hex3) {
                root.value = val
                root.displayValue = paramBridge.getDisplayValue(root.hex0, root.hex1, root.hex2, root.hex3, val)
                knobCanvas.requestPaint()
            }
        }
    }

    Column {
        anchors.fill: parent
        spacing: 2

        // Knob canvas
        Canvas {
            id: knobCanvas
            width: 56 * root.knobScale
            height: 56 * root.knobScale
            anchors.horizontalCenter: parent.horizontalCenter

            property real angle: {
                var range = root.maxValue - root.minValue
                if (range <= 0) return -135
                var ratio = (root.value - root.minValue) / range
                return -135 + ratio * 270  // -135 to +135 degrees
            }

            onAngleChanged: requestPaint()

            onPaint: {
                var ctx = getContext("2d")
                var cx = width / 2
                var cy = height / 2
                var r = Math.min(cx, cy) - 4

                ctx.reset()

                // Outer ring
                ctx.beginPath()
                ctx.arc(cx, cy, r, 0, 2 * Math.PI)
                ctx.fillStyle = SyTheme.bgControl
                ctx.fill()
                ctx.strokeStyle = SyTheme.border
                ctx.lineWidth = 1.5
                ctx.stroke()

                // Arc track (background)
                ctx.beginPath()
                ctx.arc(cx, cy, r - 4, (225) * Math.PI / 180, (-45) * Math.PI / 180)
                ctx.strokeStyle = SyTheme.divider
                ctx.lineWidth = 3
                ctx.stroke()

                // Arc track (value)
                var startAngle = 225 * Math.PI / 180
                var endAngle = (225 + (angle + 135) / 270 * 270) * Math.PI / 180
                if (endAngle > startAngle) {
                    ctx.beginPath()
                    ctx.arc(cx, cy, r - 4, startAngle, endAngle)
                    ctx.strokeStyle = SyTheme.accent
                    ctx.lineWidth = 3
                    ctx.stroke()
                }

                // Pointer line
                var pointerAngle = (angle - 90) * Math.PI / 180
                var innerR = r * 0.3
                var outerR = r * 0.75
                ctx.beginPath()
                ctx.moveTo(cx + innerR * Math.cos(pointerAngle), cy + innerR * Math.sin(pointerAngle))
                ctx.lineTo(cx + outerR * Math.cos(pointerAngle), cy + outerR * Math.sin(pointerAngle))
                ctx.strokeStyle = SyTheme.accent
                ctx.lineWidth = 2.5
                ctx.lineCap = "round"
                ctx.stroke()
            }

            MouseArea {
                anchors.fill: parent
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
                        knobCanvas.requestPaint()
                    }
                }
            }
        }

        // Value display
        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: root.displayValue
            color: SyTheme.accent
            font.pixelSize: 10 * root.knobScale
            font.family: SyTheme.fontFamily
        }

        // Label
        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: root.label
            color: SyTheme.textLabel
            font.pixelSize: 9 * root.knobScale
            font.family: SyTheme.fontFamily
            width: root.implicitWidth
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignHCenter
        }
    }
}
