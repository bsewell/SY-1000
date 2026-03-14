import QtQuick

Rectangle {
    id: root
    color: "#1a1a1a"
    implicitWidth: 800
    implicitHeight: 480

    property string hex1: "00"  // guitar="00", bass="02"
    property string hex2: "12"  // normal pickup always "12"
    property string instLabel: "NORMAL INPUT"
    property color accentColor: Qt.rgba(0.2, 0.4, 1.0, 1)

    // Power switch: "10", hex1, "12", "01"
    property int powerValue: 0
    Component.onCompleted: {
        powerValue = paramBridge.getValue("10", hex1, "12", "01")
    }

    Column {
        anchors.fill: parent
        spacing: 0

        // Header
        Rectangle {
            width: parent.width
            height: 36
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
                                paramBridge.setValue("10", root.hex1, "12", "01", root.powerValue)
                            }
                        }
                    }
                }

                // Title
                Text {
                    text: root.instLabel
                    color: "#ffffff"
                    font.pixelSize: 15
                    font.family: "Roboto Condensed"
                    font.bold: true
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }

        // Divider
        Rectangle {
            width: parent.width
            height: 1
            color: "#333333"
        }

        // Content area
        Item {
            width: parent.width
            height: parent.height - 37

            Row {
                anchors.centerIn: parent
                spacing: 24

                // Level knob
                FilmstripKnob {
                    hex0: "10"; hex1: root.hex1; hex2: "12"; hex3: "02"
                    filmstrip: "knobs/knob_56.png"; frameSize: 56
                }

                // Cable Type
                SyComboBox {
                    hex0: "10"; hex1: root.hex1; hex2: "12"; hex3: "04"
                }

                // Phase
                SyComboBox {
                    hex0: "10"; hex1: root.hex1; hex2: "12"; hex3: "05"
                }
            }
        }
    }
}
