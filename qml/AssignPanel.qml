import QtQuick
import QtQuick.Controls

Rectangle {
    id: root
    color: SyTheme.bgPanel
    implicitWidth: SyTheme.panelWidth
    implicitHeight: SyTheme.panelHeight

    property string hex1: "00"
    property string hex2: "00"
    property color accentColor: Qt.rgba(0.8, 0.5, 0.2, 1)

    property int currentIndex: Math.max(0, Math.min(15, parseInt(hex2, 16)))

    // Assign address table: each assign has a hex2 and base offset
    // Assigns 1-2: hex2=04, offsets 00/40
    // Assigns 3-4: hex2=05, offsets 00/40
    // ... up to Assigns 15-16: hex2=0B, offsets 00/40
    readonly property var assignAddresses: [
        { hex2: "04", base: "00" },  // Assign 1
        { hex2: "04", base: "40" },  // Assign 2
        { hex2: "05", base: "00" },  // Assign 3
        { hex2: "05", base: "40" },  // Assign 4
        { hex2: "06", base: "00" },  // Assign 5
        { hex2: "06", base: "40" },  // Assign 6
        { hex2: "07", base: "00" },  // Assign 7
        { hex2: "07", base: "40" },  // Assign 8
        { hex2: "08", base: "00" },  // Assign 9
        { hex2: "08", base: "40" },  // Assign 10
        { hex2: "09", base: "00" },  // Assign 11
        { hex2: "09", base: "40" },  // Assign 12
        { hex2: "0A", base: "00" },  // Assign 13
        { hex2: "0A", base: "40" },  // Assign 14
        { hex2: "0B", base: "00" },  // Assign 15
        { hex2: "0B", base: "40" }   // Assign 16
    ]

    ListModel {
        id: sidebarModel
        ListElement { label: "ASSIGN 1" }
        ListElement { label: "ASSIGN 2" }
        ListElement { label: "ASSIGN 3" }
        ListElement { label: "ASSIGN 4" }
        ListElement { label: "ASSIGN 5" }
        ListElement { label: "ASSIGN 6" }
        ListElement { label: "ASSIGN 7" }
        ListElement { label: "ASSIGN 8" }
        ListElement { label: "ASSIGN 9" }
        ListElement { label: "ASSIGN 10" }
        ListElement { label: "ASSIGN 11" }
        ListElement { label: "ASSIGN 12" }
        ListElement { label: "ASSIGN 13" }
        ListElement { label: "ASSIGN 14" }
        ListElement { label: "ASSIGN 15" }
        ListElement { label: "ASSIGN 16" }
    }

    Row {
        anchors.fill: parent
        spacing: 0

        // Sidebar
        Column {
            width: 160
            height: parent.height

            Rectangle {
                width: parent.width; height: 32
                color: SyTheme.bgSectionHead
                Text {
                    anchors.centerIn: parent
                    text: "ASSIGN"
                    color: "#ffcc00"
                    font.pixelSize: 13; font.family: SyTheme.fontFamily
                    font.bold: true; font.capitalization: Font.AllUppercase
                }
            }

            Rectangle { width: parent.width; height: 1; color: SyTheme.divider }

            ListView {
                id: sidebarList
                width: parent.width
                height: parent.height - 33
                model: sidebarModel
                clip: true
                currentIndex: root.currentIndex
                ScrollBar.vertical: ScrollBar { policy: ScrollBar.AsNeeded }

                delegate: Rectangle {
                    width: sidebarList.width; height: 28
                    color: index === root.currentIndex ? SyTheme.divider : mouseArea.containsMouse ? SyTheme.bgControl : "transparent"

                    Rectangle {
                        width: 3; height: parent.height
                        color: root.accentColor
                        visible: index === root.currentIndex
                    }

                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        x: 12
                        text: model.label
                        color: index === root.currentIndex ? SyTheme.textPrimary : SyTheme.textLabel
                        font.pixelSize: 11; font.family: SyTheme.fontFamily
                        font.bold: index === root.currentIndex
                    }

                    MouseArea {
                        id: mouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: root.currentIndex = index
                    }
                }
            }
        }

        Rectangle { width: 1; height: parent.height; color: SyTheme.divider }

        // Content area
        Item {
            width: parent.width - 161
            height: parent.height

            Loader {
                id: contentLoader
                anchors.fill: parent
                source: "assign/AssignSlot.qml"

                property string panelHex1: root.hex1
                property string assignHex2: root.assignAddresses[root.currentIndex].hex2
                property string assignBase: root.assignAddresses[root.currentIndex].base
                property int assignNumber: root.currentIndex + 1
            }
        }
    }
}
