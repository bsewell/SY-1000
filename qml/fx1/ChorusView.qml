import QtQuick
import ".."

// Chorus complex view — matches Boss Tone Studio layout
// Row 1: MODE dropdown (full-width bar)
// Row 2: Main knobs — RATE, DEPTH, PRE-DELAY, EFFECT LEVEL, WAVE FORM, LOW CUT, HIGH CUT, DIRECT LEVEL, BPM
// Row 3: Chorus 1 (dual modes only) — RATE 1..HIGH CUT 1
// Row 4: Chorus 2 (dual modes only) — RATE 2..HIGH CUT 2, OUTPUT MODE
Item {
    id: root

    property string hex1: "00"
    property string hex2: "00"

    property int choMode: 0

    Component.onCompleted: {
        choMode = paramBridge.getValue("10", hex1, hex2, "01")
    }

    Connections {
        target: modeCombo
        function onValueChanged() { root.choMode = modeCombo.value }
    }

    Column {
        anchors.fill: parent
        spacing: 0

        // MODE selector — full-width bar matching Boss layout
        Rectangle {
            width: parent.width
            height: 32
            color: SyTheme.bgControl

            Row {
                anchors.fill: parent
                anchors.leftMargin: 12
                spacing: 8

                Text {
                    width: SyTheme.selectorLabelW
                    text: "MODE"
                    color: SyTheme.textDimmed
                    font.pixelSize: SyTheme.fontLabel
                    font.family: SyTheme.fontFamily
                    font.capitalization: Font.AllUppercase
                    anchors.verticalCenter: parent.verticalCenter
                }

                SyComboBox {
                    id: modeCombo
                    hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "01"
                    labelWidth: 0
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }

        Rectangle { width: parent.width; height: 1; color: SyTheme.divider }

        // Knob content area
        Flickable {
            width: parent.width
            height: parent.height - 33
            contentHeight: knobCol.height + 24
            clip: true
            interactive: contentHeight > height

            Column {
                id: knobCol
                width: parent.width
                spacing: 12
                topPadding: 16

                // Main row (always visible): RATE, DEPTH, PRE-DELAY, EFFECT LEVEL,
                // WAVE FORM, LOW CUT, HIGH CUT, DIRECT LEVEL, BPM
                Flow {
                    width: parent.width - 16
                    x: 8
                    spacing: 8

                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "02"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "03"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "04"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "05"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "06"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "07"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "08"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "18"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "09"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                }

                // Dual chorus rows (modes 3+ = STEREO2, etc.)
                Column {
                    width: parent.width
                    spacing: 12
                    visible: root.choMode >= 3

                    Rectangle { width: parent.width - 16; height: 1; color: SyTheme.divider; x: 8 }

                    // Chorus 1: RATE 1, DEPTH 1, PRE-DELAY 1, EFFECT LEVEL 1, WAVE FORM 1, LOW CUT 1, HIGH CUT 1
                    Flow {
                        width: parent.width - 16
                        x: 8
                        spacing: 8

                        FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0A"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                        FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0B"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                        FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0C"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                        FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0D"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                        FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0E"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                        FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0F"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                        FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "10"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    }

                    // Chorus 2: RATE 2, DEPTH 2, PRE-DELAY 2, EFFECT LEVEL 2, WAVE FORM 2, LOW CUT 2, HIGH CUT 2, OUTPUT MODE
                    Flow {
                        width: parent.width - 16
                        x: 8
                        spacing: 8

                        FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "11"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                        FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "12"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                        FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "13"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                        FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "14"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                        FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "15"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                        FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "16"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                        FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "17"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    }
                }
            }
        }
    }
}
