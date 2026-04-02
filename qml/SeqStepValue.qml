import QtQuick

// Compact spinbox-style step value for SEQ step grid
// Matches the Boss Tone Studio style: rectangular box with value + up/down arrows
Item {
    id: root

    property string hex0: "10"
    property string hex1: ""
    property string hex2: ""
    property string hex3: ""

    property int value: 0
    property int minVal: 0
    property int maxVal: 127
    property bool dimmed: false

    implicitWidth: 60
    implicitHeight: 28

    function reload() {
        if (hex0 !== "" && hex3 !== "") {
            value = paramBridge.getValue(hex0, hex1, hex2, hex3)
            minVal = paramBridge.getMin(hex0, hex1, hex2, hex3)
            maxVal = paramBridge.getMax(hex0, hex1, hex2, hex3)
        }
    }

    Component.onCompleted: reload()
    onHex2Changed: reload()
    onHex3Changed: reload()

    opacity: dimmed ? 0.35 : 1.0

    Rectangle {
        anchors.fill: parent
        radius: 3
        color: SyTheme.bgControl
        border.color: "#444"
        border.width: 1

        Text {
            anchors.left: parent.left
            anchors.leftMargin: 6
            anchors.verticalCenter: parent.verticalCenter
            text: paramBridge.getDisplayValue(root.hex0, root.hex1, root.hex2, root.hex3, root.value)
            color: "#dddddd"
            font.pixelSize: SyTheme.fontLabel
            font.family: SyTheme.fontFamily
        }

        // Up/down arrows column
        Column {
            anchors.right: parent.right
            anchors.rightMargin: 2
            anchors.verticalCenter: parent.verticalCenter
            spacing: 0

            Text {
                text: "\u25B2"
                color: SyTheme.textDimmed
                font.pixelSize: 7
                MouseArea {
                    anchors.fill: parent
                    anchors.margins: -4
                    onClicked: {
                        if (root.value < root.maxVal) {
                            root.value++
                            paramBridge.setValue(root.hex0, root.hex1, root.hex2, root.hex3, root.value)
                        }
                    }
                }
            }
            Text {
                text: "\u25BC"
                color: SyTheme.textDimmed
                font.pixelSize: 7
                MouseArea {
                    anchors.fill: parent
                    anchors.margins: -4
                    onClicked: {
                        if (root.value > root.minVal) {
                            root.value--
                            paramBridge.setValue(root.hex0, root.hex1, root.hex2, root.hex3, root.value)
                        }
                    }
                }
            }
        }

        // Drag to change value
        MouseArea {
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: parent.width - 20
            property real startY: 0
            property int startVal: 0
            onPressed: (mouse) => { startY = mouse.y; startVal = root.value }
            onPositionChanged: (mouse) => {
                var delta = Math.round((startY - mouse.y) / 4)
                var newVal = Math.max(root.minVal, Math.min(root.maxVal, startVal + delta))
                if (newVal !== root.value) {
                    root.value = newVal
                    paramBridge.setValue(root.hex0, root.hex1, root.hex2, root.hex3, root.value)
                }
            }
        }
    }
}
