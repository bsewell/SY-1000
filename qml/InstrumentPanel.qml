import QtQuick

Rectangle {
    id: root
    color: "#1a1a1a"

    property string hex1: "00"  // inst1="00", inst2="01", inst3="02"
    property string instLabel: "INST 1"
    property int instType: 0
    property int currentTab: 0

    // Tab definitions per instrument type
    readonly property var tabSets: [
        ["COMMON", "ALT TUNE", "OSC", "FILTER", "AMP", "LFO1", "LFO2", "SEQ", "LAYER"],  // 0: DYNAMIC SYNTH
        ["COMMON", "ALT TUNE", "OSC", "FILTER/AMP", "LFO"],                                // 1: OSC SYNTH
        ["COMMON", "ALT TUNE", "GR-300"],                                                   // 2: GR-300
        ["COMMON", "ALT TUNE", "GUITAR", "AMP", "NS", "EQ"],                                // 3: E.GUITAR
        ["COMMON", "ALT TUNE", "ACOUSTIC", "AMP", "EQ"],                                    // 4: ACOUSTIC
        ["COMMON", "ALT TUNE", "E.BASS", "AMP", "EQ"],                                      // 5: E.BASS
        ["COMMON", "ALT TUNE", "VIO GUITAR", "HARMONY", "EQ"],                              // 6: VIO GUITAR
        ["COMMON", "ALT TUNE", "POLY FX"]                                                   // 7: POLY FX
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
            height: parent.height - 33

            // ====== Shared tabs (all inst types have COMMON at 0, ALT TUNE at 1) ======
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

            // ====== DYNAMIC SYNTH (type 0) ======
            OscTab {
                anchors.fill: parent
                hex1: root.hex1
                visible: root.instType === 0 && root.currentTab === 2
            }
            FilterTab {
                anchors.fill: parent
                hex1: root.hex1
                visible: root.instType === 0 && root.currentTab === 3
            }
            AmpTab {
                anchors.fill: parent
                hex1: root.hex1
                visible: root.instType === 0 && root.currentTab === 4
            }
            Lfo1Tab {
                anchors.fill: parent
                hex1: root.hex1
                visible: root.instType === 0 && root.currentTab === 5
            }
            Lfo2Tab {
                anchors.fill: parent
                hex1: root.hex1
                visible: root.instType === 0 && root.currentTab === 6
            }
            SeqTab {
                anchors.fill: parent
                hex1: root.hex1
                visible: root.instType === 0 && root.currentTab === 7
            }
            LayerTab {
                anchors.fill: parent
                hex1: root.hex1
                visible: root.instType === 0 && root.currentTab === 8
            }

            // ====== OSC SYNTH (type 1) ======
            OscSynthOscTab {
                anchors.fill: parent
                hex1: root.hex1
                visible: root.instType === 1 && root.currentTab === 2
            }
            OscSynthFilterAmpTab {
                anchors.fill: parent
                hex1: root.hex1
                visible: root.instType === 1 && root.currentTab === 3
            }
            OscSynthLfoTab {
                anchors.fill: parent
                hex1: root.hex1
                visible: root.instType === 1 && root.currentTab === 4
            }

            // ====== GR-300 (type 2) ======
            Gr300Tab {
                anchors.fill: parent
                hex1: root.hex1
                visible: root.instType === 2 && root.currentTab === 2
            }

            // ====== E.GUITAR (type 3) ======
            EGuitarTab {
                anchors.fill: parent
                hex1: root.hex1
                visible: root.instType === 3 && root.currentTab === 2
            }
            EGuitarAmpTab {
                anchors.fill: parent
                hex1: root.hex1
                visible: root.instType === 3 && root.currentTab === 3
            }
            EGuitarNsTab {
                anchors.fill: parent
                hex1: root.hex1
                visible: root.instType === 3 && root.currentTab === 4
            }
            EGuitarEqTab {
                anchors.fill: parent
                hex1: root.hex1
                visible: root.instType === 3 && root.currentTab === 5
            }

            // ====== ACOUSTIC (type 4) ======
            AcousticTab {
                anchors.fill: parent
                hex1: root.hex1
                visible: root.instType === 4 && root.currentTab === 2
            }
            AcousticAmpTab {
                anchors.fill: parent
                hex1: root.hex1
                visible: root.instType === 4 && root.currentTab === 3
            }
            AcousticEqTab {
                anchors.fill: parent
                hex1: root.hex1
                visible: root.instType === 4 && root.currentTab === 4
            }

            // ====== E.BASS (type 5) ======
            EBassTab {
                anchors.fill: parent
                hex1: root.hex1
                visible: root.instType === 5 && root.currentTab === 2
            }
            EBassAmpTab {
                anchors.fill: parent
                hex1: root.hex1
                visible: root.instType === 5 && root.currentTab === 3
            }
            EBassEqTab {
                anchors.fill: parent
                hex1: root.hex1
                visible: root.instType === 5 && root.currentTab === 4
            }

            // ====== VIO GUITAR (type 6) ======
            VioGuitarTab {
                anchors.fill: parent
                hex1: root.hex1
                visible: root.instType === 6 && root.currentTab === 2
            }
            VioGuitarHarmonyTab {
                anchors.fill: parent
                hex1: root.hex1
                visible: root.instType === 6 && root.currentTab === 3
            }
            VioGuitarEqTab {
                anchors.fill: parent
                hex1: root.hex1
                visible: root.instType === 6 && root.currentTab === 4
            }

            // ====== POLY FX (type 7) ======
            PolyFxTab {
                anchors.fill: parent
                hex1: root.hex1
                visible: root.instType === 7 && root.currentTab === 2
            }
        }
    }
}
