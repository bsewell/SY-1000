import QtQuick

SyPanelBase {
    id: panel
    hex1: "04"; hex2: "00"
    accentColor: Qt.rgba(0.6, 0.2, 0.8, 1)
    title: "MAIN OUT STEREO EQ"

    Column {
        width: parent.width
        spacing: 4
        topPadding: 8

        ParaEqGraph {
            id: eqGraph
            width: parent.width - 24; height: 200; x: SyTheme.panelPadding
            hex0: "00"; hex1: panel.hex1; hex2: "00"; baseHex3: "02"
        }

        Rectangle { width: parent.width - 24; height: 1; color: SyTheme.bgControl; x: SyTheme.panelPadding }

        ParaEqBands {
            width: parent.width
            hex0: "00"; hex1: panel.hex1; hex2: "00"; baseHex3: "02"
            eqGraph: eqGraph
        }
    }
}
