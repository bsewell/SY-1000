import QtQuick

SyPanelBase {
    id: panel
    hex1: "00"; hex2: "3D"
    accentColor: Qt.rgba(0.85, 0.85, 0.85, 1)
    title: "DELAY 2"
    powerHex0: "10"; powerHex1: hex1; powerHex2: hex2; powerHex3: "00"
    showHeader: false

    Column {
        width: parent.width
        spacing: 0

        StompHeader {
            width: parent.width
            accentColor: panel.accentColor
            title: "DELAY 2"
            powerHex0: "10"; powerHex1: panel.hex1; powerHex2: panel.hex2; powerHex3: "00"
        }

        Rectangle { width: parent.width; height: 1; color: SyTheme.divider }

        Flickable {
            width: parent.width
            height: panel.height - SyTheme.headerHeight - 1
            contentHeight: contentCol.height + 2 * SyTheme.panelPadding
            clip: true
            interactive: contentHeight > height

            Column {
                id: contentCol
                width: parent.width
                spacing: SyTheme.flowSpacingSm
                topPadding: SyTheme.panelPadding

                Flow {
                    width: parent.width - 2 * SyTheme.panelPadding
                    x: SyTheme.panelPadding
                    spacing: SyTheme.flowSpacingSm

                    FilmstripKnob {
                        hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "01"
                        filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge
                    }
                    FilmstripKnob {
                        hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "05"
                        filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge
                    }
                    FilmstripKnob {
                        hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "06"
                        filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge
                    }
                    FilmstripKnob {
                        hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "07"
                        filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge
                    }
                    FilmstripKnob {
                        hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "08"
                        filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge
                    }
                    FilmstripKnob {
                        hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "09"
                        filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge
                    }
                }
            }
        }
    }
}
