import QtQuick
import ".."

// Harmonizer complex view — matches Boss Tone Studio layout
// Row 0: VOICE selector bar
// Flow: HR1 controls (Harmony combo, Level, Pre-Delay, Feedback)
// Flow: HR2 controls (Harmony combo, Level, Pre-Delay) — visible for 2-voice modes
// Flow: Shared controls (Direct Level)
// Section: User Harmony 1 — 12 note combos (visible when HR1 != User)
// Section: User Harmony 2 — 12 note combos (visible when HR2 != User)
Item {
    id: root
    property string hex1: "00"
    property string hex2: "4D"

    property int harmMode: 0

    Component.onCompleted: {
        harmMode = paramBridge.getValue("10", hex1, hex2, "00")
    }

    Connections {
        target: harmModeCombo
        function onValueChanged() { root.harmMode = harmModeCombo.value }
    }

    Column {
        anchors.fill: parent
        spacing: 0

        // VOICE selector bar
        Rectangle {
            width: parent.width
            height: SyTheme.modeSelectorH
            color: SyTheme.bgControl

            Row {
                anchors.fill: parent
                anchors.leftMargin: 12
                spacing: 8

                Text {
                    width: SyTheme.selectorLabelW
                    text: "VOICE"
                    color: SyTheme.textDimmed
                    font.pixelSize: SyTheme.fontLabel
                    font.family: SyTheme.fontFamily
                    font.capitalization: Font.AllUppercase
                    anchors.verticalCenter: parent.verticalCenter
                }

                SyComboBox {
                    id: harmModeCombo
                    hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "00"
                    labelWidth: 0
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }

        Rectangle { width: parent.width; height: 1; color: SyTheme.divider }

        // Scrollable knob content
        Flickable {
            width: parent.width
            height: parent.height - SyTheme.modeSelectorH - 1
            contentHeight: knobCol.height + 2 * SyTheme.panelPadding
            clip: true
            interactive: contentHeight > height

            Column {
                id: knobCol
                width: parent.width
                spacing: 8
                topPadding: SyTheme.panelPadding

                // HR1 controls — always visible
                Flow {
                    width: parent.width - 2 * SyTheme.panelPadding
                    x: SyTheme.panelPadding
                    spacing: SyTheme.flowSpacingSm

                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "01"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "03"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "04"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "08"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "09"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                }

                // HR2 controls — visible for 2-voice modes (1 = 2 MONO, 2 = 2 STEREO)
                Flow {
                    width: parent.width - 2 * SyTheme.panelPadding
                    x: SyTheme.panelPadding
                    spacing: SyTheme.flowSpacingSm
                    visible: root.harmMode >= 1

                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "02"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0A"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0B"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                }

                Rectangle { width: parent.width - 2 * SyTheme.panelPadding; height: 1; color: SyTheme.divider; x: SyTheme.panelPadding }

                // User Harmony 1
                Text {
                    text: "USER HARMONY 1"
                    color: SyTheme.textSection
                    font.pixelSize: 10
                    font.family: SyTheme.fontFamily
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Flow {
                    width: parent.width - 2 * SyTheme.panelPadding
                    x: SyTheme.panelPadding
                    spacing: 4

                    Repeater {
                        model: 12
                        SyComboBox {
                            hex0: "10"; hex1: root.hex1; hex2: root.hex2
                            hex3: {
                                var v = 0x0F + index
                                var h = v.toString(16).toUpperCase()
                                return h.length < 2 ? "0" + h : h
                            }
                            labelWidth: 0
                            implicitWidth: 100
                        }
                    }
                }

                // User Harmony 2
                Text {
                    text: "USER HARMONY 2"
                    color: SyTheme.textSection
                    font.pixelSize: 10
                    font.family: SyTheme.fontFamily
                    anchors.horizontalCenter: parent.horizontalCenter
                    visible: root.harmMode >= 1
                }

                Flow {
                    width: parent.width - 2 * SyTheme.panelPadding
                    x: SyTheme.panelPadding
                    spacing: 4
                    visible: root.harmMode >= 1

                    Repeater {
                        model: 12
                        SyComboBox {
                            hex0: "10"; hex1: root.hex1; hex2: root.hex2
                            hex3: {
                                var v = 0x1B + index
                                var h = v.toString(16).toUpperCase()
                                return h.length < 2 ? "0" + h : h
                            }
                            labelWidth: 0
                            implicitWidth: 100
                        }
                    }
                }
            }
        }
    }
}
