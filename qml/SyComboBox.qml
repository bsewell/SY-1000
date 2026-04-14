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

    // Measure the widest option label to auto-size the dropdown
    TextMetrics {
        id: optionMetrics
        font.pixelSize: SyTheme.fontLabel
        font.family: SyTheme.fontFamily
    }

    property real maxOptionWidth: {
        var maxW = 60   // minimum so empty combos aren't tiny
        for (var i = 0; i < options.length; i++) {
            optionMetrics.text = options[i].label
            if (optionMetrics.width > maxW) maxW = optionMetrics.width
        }
        return maxW
    }

    // 8px left pad + text + 20px arrow + 8px right pad
    property real comboWidth: maxOptionWidth + 36

    implicitWidth: showLabel ? (labelWidth + 4 + comboWidth) : comboWidth
    implicitHeight: 28

    Component.onCompleted: {
        if (hex0 !== "") {
            label = paramBridge.getLabel(hex0, hex1, hex2, hex3)
            value = paramBridge.getValue(hex0, hex1, hex2, hex3)
            options = paramBridge.getOptions(hex0, hex1, hex2, hex3)
        }
    }

    Connections {
        target: paramBridge
        function onDataRefreshed() {
            if (root.hex0 !== "") {
                root.value = paramBridge.getValue(root.hex0, root.hex1, root.hex2, root.hex3)
            }
        }
        function onParameterChanged(h0, h1, h2, h3, val) {
            if (h0 === root.hex0 && h1 === root.hex1 &&
                    h2 === root.hex2 && h3 === root.hex3) {
                root.value = val
            }
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
                onClicked: Qt.callLater(overlay.destroy)
            }

            Rectangle {
                x: overlay.popupX
                y: overlay.popupY
                width: overlay.popupWidth
                height: Math.min(popupList.contentHeight + 4, 300)
                radius: 4
                color: SyTheme.bgControl
                border.color: SyTheme.border
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
                        color: Number(modelData.value) === Number(root.value) ? SyTheme.accent
                             : delegateMouse.containsMouse ? "#3a3a3a" : "transparent"
                        radius: 2

                        Text {
                            anchors.fill: parent
                            anchors.leftMargin: 8
                            text: modelData.label
                            color: Number(modelData.value) === Number(root.value) ? SyTheme.bgPanel : "#dddddd"
                            font.pixelSize: SyTheme.fontLabel
                            font.family: SyTheme.fontFamily
                            verticalAlignment: Text.AlignVCenter
                        }

                        MouseArea {
                            id: delegateMouse
                            anchors.fill: parent
                            hoverEnabled: true
                            onClicked: {
                                root.value = Number(modelData.value)
                                paramBridge.setValue(root.hex0, root.hex1, root.hex2, root.hex3, root.value)
                                Qt.callLater(overlay.destroy)
                            }
                        }
                    }
                }
            }
        }
    }

    property bool showLabel: true

    // Measure label text width independently (avoids binding loop with Text element)
    TextMetrics {
        id: labelMetrics
        font.pixelSize: SyTheme.fontLabel
        font.family: SyTheme.fontFamily
        text: root.label.toUpperCase()
    }

    property int labelWidth: showLabel ? (labelMetrics.width + 8) : 0

    Text {
        id: labelText
        visible: root.showLabel
        anchors.verticalCenter: parent.verticalCenter
        text: root.label
        color: SyTheme.textDimmed
        font.pixelSize: SyTheme.fontLabel
        font.family: SyTheme.fontFamily
        font.capitalization: Font.AllUppercase
    }

    Rectangle {
        id: comboButton
        x: root.showLabel ? (root.labelWidth + 4) : 0
        width: root.comboWidth
        height: 26
        radius: 4
        color: SyTheme.bgControl
        border.color: SyTheme.border
        border.width: 1
        anchors.verticalCenter: parent.verticalCenter

            Text {
                anchors.fill: parent
                anchors.leftMargin: 8
                anchors.rightMargin: 20
                text: root.displayText
                color: "#dddddd"
                font.pixelSize: SyTheme.fontLabel
                font.family: SyTheme.fontFamily
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }

            Text {
                anchors.right: parent.right
                anchors.rightMargin: 6
                anchors.verticalCenter: parent.verticalCenter
                text: "\u25BC"
                color: SyTheme.textDimmed
                font.pixelSize: 8
            }

        MouseArea {
            anchors.fill: parent
            onClicked: root.openPopup()
        }
    }
}
