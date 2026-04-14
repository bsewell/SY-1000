import QtQuick

Rectangle {
    id: panel
    color: SyTheme.bgPanel
    property string hex1: "00"
    property string hex2: "3D"
    property color accentColor: Qt.rgba(0.85, 0.85, 0.85, 1)
    property int powerValue: 0
    Component.onCompleted: { powerValue = paramBridge.getValue("10", hex1, hex2, "00") }

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
