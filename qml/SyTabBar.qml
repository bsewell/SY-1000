import QtQuick

Column {
    id: root

    property var tabs: []           // [ "TAB 1", "TAB 2", ... ]
    property int currentIndex: 0
    property color activeColor: SyTheme.bgSectionHead
    property color activeIndicator: "#66aacc"

    width: parent ? parent.width : 400
    spacing: 0

    Rectangle {
        width: parent.width
        height: SyTheme.modeSelectorH
        color: "#222222"

        Row {
            anchors.fill: parent
            spacing: 0

            Repeater {
                model: root.tabs

                Rectangle {
                    width: root.width / root.tabs.length
                    height: SyTheme.modeSelectorH
                    color: root.currentIndex === index ? root.activeColor : "#222222"

                    Text {
                        anchors.centerIn: parent
                        text: modelData
                        color: root.currentIndex === index ? SyTheme.textPrimary : SyTheme.textDimmed
                        font.pixelSize: SyTheme.fontLabel
                        font.family: SyTheme.fontFamily
                        font.bold: root.currentIndex === index
                    }

                    // Underline indicator
                    Rectangle {
                        anchors.bottom: parent.bottom
                        width: parent.width
                        height: 2
                        color: root.currentIndex === index ? root.activeIndicator : "transparent"
                    }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: root.currentIndex = index
                    }
                }
            }
        }
    }

    Rectangle { width: parent.width; height: 1; color: SyTheme.divider }
}
