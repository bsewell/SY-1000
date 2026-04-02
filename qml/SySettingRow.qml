import QtQuick

Item {
    id: root

    property string label: ""
    property bool tall: false
    default property alias control: controlContainer.children

    width: parent ? parent.width : 400
    height: root.tall ? SyTheme.settingRowTallH : SyTheme.settingRowH

    Text {
        x: SyTheme.sysLabelX
        anchors.verticalCenter: parent.verticalCenter
        text: root.label
        color: SyTheme.textSysLabel
        font.pixelSize: SyTheme.fontBody
        font.family: SyTheme.fontFamily
    }

    Item {
        id: controlContainer
        x: SyTheme.sysControlX
        y: 8
        width: parent.width - SyTheme.sysControlX - SyTheme.panelPadding
        height: parent.height - 16
    }
}
