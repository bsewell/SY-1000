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

    // Layout mode: "bottom" = label+value below knob, "left" = label+value to left of knob
    property string labelPosition: "bottom"

    // Dimmed state: control stays visible but grayed out and non-interactive
    property bool dimmed: false
    opacity: dimmed ? 0.35 : 1.0

    implicitWidth: labelPosition === "left" ? 56 + 4 + frameSize : frameSize + 24
    implicitHeight: labelPosition === "left" ? frameSize : frameSize + 28

    Component.onCompleted: {
        if (hex0 !== "") {
            label = paramBridge.getLabel(hex0, hex1, hex2, hex3)
            minValue = paramBridge.getMin(hex0, hex1, hex2, hex3)
            maxValue = paramBridge.getMax(hex0, hex1, hex2, hex3)
            value = paramBridge.getValue(hex0, hex1, hex2, hex3)
            displayValue = paramBridge.getDisplayValue(hex0, hex1, hex2, hex3, value)
        }
    }

    // CC knob registration is handled by C++ scanAndRegisterKnobs() when panel activates

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
            }
        }
    }

    // Knob drag interaction helper
    function _handleDrag(lastY, mouseY) {
        var delta = (lastY - mouseY) * 0.5
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

    // === Bottom layout (default) ===
    Column {
        visible: root.labelPosition === "bottom"
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
                enabled: !root.dimmed
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

    // === Left layout (compact — label+value on left, knob on right) ===
    Row {
        visible: root.labelPosition === "left"
        spacing: 4
        anchors.verticalCenter: parent.verticalCenter

        Column {
            anchors.verticalCenter: parent.verticalCenter
            width: 56

            Text {
                text: root.label
                color: SyTheme.textLabel
                font.pixelSize: 9
                font.family: SyTheme.fontFamily
                width: 56
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignRight
            }

            Text {
                text: root.displayValue
                color: SyTheme.textPrimary
                font.pixelSize: SyTheme.fontBody
                font.bold: true
                font.family: SyTheme.fontFamily
                width: 56
                horizontalAlignment: Text.AlignRight
            }
        }

        Item {
            width: root.frameSize
            height: root.frameSize
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
                enabled: !root.dimmed
                preventStealing: true
                property real lastY: 0
                onPressed: function(mouse) { lastY = mouse.y }
                onPositionChanged: function(mouse) { root._handleDrag(lastY, mouse.y); lastY = mouse.y }
            }
        }
    }
}
