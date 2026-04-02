import QtQuick

SyPanelBase {
    id: panel
    hex1: "00"; hex2: "38"
    accentColor: Qt.rgba(0.9, 0.8, 0.0, 1)
    title: "AMP"
    powerHex0: "10"; powerHex1: hex1; powerHex2: hex2; powerHex3: "00"

    Column {
        width: parent.width
        spacing: 8
        topPadding: SyTheme.panelPadding

        SySectionLabel { text: "AMP"; showDivider: false }

        Flow {
            width: parent.width - 24; x: SyTheme.panelPadding; spacing: SyTheme.flowSpacingSm
            SyComboBox { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "01" }
            FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "05"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
            SySwitch { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "0A" }
            SySwitch { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "0C" }
            FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "0D"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
        }

        SySectionLabel { text: "TONE" }

        Flow {
            width: parent.width - 24; x: SyTheme.panelPadding; spacing: SyTheme.flowSpacingSm
            SyComboBox { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "0B" }
            FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "02"; filmstrip: "knobs/knob_48_gain.png"; frameSize: SyTheme.knobSmall }
            FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "03"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
            FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "04"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
            FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "06"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
            FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "07"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
            FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "08"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
            FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "09"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
        }

        SySectionLabel { text: "SPEAKER" }

        Flow {
            width: parent.width - 24; x: SyTheme.panelPadding; spacing: SyTheme.flowSpacingSm
            SyComboBox { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "0E" }
            SyComboBox { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "0F" }
            SyComboBox { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "10" }
            FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "11"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
            FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "12"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
            FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "13"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
        }
    }
}
