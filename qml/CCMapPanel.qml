import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root
    color: SyTheme.bgPanel
    implicitWidth: SyTheme.panelWidth
    implicitHeight: SyTheme.panelHeight

    property var mappingsList: []

    function refreshMappings() {
        mappingsList = ccHandler.mappingsModel()
    }

    Component.onCompleted: refreshMappings()

    Connections {
        target: ccHandler
        function onMappingsChanged() { refreshMappings() }
        function onLearnStateChanged() { learnIndicator.visible = ccHandler.learning() }
    }

    Column {
        anchors.fill: parent
        spacing: 0

        // ── Header ───────────────────────────────────────────────────────
        Rectangle {
            width: parent.width; height: SyTheme.headerHeight
            color: SyTheme.bgSectionHead
            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 16; anchors.rightMargin: 16
                Text {
                    text: "CC MAPPING"
                    color: SyTheme.textPrimary
                    font.pixelSize: 14; font.family: SyTheme.fontFamily
                    font.bold: true; font.capitalization: Font.AllUppercase
                    Layout.fillWidth: true
                }
                Text {
                    text: root.mappingsList.length + " mappings"
                    color: SyTheme.textDimmed
                    font.pixelSize: SyTheme.fontSmall; font.family: SyTheme.fontFamily
                }
            }
        }

        Rectangle { width: parent.width; height: 1; color: SyTheme.divider }

        // ── Learn indicator bar ──────────────────────────────────────────
        Rectangle {
            id: learnIndicator
            width: parent.width; height: 36
            color: "#331A00"
            visible: false

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 16; anchors.rightMargin: 16
                spacing: 12

                Rectangle {
                    width: 10; height: 10; radius: 5
                    color: "#FF6600"
                    SequentialAnimation on opacity {
                        loops: Animation.Infinite
                        NumberAnimation { to: 0.3; duration: 500 }
                        NumberAnimation { to: 1.0; duration: 500 }
                    }
                }

                Text {
                    text: "LEARN MODE — Move a control on your MIDI controller..."
                    color: "#FFAA00"
                    font.pixelSize: SyTheme.fontBody; font.family: SyTheme.fontFamily
                    font.bold: true
                    Layout.fillWidth: true
                }

                Rectangle {
                    width: 60; height: 24; radius: 3
                    color: "#553300"
                    border.color: "#FF6600"; border.width: 1
                    Text {
                        anchors.centerIn: parent
                        text: "Cancel"
                        color: "#FFAA00"
                        font.pixelSize: SyTheme.fontSmall; font.family: SyTheme.fontFamily
                        font.bold: true
                    }
                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: ccHandler.cancelLearn()
                    }
                }
            }
        }

        // ── Toolbar ──────────────────────────────────────────────────────
        Rectangle {
            width: parent.width; height: 36
            color: SyTheme.bgControl

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 12; anchors.rightMargin: 12
                spacing: 8

                // Quick-learn: pick a common parameter to learn
                Text {
                    text: "Quick Learn:"
                    color: SyTheme.textSecondary
                    font.pixelSize: SyTheme.fontLabel; font.family: SyTheme.fontFamily
                }

                Repeater {
                    model: [
                        { label: "INST1 Level", h: ["10","00","00","31"] },
                        { label: "MASTER Level", h: ["10","00","00","15"] },
                        { label: "FX1 Param1", h: ["10","00","40","02"] },
                        { label: "DELAY1 Level", h: ["10","00","50","02"] }
                    ]
                    Rectangle {
                        width: learnBtnText.implicitWidth + 16; height: 24; radius: 3
                        color: "#1A2A3A"
                        border.color: SyTheme.accent; border.width: 1
                        Text {
                            id: learnBtnText
                            anchors.centerIn: parent
                            text: modelData.label
                            color: SyTheme.accent
                            font.pixelSize: SyTheme.fontSmall; font.family: SyTheme.fontFamily
                        }
                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                ccHandler.startLearn(modelData.h[0], modelData.h[1],
                                                     modelData.h[2], modelData.h[3])
                            }
                        }
                    }
                }

                Item { Layout.fillWidth: true }

                // Save button
                Rectangle {
                    width: 50; height: 24; radius: 3
                    color: "#003322"
                    border.color: "#00FF88"; border.width: 1
                    Text {
                        anchors.centerIn: parent
                        text: "Save"
                        color: "#00FF88"
                        font.pixelSize: SyTheme.fontSmall; font.family: SyTheme.fontFamily
                        font.bold: true
                    }
                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: ccHandler.saveUserMappings()
                    }
                }
            }
        }

        Rectangle { width: parent.width; height: 1; color: SyTheme.divider }

        // ── Column headers ───────────────────────────────────────────────
        Rectangle {
            width: parent.width; height: 28
            color: SyTheme.bgElevated
            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 12; anchors.rightMargin: 12
                spacing: 0
                Text { text: "ON";      color: SyTheme.textDimmed; font.pixelSize: SyTheme.fontTiny; font.family: SyTheme.fontFamily; Layout.preferredWidth: 36 }
                Text { text: "CC#";     color: SyTheme.textDimmed; font.pixelSize: SyTheme.fontTiny; font.family: SyTheme.fontFamily; Layout.preferredWidth: 50 }
                Text { text: "CH";      color: SyTheme.textDimmed; font.pixelSize: SyTheme.fontTiny; font.family: SyTheme.fontFamily; Layout.preferredWidth: 40 }
                Text { text: "PARAMETER"; color: SyTheme.textDimmed; font.pixelSize: SyTheme.fontTiny; font.family: SyTheme.fontFamily; Layout.fillWidth: true }
                Text { text: "ADDRESS"; color: SyTheme.textDimmed; font.pixelSize: SyTheme.fontTiny; font.family: SyTheme.fontFamily; Layout.preferredWidth: 120 }
                Text { text: "";        Layout.preferredWidth: 36 }
            }
        }

        Rectangle { width: parent.width; height: 1; color: SyTheme.divider }

        // ── Mapping list ─────────────────────────────────────────────────
        ListView {
            id: mappingListView
            width: parent.width
            height: parent.height - SyTheme.headerHeight - 1 - 36 - 36 - 1 - 28 - 1
                    - (learnIndicator.visible ? 36 : 0)
            clip: true
            model: root.mappingsList
            spacing: 0

            ScrollBar.vertical: ScrollBar { policy: ScrollBar.AsNeeded }

            delegate: Rectangle {
                width: mappingListView.width
                height: 36
                color: index % 2 === 0 ? "transparent" : "#0Affffff"

                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 12; anchors.rightMargin: 12
                    spacing: 0

                    // Enable checkbox
                    Rectangle {
                        Layout.preferredWidth: 36; Layout.preferredHeight: 20
                        color: "transparent"
                        Rectangle {
                            anchors.centerIn: parent
                            width: 16; height: 16; radius: 3
                            color: modelData.enabled ? "#00FF88" : "#333"
                            border.color: modelData.enabled ? "#00FF88" : "#555"
                            border.width: 1
                            Text {
                                anchors.centerIn: parent
                                text: modelData.enabled ? "\u2713" : ""
                                color: "#000"; font.pixelSize: 12; font.bold: true
                            }
                        }
                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: ccHandler.setMappingEnabled(modelData.channel, modelData.cc, !modelData.enabled)
                        }
                    }

                    // CC number
                    Text {
                        text: modelData.cc
                        color: SyTheme.accent
                        font.pixelSize: SyTheme.fontBody; font.family: "Menlo"
                        font.bold: true
                        Layout.preferredWidth: 50
                    }

                    // Channel
                    Text {
                        text: modelData.channel + 1
                        color: SyTheme.textSecondary
                        font.pixelSize: SyTheme.fontBody; font.family: "Menlo"
                        Layout.preferredWidth: 40
                    }

                    // Label
                    Text {
                        text: modelData.label || "(unnamed)"
                        color: SyTheme.textPrimary
                        font.pixelSize: SyTheme.fontBody; font.family: SyTheme.fontFamily
                        elide: Text.ElideRight
                        Layout.fillWidth: true
                    }

                    // Hex address
                    Text {
                        text: modelData.hex0 + " " + modelData.hex1 + " " + modelData.hex2 + " " + modelData.hex3
                        color: SyTheme.textDimmed
                        font.pixelSize: SyTheme.fontSmall; font.family: "Menlo"
                        Layout.preferredWidth: 120
                    }

                    // Delete button
                    Rectangle {
                        Layout.preferredWidth: 28; Layout.preferredHeight: 20; radius: 3
                        color: "#330000"
                        border.color: "#FF4444"; border.width: 1
                        Text {
                            anchors.centerIn: parent
                            text: "\u2715"
                            color: "#FF4444"
                            font.pixelSize: 11; font.bold: true
                        }
                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: ccHandler.removeMappingByIndex(index)
                        }
                    }
                }
            }

            // Empty state
            Text {
                anchors.centerIn: parent
                visible: root.mappingsList.length === 0
                text: "No CC mappings yet.\nUse Quick Learn above to map a controller knob to a parameter."
                color: SyTheme.textDimmed
                font.pixelSize: SyTheme.fontBody; font.family: SyTheme.fontFamily
                horizontalAlignment: Text.AlignHCenter
            }
        }
    }
}
