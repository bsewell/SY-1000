import QtQuick
import ".."

// Harmonizer complex view — matches Boss Tone Studio layout
// Row 0: VOICE selector bar
// Flow: HR1 controls (Harmony, Level, Pre-Delay, Feedback, Key, Direct, BPM)
// Flow: HR2 controls (Harmony, Level, Pre-Delay) — always visible, dimmed in 1-voice
// Section: User Harmony 1 — 12 note knobs — always visible
// Section: User Harmony 2 — 12 note knobs — always visible
//
// KEY and BPM are shared Master params at hex1="00", hex2="12"
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

                // HR1 controls + shared Master params — always visible
                Flow {
                    width: parent.width - 2 * SyTheme.panelPadding
                    x: SyTheme.panelPadding
                    spacing: SyTheme.flowSpacingSm

                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "01"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "03"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "04"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "08"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    // KEY — shared Master param
                    FilmstripKnob { hex0: "10"; hex1: "00"; hex2: "12"; hex3: "42"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "09"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    // BPM — shared Master param (dataknob, 4 bytes at 3E-41)
                    FilmstripKnob { hex0: "10"; hex1: "00"; hex2: "12"; hex3: "3E"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                }

                // HR2 controls — always visible, dimmed in 1-voice mode
                Flow {
                    width: parent.width - 2 * SyTheme.panelPadding
                    x: SyTheme.panelPadding
                    spacing: SyTheme.flowSpacingSm
                    opacity: root.harmMode >= 1 ? 1.0 : 0.35

                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "02"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0A"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0B"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                }

                Rectangle { width: parent.width - 2 * SyTheme.panelPadding; height: 1; color: SyTheme.divider; x: SyTheme.panelPadding }

                // User Harmony 1 — always visible
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
                    spacing: SyTheme.flowSpacingSm

                    Repeater {
                        model: 12
                        FilmstripKnob {
                            hex0: "10"; hex1: root.hex1; hex2: root.hex2
                            hex3: {
                                var v = 0x0F + index
                                var h = v.toString(16).toUpperCase()
                                return h.length < 2 ? "0" + h : h
                            }
                            filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall
                        }
                    }
                }

                // User Harmony 2 — always visible
                Text {
                    text: "USER HARMONY 2"
                    color: SyTheme.textSection
                    font.pixelSize: 10
                    font.family: SyTheme.fontFamily
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Flow {
                    width: parent.width - 2 * SyTheme.panelPadding
                    x: SyTheme.panelPadding
                    spacing: SyTheme.flowSpacingSm

                    Repeater {
                        model: 12
                        FilmstripKnob {
                            hex0: "10"; hex1: root.hex1; hex2: root.hex2
                            hex3: {
                                var v = 0x1B + index
                                var h = v.toString(16).toUpperCase()
                                return h.length < 2 ? "0" + h : h
                            }
                            filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall
                        }
                    }
                }
            }
        }
    }
}
