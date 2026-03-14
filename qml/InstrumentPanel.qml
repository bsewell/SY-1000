import QtQuick

Rectangle {
    id: root
    color: "#1a1a1a"

    property string hex1: "00"  // inst1="00", inst2="01", inst3="02"
    property string instLabel: "INST 1"

    // Tab definitions
    readonly property var tabNames: ["COMMON", "ALT TUNE", "OSC", "FILTER", "AMP", "LFO1", "LFO2", "SEQ", "LAYER"]
    property int currentTab: 0

    Column {
        anchors.fill: parent
        spacing: 0

        // Header bar with dropdown + tabs in one row
        Rectangle {
            width: parent.width
            height: 32
            color: "#2a2a2a"

            Row {
                anchors.fill: parent
                anchors.leftMargin: 12
                spacing: 0

                // INST TYPE dropdown
                SyComboBox {
                    hex0: "10"; hex1: root.hex1; hex2: "15"; hex3: "01"
                    anchors.verticalCenter: parent.verticalCenter
                }

                // Spacer
                Item { width: 16; height: 1 }

                // Tab bar
                Repeater {
                    model: root.tabNames

                    Rectangle {
                        width: tabText.implicitWidth + 20
                        height: 32
                        color: "transparent"
                        anchors.verticalCenter: parent.verticalCenter

                        Rectangle {
                            anchors.bottom: parent.bottom
                            width: parent.width
                            height: 2
                            color: "#00ccff"
                            visible: index === root.currentTab
                        }

                        Text {
                            id: tabText
                            anchors.centerIn: parent
                            text: modelData
                            color: index === root.currentTab ? "#00ccff" : "#888888"
                            font.pixelSize: 11
                            font.family: "Roboto Condensed"
                            font.bold: index === root.currentTab
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: root.currentTab = index
                        }
                    }
                }
            }
        }

        // Divider
        Rectangle {
            width: parent.width
            height: 1
            color: "#333333"
        }

        // Tab content area
        Item {
            width: parent.width
            height: parent.height - 33  // header + divider

            // COMMON tab
            CommonTab {
                anchors.fill: parent
                hex1: root.hex1
                visible: root.currentTab === 0
            }

            // ALT TUNE tab
            AltTuneTab {
                anchors.fill: parent
                hex1: root.hex1
                visible: root.currentTab === 1
            }

            // OSC tab
            OscTab {
                anchors.fill: parent
                hex1: root.hex1
                visible: root.currentTab === 2
            }

            // FILTER tab
            FilterTab {
                anchors.fill: parent
                hex1: root.hex1
                visible: root.currentTab === 3
            }

            // AMP tab
            AmpTab {
                anchors.fill: parent
                hex1: root.hex1
                visible: root.currentTab === 4
            }

            // LFO1 tab
            Lfo1Tab {
                anchors.fill: parent
                hex1: root.hex1
                visible: root.currentTab === 5
            }

            // LFO2 tab
            Lfo2Tab {
                anchors.fill: parent
                hex1: root.hex1
                visible: root.currentTab === 6
            }

            // SEQ tab
            SeqTab {
                anchors.fill: parent
                hex1: root.hex1
                visible: root.currentTab === 7
            }

            // LAYER tab
            LayerTab {
                anchors.fill: parent
                hex1: root.hex1
                visible: root.currentTab === 8
            }
        }
    }
}
