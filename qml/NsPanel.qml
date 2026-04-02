import QtQuick

SyPanelBase {
    id: panel
    hex1: "00"; hex2: "39"
    accentColor: Qt.rgba(0, 0.7, 0.2, 1)
    title: "NOISE SUPPRESSOR"
    powerHex0: "10"; powerHex1: hex1; powerHex2: hex2; powerHex3: "00"

    Flow {
        x: SyTheme.panelPadding; y: SyTheme.sectionPadding
        width: parent.width - 2 * SyTheme.panelPadding
        spacing: SyTheme.flowSpacing

        FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "01"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
        FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "02"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
        SyComboBox { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "03" }
    }
}
