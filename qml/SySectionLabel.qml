import QtQuick

Column {
    id: root

    property string text: ""
    property bool showDivider: true
    property bool isHeader: false

    width: parent ? parent.width : 400
    spacing: 0

    // Optional divider above (for non-header section breaks)
    Rectangle {
        visible: root.showDivider && !root.isHeader
        width: parent.width - 2 * SyTheme.panelPadding
        height: 1
        x: SyTheme.panelPadding
        color: SyTheme.divider
    }

    // Section header bar (dark blue) or inline label
    Rectangle {
        width: parent.width
        height: root.isHeader ? SyTheme.modeSelectorH : 24
        color: root.isHeader ? SyTheme.bgSectionHead : "transparent"

        Text {
            x: root.isHeader ? SyTheme.sysSectionLabelX : SyTheme.panelPadding
            anchors.verticalCenter: parent.verticalCenter
            text: root.text
            color: root.isHeader ? SyTheme.textPrimary : SyTheme.textSection
            font.pixelSize: root.isHeader ? SyTheme.fontSectionHead : SyTheme.fontSmall
            font.family: SyTheme.fontFamily
            font.capitalization: root.isHeader ? Font.MixedCase : Font.AllUppercase
        }
    }
}
