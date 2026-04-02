import QtQuick

SyPanelBase {
    id: panel
    hex1: "00"; hex2: "12"
    accentColor: Qt.rgba(0.5, 0.5, 0.5, 1)
    title: "BALANCER 2"

    Flow {
        x: SyTheme.panelPadding; y: SyTheme.sectionPadding
        width: parent.width - 2 * SyTheme.panelPadding
        spacing: SyTheme.flowSpacing

        FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: "12"; hex3: "29"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
        FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: "12"; hex3: "28"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
        FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: "12"; hex3: "2A"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
    }
}
