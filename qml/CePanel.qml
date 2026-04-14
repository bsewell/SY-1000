import QtQuick

Rectangle {
    id: panel
    color: SyTheme.bgPanel

    property string hex1: "00"
    property string hex2: "3F"
    property color accentColor: Qt.rgba(0, 0.7, 0.8, 1)
    property int powerValue: 0
    property int ceMode: 0

    Component.onCompleted: {
        powerValue = paramBridge.getValue("10", hex1, hex2, "00")
        ceMode = paramBridge.getValue("10", hex1, hex2, "01")
    }

    Connections {
        target: modeCombo
        function onValueChanged() { panel.ceMode = modeCombo.value }
    }

    Column {
        anchors.fill: parent
        spacing: 0

        StompHeader {
            width: parent.width
            accentColor: panel.accentColor
            title: "CHORUS"
            powerHex0: "10"; powerHex1: panel.hex1; powerHex2: panel.hex2; powerHex3: "00"
        }

        SyModeSelector {
            id: modeSelector
            hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "01"
            property alias modeCombo: modeSelector.combo
        }

        Flickable {
            width: parent.width
            height: parent.height - SyTheme.headerHeight - SyTheme.modeSelectorH - 1
            contentHeight: knobCol.height + 24
            clip: true
            interactive: contentHeight > height

            Column {
                id: knobCol
                width: parent.width
                spacing: 12
                topPadding: 16

                // Main row
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

                // Dual chorus rows (modes 3+)
                Column {
                    width: parent.width; spacing: 12
                    visible: panel.ceMode >= 3

                    Rectangle { width: parent.width - 16; height: 1; color: SyTheme.divider; x: 8 }

                    Flow {
                        width: parent.width - 16; x: 8; spacing: 8
                        FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "0A"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                        FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "0B"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                        FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "0C"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                        FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "0D"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                        FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "0E"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                        FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "0F"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                        FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "10"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    }

                    Flow {
                        width: parent.width - 16; x: 8; spacing: 8
                        FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "11"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                        FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "12"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                        FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "13"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                        FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "14"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                        FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "15"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                        FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "16"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                        FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "17"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    }
                }
            }
        }
    }
}
