import QtQuick

SyPanelBase {
    id: panel
    hex1: "00"; hex2: "12"
    accentColor: Qt.rgba(0.5, 0.5, 0.5, 1)
    title: "FOOT VOLUME 1"

    Flow {
        x: SyTheme.panelPadding; y: SyTheme.sectionPadding
        width: parent.width - 2 * SyTheme.panelPadding
        spacing: SyTheme.flowSpacing

        FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: "12"; hex3: "06"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
        FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: "12"; hex3: "0A"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
        FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: "12"; hex3: "0E"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
        SyComboBox { hex0: "10"; hex1: panel.hex1; hex2: "12"; hex3: "12" }
    }
}
