import QtQuick

SyPanelBase {
    id: panel
    hex1: "00"; hex2: "3B"
    accentColor: Qt.rgba(0, 0.7, 0.2, 1)
    title: "EQ 2"
    powerHex0: "10"; powerHex1: hex1; powerHex2: hex2; powerHex3: "00"

    Column {
        width: parent.width
        spacing: 4
        topPadding: 8

        ParaEqGraph {
            id: eqGraph
            width: parent.width - 24; height: 200; x: SyTheme.panelPadding
            hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; baseHex3: "01"
        }

        Rectangle { width: parent.width - 24; height: 1; color: SyTheme.bgControl; x: SyTheme.panelPadding }

        ParaEqBands {
            width: parent.width
            hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; baseHex3: "01"
            eqGraph: eqGraph
        }
    }
}
