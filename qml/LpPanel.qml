import QtQuick

SyPanelBase {
    id: panel
    hex1: "00"; hex2: "12"
    accentColor: Qt.rgba(0, 0.7, 0.2, 1)
    title: "SEND/RETURN"
    powerHex0: "10"; powerHex1: hex1; powerHex2: "12"; powerHex3: "2E"

    Flow {
        x: SyTheme.panelPadding; y: SyTheme.sectionPadding
        width: parent.width - 2 * SyTheme.panelPadding
        spacing: SyTheme.flowSpacing

        SyComboBox { hex0: "10"; hex1: panel.hex1; hex2: "12"; hex3: "2F" }
        FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: "12"; hex3: "30"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
        FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: "12"; hex3: "32"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
        FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: "12"; hex3: "34"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
        SyComboBox { hex0: "10"; hex1: panel.hex1; hex2: "12"; hex3: "35" }
    }
}
