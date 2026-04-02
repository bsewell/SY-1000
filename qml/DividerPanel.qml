import QtQuick

SyPanelBase {
    id: panel
    hex1: "00"; hex2: "12"
    accentColor: Qt.rgba(0.5, 0.5, 0.5, 1)
    title: "DIVIDER"

    Flow {
        x: SyTheme.panelPadding; y: SyTheme.sectionPadding
        width: parent.width - 2 * SyTheme.panelPadding
        spacing: SyTheme.flowSpacing

        SyComboBox { hex0: "10"; hex1: panel.hex1; hex2: "12"; hex3: "20" }
        SyComboBox { hex0: "10"; hex1: panel.hex1; hex2: "12"; hex3: "21" }
    }
}
