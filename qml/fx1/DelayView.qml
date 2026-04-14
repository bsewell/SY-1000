import QtQuick
import ".."

// FX Delay view — Boss 3-row layout matching Master Delay
// Row 1: TIME, FEEDBACK, HIGH CUT, TAP TIME, MOD RATE, MOD DEPTH
// Row 2: D1 TIME, D1 FEEDBACK, D1 HIGH CUT, D1 EFFECT LEVEL,
//        D2 TIME, D2 FEEDBACK, D2 HIGH CUT, D2 EFFECT LEVEL
// Row 3: TRIGGER, RISE TIME, FALL TIME, FADE TIME,
//        EFFECT LEVEL, DIRECT LEVEL, CARRY OVER, BPM
Item {
    id: root
    property string hex1: "00"
    property string hex2: "48"

    Column {
        anchors.fill: parent
        spacing: 0

        // TYPE + MODE selector bar
        Rectangle {
            width: parent.width
            height: SyTheme.modeSelectorH
            color: SyTheme.bgControl

            Row {
                anchors.fill: parent
                anchors.leftMargin: SyTheme.panelPadding
                spacing: 8

                Text {
                    width: SyTheme.selectorLabelW
                    text: "TYPE"
                    color: SyTheme.textDimmed
                    font.pixelSize: SyTheme.fontLabel
                    font.family: SyTheme.fontFamily
                    font.capitalization: Font.AllUppercase
                    anchors.verticalCenter: parent.verticalCenter
                }

                SyComboBox {
                    hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "01"
                    labelWidth: 0
                    anchors.verticalCenter: parent.verticalCenter
                }

                Item { width: 16; height: 1 }

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
                    hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "1A"
                    labelWidth: 0
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }

        Rectangle { width: parent.width; height: 1; color: SyTheme.divider }

        // Knob content
        Flickable {
            width: parent.width
            height: parent.height - SyTheme.modeSelectorH - 1
            contentHeight: contentCol.height + 2 * SyTheme.panelPadding
            clip: true
            interactive: contentHeight > height

            Column {
                id: contentCol
                width: parent.width
                spacing: 8
                topPadding: SyTheme.panelPadding

                // --- Row 1: TIME, FEEDBACK, HIGH CUT, TAP TIME, MOD RATE, MOD DEPTH ---
                Flow {
                    width: parent.width - 2 * SyTheme.panelPadding
                    x: SyTheme.panelPadding
                    spacing: SyTheme.flowSpacingSm

                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "02"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "06"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "07"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "1E"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "18"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "19"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                }

                Rectangle { width: parent.width - 2 * SyTheme.panelPadding; height: 1; color: SyTheme.divider; x: SyTheme.panelPadding }

                // --- Row 2: D1/D2 TIME, FEEDBACK, HIGH CUT, EFFECT LEVEL ---
                Flow {
                    width: parent.width - 2 * SyTheme.panelPadding
                    x: SyTheme.panelPadding
                    spacing: SyTheme.flowSpacingSm

                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0A"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0E"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0F"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "10"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "11"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "15"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "16"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "17"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                }

                Rectangle { width: parent.width - 2 * SyTheme.panelPadding; height: 1; color: SyTheme.divider; x: SyTheme.panelPadding }

                // --- Row 3: TRIGGER, RISE TIME, FALL TIME, FADE TIME,
                //            EFFECT LEVEL, DIRECT LEVEL, CARRY OVER, BPM ---
                Flow {
                    width: parent.width - 2 * SyTheme.panelPadding
                    x: SyTheme.panelPadding
                    spacing: SyTheme.flowSpacingSm

                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "19"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "1B"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "1C"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "1D"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "08"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "09"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "1F"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                }
            }
        }
    }
}
