import QtQuick

Rectangle {
    id: panel
    color: SyTheme.bgPanel

    property string hex1: "00"
    property string hex2: "36"
    property color accentColor: Qt.rgba(0.2, 0.4, 1.0, 1)
    property int powerValue: 0

    Component.onCompleted: {
        powerValue = paramBridge.getValue("10", hex1, hex2, "00")
    }

    Column {
        anchors.fill: parent
        spacing: 0

        StompHeader {
            width: parent.width
            accentColor: panel.accentColor
            title: "COMPRESSOR"
            powerHex0: "10"; powerHex1: panel.hex1; powerHex2: panel.hex2; powerHex3: "00"
        }

        SyModeSelector {
            label: "TYPE"
            hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "01"
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

                Flow {
                    width: parent.width - 16; x: 8; spacing: 8
                    FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "03"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "02"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "06"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "04"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "05"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                }
            }
        }
    }
}
