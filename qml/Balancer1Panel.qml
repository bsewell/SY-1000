import QtQuick

SyPanelBase {
    id: panel
    hex1: "00"; hex2: "12"
    accentColor: Qt.rgba(0.5, 0.5, 0.5, 1)
    title: "BALANCER 1"

    Flow {
        x: SyTheme.panelPadding; y: SyTheme.sectionPadding
        width: parent.width - 2 * SyTheme.panelPadding
        spacing: SyTheme.flowSpacing

        FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: "12"; hex3: "26"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
        FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: "12"; hex3: "25"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
        FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: "12"; hex3: "27"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
    }
}
