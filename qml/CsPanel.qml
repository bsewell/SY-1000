import QtQuick

SyPanelBase {
    id: panel
    hex1: "00"; hex2: "36"
    accentColor: Qt.rgba(0.2, 0.4, 1.0, 1)
    title: "COMPRESSOR"
    powerHex0: "10"; powerHex1: hex1; powerHex2: hex2; powerHex3: "00"

    Flow {
        x: SyTheme.panelPadding; y: SyTheme.sectionPadding
        width: parent.width - 2 * SyTheme.panelPadding
        spacing: SyTheme.flowSpacing

        SyComboBox { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "01" }
        FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "03"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
        FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "02"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
        FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "06"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
        FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "04"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
        FilmstripKnob { hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "05"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
    }
}
