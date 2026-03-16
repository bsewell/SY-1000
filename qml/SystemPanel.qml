import QtQuick

Rectangle {
    id: root
    color: "#1a1a1a"
    implicitWidth: 800
    implicitHeight: 480

    property string hex1: "01"
    property string hex2: "00"
    property color accentColor: Qt.rgba(0.4, 0.6, 0.8, 1)

    // Derived hex1b for addresses that use 04/0B (output, hardware, total, usb, midi-g2m)
    property string hex1b: (hex1 === "01") ? "04" : "0B"

    property int currentIndex: 0

    ListModel {
        id: sidebarModel
        ListElement { label: "KNOB SETTING";     source: "system/SysKnobSetting.qml" }
        ListElement { label: "INPUT SETTING";    source: "system/SysInputSetting.qml" }
        ListElement { label: "OUTPUT SETTING";   source: "system/SysOutputSetting.qml" }
        ListElement { label: "TOTAL SETTING";    source: "system/SysTotalSetting.qml" }
        ListElement { label: "PLAY OPTION";      source: "system/SysPlayOption.qml" }
        ListElement { label: "HARDWARE SETTING"; source: "system/SysHardwareSetting.qml" }
        ListElement { label: "USB SETTING";      source: "system/SysUsbSetting.qml" }
        ListElement { label: "MIDI SETTING";     source: "system/SysMidiSetting.qml" }
        ListElement { label: "TUNER";            source: "TunerPanel.qml" }
        ListElement { label: "GK SET SELECT";    source: "system/SysGkSetSelect.qml" }
        ListElement { label: "NORMAL SET SELECT"; source: "system/SysNormalSetSelect.qml" }
        ListElement { label: "GUITAR TO MIDI";   source: "system/SysGuitarToMidi.qml" }
        ListElement { label: "AUTO POWER OFF";   source: "system/SysAutoPowerOff.qml" }
        ListElement { label: "OTHER";            source: "system/SysPlaceholder.qml" }
    }

    Row {
        anchors.fill: parent
        spacing: 0

        // --- Caution banner + Sidebar ---
        Column {
            width: 200
            height: parent.height

            // Caution banner
            Rectangle {
                width: parent.width
                height: 32
                color: "#442200"
                Text {
                    anchors.centerIn: parent
                    text: "SYSTEM"
                    color: "#ffcc00"
                    font.pixelSize: 13
                    font.family: "Roboto Condensed"
                    font.bold: true
                    font.capitalization: Font.AllUppercase
                }
            }

            Rectangle { width: parent.width; height: 1; color: "#333333" }

            // Sidebar list
            ListView {
                id: sidebarList
                width: parent.width
                height: parent.height - 33
                model: sidebarModel
                clip: true
                currentIndex: root.currentIndex

                delegate: Rectangle {
                    width: sidebarList.width
                    height: 32
                    color: index === root.currentIndex ? "#333333" : mouseArea.containsMouse ? "#2a2a2a" : "transparent"

                    Rectangle {
                        width: 3
                        height: parent.height
                        color: root.accentColor
                        visible: index === root.currentIndex
                    }

                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        x: 12
                        text: model.label
                        color: index === root.currentIndex ? "#ffffff" : "#aaaaaa"
                        font.pixelSize: 11
                        font.family: "Roboto Condensed"
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

        // Divider
        Rectangle { width: 1; height: parent.height; color: "#333333" }

        // --- Content area ---
        Item {
            width: parent.width - 201
            height: parent.height

            Loader {
                id: contentLoader
                anchors.fill: parent
                source: sidebarModel.get(root.currentIndex).source

                property string panelHex1: root.hex1
                property string panelHex1b: root.hex1b
            }
        }
    }
}
