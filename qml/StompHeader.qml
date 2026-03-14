import QtQuick

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
    implicitHeight: 36

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
        color: Qt.rgba(root.accentColor.r, root.accentColor.g, root.accentColor.b, 0.82)

        Row {
            anchors.fill: parent
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            spacing: 8

            // Power button
            Item {
                width: 40
                height: 36
                anchors.verticalCenter: parent.verticalCenter

                Rectangle {
                    anchors.centerIn: parent
                    width: 28
                    height: 18
                    radius: 9
                    color: root.powerValue !== 0 ? "#ffffff" : Qt.rgba(0, 0, 0, 0.3)
                    border.color: Qt.rgba(1, 1, 1, 0.4)
                    border.width: 1

                    Rectangle {
                        x: root.powerValue !== 0 ? parent.width - width - 2 : 2
                        y: 2
                        width: 14
                        height: 14
                        radius: 7
                        color: root.powerValue !== 0 ? root.accentColor : "#888"
                        Behavior on x { NumberAnimation { duration: 100 } }
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            root.powerValue = root.powerValue !== 0 ? 0 : 1
                            paramBridge.setValue(root.powerHex0, root.powerHex1, root.powerHex2, root.powerHex3, root.powerValue)
                        }
                    }
                }
            }

            // Title
            Text {
                text: root.title
                color: "#ffffff"
                font.pixelSize: 15
                font.family: "Roboto Condensed"
                font.bold: true
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }
}
