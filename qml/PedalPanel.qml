import QtQuick

Rectangle {
    id: root
    color: SyTheme.bgPanel
    implicitWidth: SyTheme.panelWidth
    implicitHeight: SyTheme.panelHeight

    property string hex1: "00"
    property string hex2: "00"
    property color accentColor: Qt.rgba(0.4, 0.6, 0.8, 1)

    // hex1 = "00" (guitar) or "02" (bass) for patch data (hex0="10")
    // sys1 = "01" (guitar) or "08" (bass) for system data (hex0="00")
    property string sysHex1: (hex1 === "00") ? "01" : "08"

    property int currentIndex: 0

    ListModel {
        id: sidebarModel
        ListElement { label: "NUMBERS 1-4";   source: "pedal/PdlNumbers.qml" }
        ListElement { label: "BANK / CURRENT"; source: "pedal/PdlBanks.qml" }
        ListElement { label: "CTL 1-6";       source: "pedal/PdlCtl.qml" }
        ListElement { label: "EXP / HOLD";    source: "pedal/PdlExp.qml" }
        ListElement { label: "GK / SWITCH";   source: "pedal/PdlGk.qml" }
        ListElement { label: "LEDs";          source: "pedal/PdlLeds.qml" }
    }

    Row {
        anchors.fill: parent
        spacing: 0

        Column {
            width: 200
            height: parent.height

            Rectangle {
                width: parent.width; height: 32
                color: "#442200"
                Text {
                    anchors.centerIn: parent
                    text: "PEDAL / GK"
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

                delegate: Rectangle {
                    width: sidebarList.width; height: 32
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
                        font.pixelSize: SyTheme.fontLabel; font.family: SyTheme.fontFamily
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

        Item {
            width: parent.width - 201
            height: parent.height

            Loader {
                id: contentLoader
                anchors.fill: parent
                source: sidebarModel.get(root.currentIndex).source

                property string panelHex1: root.hex1
                property string panelSysHex1: root.sysHex1
            }
        }
    }
}
