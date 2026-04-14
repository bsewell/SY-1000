import QtQuick

Rectangle {
    id: panel
    color: SyTheme.bgPanel

    property string hex1: "01"
    property string hex2: "32"
    property color accentColor: Qt.rgba(0, 0.7, 0.8, 1)
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
            title: "REVERB"
            powerHex0: "10"; powerHex1: panel.hex1; powerHex2: panel.hex2; powerHex3: "00"
        }

        SyModeSelector {
            label: "TYPE"
            hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "01"
        }

        // Sub-type selector
        Rectangle {
            width: parent.width; height: SyTheme.modeSelectorH; color: SyTheme.bgControl
            Row {
                anchors.fill: parent; anchors.leftMargin: SyTheme.panelPadding; spacing: 8
                Text {
                    width: SyTheme.selectorLabelW; text: "SPRING"; color: SyTheme.textDimmed
                    font.pixelSize: SyTheme.fontLabel; font.family: SyTheme.fontFamily
                    font.capitalization: Font.AllUppercase; anchors.verticalCenter: parent.verticalCenter
                }
                SyComboBox {
                    hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "03"
                    labelWidth: 0; anchors.verticalCenter: parent.verticalCenter
                }
            }
        }

        Rectangle { width: parent.width; height: 1; color: SyTheme.divider }

        Flickable {
            width: parent.width
            height: parent.height - SyTheme.headerHeight - (SyTheme.modeSelectorH * 2) - 2
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
                    FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "04"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "02"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "0B"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "0D"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "09"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "0A"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "08"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "0C"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                }
            }
        }
    }
}
