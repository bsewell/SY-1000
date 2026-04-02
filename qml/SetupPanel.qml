import QtQuick

Rectangle {
    id: root
    color: SyTheme.bgPanel
    implicitWidth: SyTheme.panelWidth
    implicitHeight: SyTheme.panelHeight

    property string hex1: "00"
    property string hex2: "00"
    property color accentColor: Qt.rgba(0.3, 0.7, 0.4, 1)

    // hex2A/hex2B for synth dynamic overrides (bass mode adjustments)
    property string hex2A: (hex1 === "00") ? "20" : "1F"
    property string hex2B: (hex1 === "00") ? "2B" : "29"

    // Current patch type: 0=GK, 1=Normal
    property int patchType: 0

    Component.onCompleted: {
        patchType = paramBridge.getValue("10", hex1, "12", "00")
    }

    Connections {
        target: patchTypeCombo
        function onValueChanged() {
            root.patchType = patchTypeCombo.value
        }
    }

    Column {
        anchors.fill: parent
        spacing: 0

        // Header
        Rectangle {
            width: parent.width; height: 40
            color: Qt.rgba(0.3 * 0.35, 0.7 * 0.35, 0.4 * 0.35, 1.0)

            Text {
                x: 16; anchors.verticalCenter: parent.verticalCenter
                text: "SETUP"
                color: SyTheme.textPrimary
                font.pixelSize: 16; font.family: SyTheme.fontFamily
                font.bold: true
            }
        }

        Rectangle { width: parent.width; height: 1; color: SyTheme.divider }

        // Patch Type selector
        Rectangle {
            width: parent.width; height: 36
            color: SyTheme.bgControl

            Row {
                anchors.fill: parent
                anchors.leftMargin: 16
                spacing: 12

                Text {
                    text: "PATCH TYPE"
                    color: SyTheme.textDimmed
                    font.pixelSize: 12; font.family: SyTheme.fontFamily
                    font.capitalization: Font.AllUppercase
                    anchors.verticalCenter: parent.verticalCenter
                }

                SyComboBox {
                    id: patchTypeCombo
                    hex0: "10"; hex1: root.hex1; hex2: "12"; hex3: "00"
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }

        Rectangle { width: parent.width; height: 1; color: SyTheme.divider }

        // Content area
        Item {
            width: parent.width
            height: parent.height - 78

            // GK Input Levels view (patchType == 0)
            Column {
                anchors.fill: parent
                anchors.margins: 16
                spacing: 12
                visible: root.patchType === 0

                Text {
                    text: "GK INPUT LEVELS"
                    color: SyTheme.textSysLabel
                    font.pixelSize: 14; font.family: SyTheme.fontFamily
                    font.bold: true
                }

                Rectangle {
                    width: parent.width; height: 1
                    color: SyTheme.dividerLight
                }

                Row {
                    spacing: 24

                    Repeater {
                        model: [
                            { label: "STR 6", idx: 0 },
                            { label: "STR 5", idx: 1 },
                            { label: "STR 4", idx: 2 },
                            { label: "STR 3", idx: 3 },
                            { label: "STR 2", idx: 4 },
                            { label: "STR 1", idx: 5 }
                        ]

                        Column {
                            spacing: 6

                            Rectangle {
                                width: 24; height: 160
                                color: "#222222"
                                border.color: "#444444"
                                border.width: 1
                                radius: 3

                                Rectangle {
                                    width: parent.width - 4
                                    height: parent.height * 0.3
                                    anchors.bottom: parent.bottom
                                    anchors.bottomMargin: 2
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    radius: 2
                                    gradient: Gradient {
                                        GradientStop { position: 0.0; color: "#44cc44" }
                                        GradientStop { position: 1.0; color: "#226622" }
                                    }
                                }
                            }

                            Text {
                                text: modelData.label
                                color: SyTheme.textLabel
                                font.pixelSize: 10; font.family: SyTheme.fontFamily
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
                        }
                    }
                }

                Text {
                    text: "VU meters update in real-time when connected to SY-1000"
                    color: SyTheme.textSection
                    font.pixelSize: 10; font.family: SyTheme.fontFamily
                    font.italic: true
                }
            }

            // Normal Input Levels view (patchType == 1)
            Column {
                anchors.fill: parent
                anchors.margins: 16
                spacing: 12
                visible: root.patchType === 1

                Text {
                    text: "NORMAL INPUT LEVELS"
                    color: SyTheme.textSysLabel
                    font.pixelSize: 14; font.family: SyTheme.fontFamily
                    font.bold: true
                }

                Rectangle {
                    width: parent.width; height: 1
                    color: SyTheme.dividerLight
                }

                Row {
                    spacing: 16

                    Column {
                        spacing: 6

                        Rectangle {
                            width: 40; height: 160
                            color: "#222222"
                            border.color: "#444444"
                            border.width: 1
                            radius: 3

                            Rectangle {
                                width: parent.width - 4
                                height: parent.height * 0.35
                                anchors.bottom: parent.bottom
                                anchors.bottomMargin: 2
                                anchors.horizontalCenter: parent.horizontalCenter
                                radius: 2
                                gradient: Gradient {
                                    GradientStop { position: 0.0; color: "#44cc44" }
                                    GradientStop { position: 1.0; color: "#226622" }
                                }
                            }
                        }

                        Text {
                            text: "L"
                            color: SyTheme.textLabel
                            font.pixelSize: 11; font.family: SyTheme.fontFamily
                            anchors.horizontalCenter: parent.horizontalCenter
                        }
                    }

                    Column {
                        spacing: 6

                        Rectangle {
                            width: 40; height: 160
                            color: "#222222"
                            border.color: "#444444"
                            border.width: 1
                            radius: 3

                            Rectangle {
                                width: parent.width - 4
                                height: parent.height * 0.3
                                anchors.bottom: parent.bottom
                                anchors.bottomMargin: 2
                                anchors.horizontalCenter: parent.horizontalCenter
                                radius: 2
                                gradient: Gradient {
                                    GradientStop { position: 0.0; color: "#44cc44" }
                                    GradientStop { position: 1.0; color: "#226622" }
                                }
                            }
                        }

                        Text {
                            text: "R"
                            color: SyTheme.textLabel
                            font.pixelSize: 11; font.family: SyTheme.fontFamily
                            anchors.horizontalCenter: parent.horizontalCenter
                        }
                    }
                }

                Text {
                    text: "VU meters update in real-time when connected to SY-1000"
                    color: SyTheme.textSection
                    font.pixelSize: 10; font.family: SyTheme.fontFamily
                    font.italic: true
                }
            }
        }
    }
}
