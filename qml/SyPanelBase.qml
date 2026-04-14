import QtQuick

Rectangle {
    id: root

    property string hex1: "00"
    property string hex2: "00"
    property color accentColor: Qt.rgba(0.45, 0.45, 0.45, 1)
    property string title: ""
    property string powerHex0: ""
    property string powerHex1: ""
    property string powerHex2: ""
    property string powerHex3: ""
    property bool showHeader: true

    // Power state forwarded from C++ editWindow::setPowerState()
    property int powerValue: 0

    // Content goes here via default property
    default property alias content: contentArea.children

    color: SyTheme.bgPanel
    implicitWidth: SyTheme.panelWidth
    implicitHeight: SyTheme.panelHeight

    Column {
        anchors.fill: parent
        spacing: 0

        // Header (optional)
        StompHeader {
            visible: root.showHeader && root.title !== ""
            width: parent.width
            accentColor: root.accentColor
            title: root.title
            powerHex0: root.powerHex0
            powerHex1: root.powerHex1
            powerHex2: root.powerHex2
            powerHex3: root.powerHex3
        }

        Rectangle {
            visible: root.showHeader && root.title !== ""
            width: parent.width; height: 1; color: SyTheme.divider
        }

        // Scrollable content area
        Flickable {
            id: flickable
            width: parent.width
            height: parent.height - (root.showHeader && root.title !== "" ? SyTheme.headerHeight + 1 : 0)
            contentHeight: contentArea.childrenRect.height + 2 * SyTheme.sectionPadding
            clip: true
            interactive: contentHeight > height

            Item {
                id: contentArea
                width: parent.width
            }
        }
    }
}
