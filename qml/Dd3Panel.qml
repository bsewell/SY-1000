import QtQuick

SyPanelBase {
    id: panel
    hex1: "00"; hex2: "3E"
    accentColor: Qt.rgba(0.85, 0.85, 0.85, 1)
    title: "MASTER DELAY"
    powerHex0: "10"; powerHex1: hex1; powerHex2: hex2; powerHex3: "00"
    showHeader: false

    Column {
        width: parent.width
        spacing: 0

        StompHeader {
            width: parent.width
            accentColor: panel.accentColor
            title: "MASTER DELAY"
            powerHex0: "10"; powerHex1: panel.hex1; powerHex2: panel.hex2; powerHex3: "00"
        }

        Rectangle { width: parent.width; height: 1; color: SyTheme.divider }

        SyModeSelector {
            label: "TYPE"
            hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "01"
        }

        Flickable {
            width: parent.width
            height: panel.height - SyTheme.headerHeight - 1 - SyTheme.modeSelectorH - 1
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

                    FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "02"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "06"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "07"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "1F"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "18"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "19"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                }

                Rectangle { width: parent.width - 2 * SyTheme.panelPadding; height: 1; color: SyTheme.divider; x: SyTheme.panelPadding }

                // --- Row 2: D1 TIME, D1 FEEDBACK, D1 HIGH CUT, D1 EFFECT LEVEL,
                //            D2 TIME, D2 FEEDBACK, D2 HIGH CUT, D2 EFFECT LEVEL ---
                Flow {
                    width: parent.width - 2 * SyTheme.panelPadding
                    x: SyTheme.panelPadding
                    spacing: SyTheme.flowSpacingSm

                    FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "0A"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "0E"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "0F"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "10"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "11"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "15"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "16"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "17"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                }

                Rectangle { width: parent.width - 2 * SyTheme.panelPadding; height: 1; color: SyTheme.divider; x: SyTheme.panelPadding }

                // --- Row 3: TRIGGER, RISE TIME, FALL TIME, FADE TIME,
                //            EFFECT LEVEL, DIRECT LEVEL, CARRY OVER ---
                Flow {
                    width: parent.width - 2 * SyTheme.panelPadding
                    x: SyTheme.panelPadding
                    spacing: SyTheme.flowSpacingSm

                    FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "1A"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "1C"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "1D"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "1E"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "08"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "09"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "20"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                }
            }
        }
    }
}
