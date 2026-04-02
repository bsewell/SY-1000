import QtQuick

SyPanelBase {
    id: panel
    hex1: "00"; hex2: "12"
    accentColor: Qt.rgba(0.5, 0.5, 0.5, 1)
    title: "FOOT VOLUME 2"

    Flow {
        x: SyTheme.panelPadding; y: SyTheme.sectionPadding
        width: parent.width - 2 * SyTheme.panelPadding
        spacing: SyTheme.flowSpacing

        FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: "12"; hex3: "13"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
        FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: "12"; hex3: "17"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
        FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: "12"; hex3: "1B"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
        SyComboBox { hex0: "10"; hex1: panel.hex1; hex2: "12"; hex3: "1F" }
    }
}
