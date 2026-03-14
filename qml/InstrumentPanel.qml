import QtQuick

Rectangle {
    id: root
    color: "#1a1a1a"
    implicitWidth: 800
    implicitHeight: 480

    property string hex1: "00"  // inst1="00", inst2="01", inst3="02"
    property string instLabel: "INSTRUMENT 1"
    property color accentColor: Qt.rgba(1, 0, 0, 1)
    property int instType: 0
    property int currentTab: 0

    // Power switch: "10", hex1, "15", "00"
    property int powerValue: 0
    Component.onCompleted: {
        powerValue = paramBridge.getValue("10", hex1, "15", "00")
    }

    // Tab definitions per instrument type
    readonly property var tabSets: [
        ["COMMON", "ALT TUNE", "OSC", "FILTER", "AMP", "LFO1", "LFO2", "SEQ", "LAYER"],
        ["COMMON", "ALT TUNE", "OSC", "FILTER/AMP", "LFO"],
        ["COMMON", "ALT TUNE", "GR-300"],
        ["COMMON", "ALT TUNE", "GUITAR", "AMP", "NS", "EQ"],
        ["COMMON", "ALT TUNE", "ACOUSTIC", "AMP", "EQ"],
        ["COMMON", "ALT TUNE", "E.BASS", "AMP", "EQ"],
        ["COMMON", "ALT TUNE", "VIO GUITAR", "HARMONY", "EQ"],
        ["COMMON", "ALT TUNE", "POLY FX"]
    ]

    property var tabNames: {
        var idx = instType
        if (idx < 0 || idx >= tabSets.length) idx = 0
        return tabSets[idx]
    }

    onInstTypeChanged: currentTab = 0

    Connections {
        target: instTypeCombo
        function onValueChanged() { root.instType = instTypeCombo.value }
    }

    Column {
        anchors.fill: parent
        spacing: 0

        // ===== Row 1: Header — power button + title only =====
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
                                paramBridge.setValue("10", root.hex1, "15", "00", root.powerValue)
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

        // ===== Row 2: INST TYPE dropdown + tab bar =====
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
                    id: instTypeCombo
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
            height: parent.height - 69

            CommonTab {
                anchors.fill: parent
                hex1: root.hex1
                visible: root.currentTab === 0
            }
            AltTuneTab {
                anchors.fill: parent
                hex1: root.hex1
                visible: root.currentTab === 1
            }

            // DYNAMIC SYNTH (type 0)
            OscTab {
                anchors.fill: parent; hex1: root.hex1
                visible: root.instType === 0 && root.currentTab === 2
            }
            FilterTab {
                anchors.fill: parent; hex1: root.hex1
                visible: root.instType === 0 && root.currentTab === 3
            }
            AmpTab {
                anchors.fill: parent; hex1: root.hex1
                visible: root.instType === 0 && root.currentTab === 4
            }
            Lfo1Tab {
                anchors.fill: parent; hex1: root.hex1
                visible: root.instType === 0 && root.currentTab === 5
            }
            Lfo2Tab {
                anchors.fill: parent; hex1: root.hex1
                visible: root.instType === 0 && root.currentTab === 6
            }
            SeqTab {
                anchors.fill: parent; hex1: root.hex1
                visible: root.instType === 0 && root.currentTab === 7
            }
            LayerTab {
                anchors.fill: parent; hex1: root.hex1
                visible: root.instType === 0 && root.currentTab === 8
            }

            // OSC SYNTH (type 1)
            OscSynthOscTab {
                anchors.fill: parent; hex1: root.hex1
                visible: root.instType === 1 && root.currentTab === 2
            }
            OscSynthFilterAmpTab {
                anchors.fill: parent; hex1: root.hex1
                visible: root.instType === 1 && root.currentTab === 3
            }
            OscSynthLfoTab {
                anchors.fill: parent; hex1: root.hex1
                visible: root.instType === 1 && root.currentTab === 4
            }

            // GR-300 (type 2)
            Gr300Tab {
                anchors.fill: parent; hex1: root.hex1
                visible: root.instType === 2 && root.currentTab === 2
            }

            // E.GUITAR (type 3)
            EGuitarTab {
                anchors.fill: parent; hex1: root.hex1
                visible: root.instType === 3 && root.currentTab === 2
            }
            EGuitarAmpTab {
                anchors.fill: parent; hex1: root.hex1
                visible: root.instType === 3 && root.currentTab === 3
            }
            EGuitarNsTab {
                anchors.fill: parent; hex1: root.hex1
                visible: root.instType === 3 && root.currentTab === 4
            }
            EGuitarEqTab {
                anchors.fill: parent; hex1: root.hex1
                visible: root.instType === 3 && root.currentTab === 5
            }

            // ACOUSTIC (type 4)
            AcousticTab {
                anchors.fill: parent; hex1: root.hex1
                visible: root.instType === 4 && root.currentTab === 2
            }
            AcousticAmpTab {
                anchors.fill: parent; hex1: root.hex1
                visible: root.instType === 4 && root.currentTab === 3
            }
            AcousticEqTab {
                anchors.fill: parent; hex1: root.hex1
                visible: root.instType === 4 && root.currentTab === 4
            }

            // E.BASS (type 5)
            EBassTab {
                anchors.fill: parent; hex1: root.hex1
                visible: root.instType === 5 && root.currentTab === 2
            }
            EBassAmpTab {
                anchors.fill: parent; hex1: root.hex1
                visible: root.instType === 5 && root.currentTab === 3
            }
            EBassEqTab {
                anchors.fill: parent; hex1: root.hex1
                visible: root.instType === 5 && root.currentTab === 4
            }

            // VIO GUITAR (type 6)
            VioGuitarTab {
                anchors.fill: parent; hex1: root.hex1
                visible: root.instType === 6 && root.currentTab === 2
            }
            VioGuitarHarmonyTab {
                anchors.fill: parent; hex1: root.hex1
                visible: root.instType === 6 && root.currentTab === 3
            }
            VioGuitarEqTab {
                anchors.fill: parent; hex1: root.hex1
                visible: root.instType === 6 && root.currentTab === 4
            }

            // POLY FX (type 7)
            PolyFxTab {
                anchors.fill: parent; hex1: root.hex1
                visible: root.instType === 7 && root.currentTab === 2
            }
        }
    }
}
