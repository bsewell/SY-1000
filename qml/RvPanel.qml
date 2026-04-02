import QtQuick

SyPanelBase {
    id: panel
    hex1: "01"; hex2: "32"
    accentColor: Qt.rgba(0, 0.7, 0.8, 1)
    title: "REVERB"
    powerHex0: "10"; powerHex1: hex1; powerHex2: hex2; powerHex3: "00"

    Column {
        width: parent.width
        spacing: SyTheme.flowSpacingSm
        topPadding: SyTheme.panelPadding

        Flow {
            width: parent.width - 24; x: SyTheme.panelPadding; spacing: SyTheme.flowSpacingSm
            SyComboBox { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "01" }
            SyComboBox { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "03" }
        }

        Rectangle { width: parent.width - 24; height: 1; color: SyTheme.divider; x: SyTheme.panelPadding }

        Flow {
            width: parent.width - 24; x: SyTheme.panelPadding; spacing: SyTheme.flowSpacingSm
            FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "04"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
            FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "02"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
            FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "0B"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
            FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "0D"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
            FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "09"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
            FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "0A"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
        }

        Rectangle { width: parent.width - 24; height: 1; color: SyTheme.divider; x: SyTheme.panelPadding }

        Flow {
            width: parent.width - 24; x: SyTheme.panelPadding; spacing: SyTheme.flowSpacingSm
            FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "08"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
            FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "0C"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
        }
    }
}
