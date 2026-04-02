import QtQuick

Rectangle {
    id: root
    color: SyTheme.bgPanel
    implicitWidth: SyTheme.panelWidth
    implicitHeight: SyTheme.panelHeight

    property string hex1: "00"  // guitar="00", bass="02"
    property string hex2: "15"  // inst1="15", inst2="20"/"1F", inst3="2B"/"29"
    property string instLabel: "INSTRUMENT 1"
    property color accentColor: Qt.rgba(1, 0, 0, 1)
    property int instType: 0
    property int currentTab: 0

    // Compute page-specific hex2 from base hex2 + offset
    // Guitar mode offsets: +0=COMMON, +1=DynSynth, +2=DynSynthCont, +3=OscSynth,
    //   +4=GR300, +5=EGuitar, +6=Acoustic, +7=EBass, +8=VioGuitar, +9=PolyFX, +10=AltTune
    // Bass mode: VioGuitar page doesn't exist, so PolyFX=+8, AltTune=+9
    function pageHex2(offset) {
        var base = parseInt(hex2, 16)
        var val = base + offset
        var h = val.toString(16).toUpperCase()
        return h.length < 2 ? "0" + h : h
    }

    // Guitar-mode offsets (used for most pages)
    readonly property string hex2DynSynth:     pageHex2(1)
    readonly property string hex2DynSynthCont: pageHex2(2)
    readonly property string hex2OscSynth:     pageHex2(3)
    readonly property string hex2Gr300:        pageHex2(4)
    readonly property string hex2EGuitar:      pageHex2(5)
    readonly property string hex2Acoustic:     pageHex2(6)
    readonly property string hex2EBass:        pageHex2(7)
    readonly property string hex2VioGuitar:    pageHex2(8)
    // PolyFX and AltTune shift in bass mode (no VioGuitar page)
    readonly property string hex2PolyFx:       hex1 === "02" ? pageHex2(8) : pageHex2(9)
    readonly property string hex2AltTune:      hex1 === "02" ? pageHex2(9) : pageHex2(10)

    // Power switch: "10", hex1, hex2, "00"
    property int powerValue: 0
    Component.onCompleted: {
        powerValue = paramBridge.getValue("10", hex1, hex2, "00")
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

        // ===== Row 1: Header — power icon + title + VARIATION button =====
        Rectangle {
            width: parent.width
            height: 40
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

                    // Power symbol ⏻ — circle with line
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
                        paramBridge.setValue("10", root.hex1, root.hex2, "00", root.powerValue)
                    }
                }
            }

            // Title
            Text {
                anchors.left: powerBtn.right
                anchors.leftMargin: 8
                anchors.verticalCenter: parent.verticalCenter
                text: root.instLabel
                color: SyTheme.textPrimary
                font.pixelSize: SyTheme.fontTitle
                font.family: SyTheme.fontFamily
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
                border.color: SyTheme.border
                border.width: 1

                Text {
                    id: varText
                    anchors.centerIn: parent
                    text: "VARIATION  \u25BC"
                    color: SyTheme.textSecondary
                    font.pixelSize: SyTheme.fontSmall
                    font.family: SyTheme.fontFamily
                }

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                }
            }
        }

        // ===== Row 2: INST TYPE label + dropdown + tab bar =====
        Rectangle {
            width: parent.width
            height: 34
            color: SyTheme.bgControl

            // "INST TYPE" static label
            Text {
                id: instTypeLabel
                anchors.left: parent.left
                anchors.leftMargin: 12
                anchors.verticalCenter: parent.verticalCenter
                text: "INST TYPE"
                color: SyTheme.textDimmed
                font.pixelSize: SyTheme.fontLabel
                font.family: SyTheme.fontFamily
            }

            // Dropdown — immediately after label
            SyComboBox {
                id: instTypeCombo
                hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "01"
                anchors.left: instTypeLabel.right
                anchors.leftMargin: 8
                anchors.verticalCenter: parent.verticalCenter
                labelWidth: 0
                implicitWidth: 180
            }

            // Tab bar — anchored left of dropdown, reads left-to-right
            Row {
                id: tabRow
                anchors.left: instTypeCombo.right
                anchors.leftMargin: 16
                anchors.verticalCenter: parent.verticalCenter
                height: parent.height
                spacing: 0

                Repeater {
                    model: root.tabNames

                    Rectangle {
                        width: tabText.implicitWidth + 20
                        height: 34
                        color: "transparent"

                        Rectangle {
                            anchors.bottom: parent.bottom
                            width: parent.width
                            height: 2
                            color: SyTheme.accent
                            visible: index === root.currentTab
                        }

                        Text {
                            id: tabText
                            anchors.centerIn: parent
                            text: modelData
                            color: index === root.currentTab ? SyTheme.accent : SyTheme.textDimmed
                            font.pixelSize: SyTheme.fontLabel
                            font.family: SyTheme.fontFamily
                            font.bold: index === root.currentTab
                        }

                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
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
            color: SyTheme.divider
        }

        // Tab content area
        Item {
            width: parent.width
            height: parent.height - 75

            CommonTab {
                anchors.fill: parent
                hex1: root.hex1
                hex2: root.hex2
                visible: root.currentTab === 0
            }
            AltTuneTab {
                anchors.fill: parent
                hex1: root.hex1
                hex2: root.hex2AltTune
                visible: root.currentTab === 1
            }

            // DYNAMIC SYNTH (type 0)
            OscTab {
                anchors.fill: parent; hex1: root.hex1; hex2: root.hex2DynSynth
                visible: root.instType === 0 && root.currentTab === 2
            }
            FilterTab {
                anchors.fill: parent; hex1: root.hex1; hex2: root.hex2DynSynth
                visible: root.instType === 0 && root.currentTab === 3
            }
            AmpTab {
                anchors.fill: parent; hex1: root.hex1; hex2: root.hex2DynSynth
                visible: root.instType === 0 && root.currentTab === 4
            }
            Lfo1Tab {
                anchors.fill: parent; hex1: root.hex1; hex2: root.hex2DynSynth
                visible: root.instType === 0 && root.currentTab === 5
            }
            Lfo2Tab {
                anchors.fill: parent; hex1: root.hex1; hex2: root.hex2DynSynth
                visible: root.instType === 0 && root.currentTab === 6
            }
            SeqTab {
                anchors.fill: parent; hex1: root.hex1
                hex2a: root.hex2DynSynth; hex2b: root.hex2DynSynthCont
                visible: root.instType === 0 && root.currentTab === 7
            }
            LayerTab {
                anchors.fill: parent; hex1: root.hex1; hex2: root.hex2DynSynthCont
                visible: root.instType === 0 && root.currentTab === 8
            }

            // OSC SYNTH (type 1)
            OscSynthOscTab {
                anchors.fill: parent; hex1: root.hex1; hex2: root.hex2OscSynth
                visible: root.instType === 1 && root.currentTab === 2
            }
            OscSynthFilterAmpTab {
                anchors.fill: parent; hex1: root.hex1; hex2: root.hex2OscSynth
                visible: root.instType === 1 && root.currentTab === 3
            }
            OscSynthLfoTab {
                anchors.fill: parent; hex1: root.hex1; hex2: root.hex2OscSynth
                visible: root.instType === 1 && root.currentTab === 4
            }

            // GR-300 (type 2)
            Gr300Tab {
                anchors.fill: parent; hex1: root.hex1; hex2: root.hex2Gr300
                visible: root.instType === 2 && root.currentTab === 2
            }

            // E.GUITAR (type 3)
            EGuitarTab {
                anchors.fill: parent; hex1: root.hex1; hex2: root.hex2EGuitar
                visible: root.instType === 3 && root.currentTab === 2
            }
            EGuitarAmpTab {
                anchors.fill: parent; hex1: root.hex1; hex2: root.hex2EGuitar
                visible: root.instType === 3 && root.currentTab === 3
            }
            EGuitarNsTab {
                anchors.fill: parent; hex1: root.hex1; hex2: root.hex2EGuitar
                visible: root.instType === 3 && root.currentTab === 4
            }
            EGuitarEqTab {
                anchors.fill: parent; hex1: root.hex1; hex2: root.hex2EGuitar
                visible: root.instType === 3 && root.currentTab === 5
            }

            // ACOUSTIC (type 4)
            AcousticTab {
                anchors.fill: parent; hex1: root.hex1; hex2: root.hex2Acoustic
                visible: root.instType === 4 && root.currentTab === 2
            }
            AcousticAmpTab {
                anchors.fill: parent; hex1: root.hex1; hex2: root.hex2Acoustic
                visible: root.instType === 4 && root.currentTab === 3
            }
            AcousticEqTab {
                anchors.fill: parent; hex1: root.hex1; hex2: root.hex2Acoustic
                visible: root.instType === 4 && root.currentTab === 4
            }

            // E.BASS (type 5)
            EBassTab {
                anchors.fill: parent; hex1: root.hex1; hex2: root.hex2EBass
                visible: root.instType === 5 && root.currentTab === 2
            }
            EBassAmpTab {
                anchors.fill: parent; hex1: root.hex1; hex2: root.hex2EBass
                visible: root.instType === 5 && root.currentTab === 3
            }
            EBassEqTab {
                anchors.fill: parent; hex1: root.hex1; hex2: root.hex2EBass
                visible: root.instType === 5 && root.currentTab === 4
            }

            // VIO GUITAR (type 6)
            VioGuitarTab {
                anchors.fill: parent; hex1: root.hex1; hex2: root.hex2VioGuitar
                visible: root.instType === 6 && root.currentTab === 2
            }
            VioGuitarHarmonyTab {
                anchors.fill: parent; hex1: root.hex1; hex2: root.hex2VioGuitar
                visible: root.instType === 6 && root.currentTab === 3
            }
            VioGuitarEqTab {
                anchors.fill: parent; hex1: root.hex1; hex2: root.hex2VioGuitar
                visible: root.instType === 6 && root.currentTab === 4
            }

            // POLY FX (type 7)
            PolyFxTab {
                anchors.fill: parent; hex1: root.hex1; hex2: root.hex2PolyFx
                visible: root.instType === 7 && root.currentTab === 2
            }
        }
    }
}
