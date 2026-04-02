import QtQuick

SyPanelBase {
    id: panel
    hex1: "00"
    hex2: "12"
    accentColor: Qt.rgba(0.45, 0.45, 0.45, 1)
    title: "MIXER"

    Flow {
        x: SyTheme.panelPadding
        y: SyTheme.sectionPadding
        width: parent.width - 2 * SyTheme.panelPadding
        spacing: SyTheme.flowSpacing

        FilmstripKnob {
            hex0: "10"; hex1: panel.hex1; hex2: "12"; hex3: "23"
            filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge
        }

        FilmstripKnob {
            hex0: "10"; hex1: panel.hex1; hex2: "12"; hex3: "22"
            filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge
        }

        FilmstripKnob {
            hex0: "10"; hex1: panel.hex1; hex2: "12"; hex3: "24"
            filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge
        }
    }
}
