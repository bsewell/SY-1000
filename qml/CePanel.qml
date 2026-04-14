import QtQuick

SyPanelBase {
    id: panel
    hex1: "00"; hex2: "3F"
    accentColor: Qt.rgba(0, 0.7, 0.8, 1)
    title: "CHORUS"
    powerHex0: "10"; powerHex1: hex1; powerHex2: hex2; powerHex3: "00"
    showHeader: false   // We need mode selector between header and content

    property int ceMode: 0
    property var modeCombo: null

    Component.onCompleted: {
        ceMode = paramBridge.getValue("10", hex1, hex2, "01")
    }

    Column {
        Connections {
            target: modeCombo
            function onValueChanged() { panel.ceMode = modeCombo.value }
        }
        width: parent.width
        spacing: 0

        StompHeader {
            width: parent.width
            accentColor: panel.accentColor
            title: "CHORUS"
            powerHex0: "10"; powerHex1: panel.hex1; powerHex2: panel.hex2; powerHex3: "00"
        }

        Rectangle { width: parent.width; height: 1; color: SyTheme.divider }

        SyModeSelector {
            hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "01"
            Component.onCompleted: modeCombo = combo
        }

        Flickable {
            width: parent.width
            height: panel.height - SyTheme.headerHeight - 1 - SyTheme.modeSelectorH - 1
            contentHeight: contentCol.height + 24
            clip: true
            interactive: contentHeight > height

            Column {
                id: contentCol
                width: parent.width
                spacing: SyTheme.flowSpacingSm
                topPadding: SyTheme.sectionPadding

                Flow {
                    width: parent.width - 16; x: 8; spacing: 8
                    FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "02"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "03"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "04"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "05"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "06"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "07"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "08"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "18"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "09"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                }

                // Dual chorus rows (modes 3+ = STEREO2, etc.)
                Column {
                    width: parent.width; spacing: SyTheme.flowSpacingSm
                    visible: panel.ceMode >= 3

                    Rectangle { width: parent.width - 16; height: 1; color: SyTheme.divider; x: 8 }

                    Flow {
                        width: parent.width - 24; x: SyTheme.panelPadding; spacing: SyTheme.flowSpacingSm
                        FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "0A"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                        FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "0B"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                        FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "0C"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                        FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "0D"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                        FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "0E"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                        FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "0F"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                        FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "10"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                    }

                    Flow {
                        width: parent.width - 24; x: SyTheme.panelPadding; spacing: SyTheme.flowSpacingSm
                        FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "11"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                        FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "12"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                        FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "13"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                        FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "14"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                        FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "15"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                        FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "16"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                        FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "17"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                    }
                }
            }
        }
    }
}
