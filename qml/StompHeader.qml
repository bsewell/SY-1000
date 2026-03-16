import QtQuick

// Shared header bar for all stomp/effect panels
// Matches Boss Tone Studio: power icon (⏻) + title + VARIATION dropdown
Item {
    id: root

    property color accentColor: Qt.rgba(0.2, 0.4, 1.0, 1)
    property string title: ""
    property string powerHex0: "10"
    property string powerHex1: "00"
    property string powerHex2: "00"
    property string powerHex3: "00"
    property int powerValue: 0

    implicitWidth: parent ? parent.width : 800
    implicitHeight: 40

    Component.onCompleted: {
        if (powerHex0 !== "") {
            powerValue = paramBridge.getValue(powerHex0, powerHex1, powerHex2, powerHex3)
        }
    }

    // Sync power state when changed externally (e.g. stompbox tile click)
    Connections {
        target: paramBridge
        function onParameterChanged(h0, h1, h2, h3, val) {
            if (h0 === root.powerHex0 && h1 === root.powerHex1 &&
                h2 === root.powerHex2 && h3 === root.powerHex3) {
                root.powerValue = val
            }
        }
    }

    Rectangle {
        anchors.fill: parent
        color: Qt.rgba(root.accentColor.r * 0.35, root.accentColor.g * 0.35, root.accentColor.b * 0.35, 1.0)

        // Power icon button
        Item {
            id: powerBtn
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            width: 32
            height: 32

            Rectangle {
                anchors.centerIn: parent
                width: 28
                height: 28
                radius: 14
                color: "transparent"
                border.color: root.powerValue !== 0 ? root.accentColor : "#666"
                border.width: 2

                Text {
                    anchors.centerIn: parent
                    text: "\u23FB"
                    color: root.powerValue !== 0 ? root.accentColor : "#666"
                    font.pixelSize: 14
                }
            }

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    root.powerValue = root.powerValue !== 0 ? 0 : 1
                    paramBridge.setValue(root.powerHex0, root.powerHex1, root.powerHex2, root.powerHex3, root.powerValue)
                }
            }
        }

        // Title
        Text {
            anchors.left: powerBtn.right
            anchors.leftMargin: 8
            anchors.verticalCenter: parent.verticalCenter
            text: root.title
            color: "#ffffff"
            font.pixelSize: 16
            font.family: "Roboto Condensed"
            font.bold: true
        }

        // VARIATION dropdown (right-aligned)
        Rectangle {
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            width: varText.implicitWidth + 24
            height: 24
            radius: 3
            color: "transparent"
            border.color: "#666"
            border.width: 1

            Text {
                id: varText
                anchors.centerIn: parent
                text: "VARIATION  \u25BC"
                color: "#cccccc"
                font.pixelSize: 10
                font.family: "Roboto Condensed"
            }

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
            }
        }
    }
}
