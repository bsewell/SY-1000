import QtQuick
import ".."

Item {
    id: midiRoot
    property string panelHex1: parent ? parent.panelHex1 : "01"
    property string panelHex1b: parent ? parent.panelHex1b : "04"

    property int currentTab: 0

    Column {
        anchors.fill: parent
        spacing: 0

        // Tab bar
        Row {
            x: 0
            height: 32
            spacing: 0

            Repeater {
                model: ["SETTING 1", "SETTING 2", "GUITAR TO MIDI"]

                Rectangle {
                    width: 140
                    height: 32
                    color: midiRoot.currentTab === index ? "#333333" : mouseArea.containsMouse ? "#2a2a2a" : "#222222"

                    Rectangle {
                        width: parent.width
                        height: 2
                        anchors.bottom: parent.bottom
                        color: midiRoot.currentTab === index ? "#66aacc" : "transparent"
                    }

                    Text {
                        anchors.centerIn: parent
                        text: modelData
                        color: midiRoot.currentTab === index ? "#ffffff" : "#888888"
                        font.pixelSize: 11
                        font.family: "Roboto Condensed"
                        font.bold: midiRoot.currentTab === index
                    }

                    MouseArea {
                        id: mouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: midiRoot.currentTab = index
                    }
                }
            }
        }

        Rectangle { width: parent.width; height: 1; color: "#333333" }

        // Content
        Loader {
            width: parent.width
            height: parent.height - 33
            sourceComponent: {
                switch (midiRoot.currentTab) {
                case 0: return setting1Component
                case 1: return setting2Component
                case 2: return guitarToMidiComponent
                default: return null
                }
            }
        }
    }

    // SETTING 1: 10 controls
    Component {
        id: setting1Component

        Flickable {
            contentHeight: s1Col.height + 24
            clip: true

            Column {
                id: s1Col
                width: parent.width
                spacing: 8
                topPadding: 16

                Row {
                    x: 24; spacing: 16
                    SyComboBox { hex0: "00"; hex1: midiRoot.panelHex1; hex2: "30"; hex3: "00" }
                    SySwitch   { hex0: "00"; hex1: midiRoot.panelHex1; hex2: "30"; hex3: "01" }
                }
                Row {
                    x: 24; spacing: 16
                    SyComboBox { hex0: "00"; hex1: midiRoot.panelHex1; hex2: "30"; hex3: "02" }
                    SyComboBox { hex0: "00"; hex1: midiRoot.panelHex1; hex2: "30"; hex3: "03" }
                }
                Row {
                    x: 24; spacing: 16
                    SyComboBox { hex0: "00"; hex1: midiRoot.panelHex1; hex2: "30"; hex3: "04" }
                    SyComboBox { hex0: "00"; hex1: midiRoot.panelHex1; hex2: "30"; hex3: "05" }
                }
                Row {
                    x: 24; spacing: 16
                    SySwitch   { hex0: "00"; hex1: midiRoot.panelHex1; hex2: "30"; hex3: "06" }
                    SySwitch   { hex0: "00"; hex1: midiRoot.panelHex1; hex2: "30"; hex3: "07" }
                }
                Row {
                    x: 24; spacing: 16
                    SySwitch   { hex0: "00"; hex1: midiRoot.panelHex1; hex2: "30"; hex3: "08" }
                    SySwitch   { hex0: "00"; hex1: midiRoot.panelHex1; hex2: "30"; hex3: "09" }
                }
            }
        }
    }

    // SETTING 2: 17 CC# combos
    Component {
        id: setting2Component

        Flickable {
            contentHeight: s2Col.height + 24
            clip: true

            Column {
                id: s2Col
                width: parent.width
                spacing: 4
                topPadding: 16

                Text {
                    x: 16; text: "CC# NUMBERS"
                    color: "#888888"; font.pixelSize: 11
                    font.family: "Roboto Condensed"; font.capitalization: Font.AllUppercase
                }

                Item { width: 1; height: 8 }

                // No1-No4
                Row {
                    x: 24; spacing: 16
                    SyComboBox { hex0: "00"; hex1: midiRoot.panelHex1; hex2: "30"; hex3: "0A" }
                    SyComboBox { hex0: "00"; hex1: midiRoot.panelHex1; hex2: "30"; hex3: "0B" }
                    SyComboBox { hex0: "00"; hex1: midiRoot.panelHex1; hex2: "30"; hex3: "0C" }
                    SyComboBox { hex0: "00"; hex1: midiRoot.panelHex1; hex2: "30"; hex3: "0D" }
                }

                Item { width: 1; height: 8 }

                // Bank Down/Up
                Row {
                    x: 24; spacing: 16
                    SyComboBox { hex0: "00"; hex1: midiRoot.panelHex1; hex2: "30"; hex3: "0E" }
                    SyComboBox { hex0: "00"; hex1: midiRoot.panelHex1; hex2: "30"; hex3: "0F" }
                }

                Item { width: 1; height: 8 }

                // CTL1-6
                Row {
                    x: 24; spacing: 16
                    SyComboBox { hex0: "00"; hex1: midiRoot.panelHex1; hex2: "30"; hex3: "10" }
                    SyComboBox { hex0: "00"; hex1: midiRoot.panelHex1; hex2: "30"; hex3: "11" }
                    SyComboBox { hex0: "00"; hex1: midiRoot.panelHex1; hex2: "30"; hex3: "12" }
                }
                Row {
                    x: 24; spacing: 16
                    SyComboBox { hex0: "00"; hex1: midiRoot.panelHex1; hex2: "30"; hex3: "13" }
                    SyComboBox { hex0: "00"; hex1: midiRoot.panelHex1; hex2: "30"; hex3: "14" }
                    SyComboBox { hex0: "00"; hex1: midiRoot.panelHex1; hex2: "30"; hex3: "15" }
                }

                Item { width: 1; height: 8 }

                // EXP1, EXP2, GK Vol, GK S1, GK S2
                Row {
                    x: 24; spacing: 16
                    SyComboBox { hex0: "00"; hex1: midiRoot.panelHex1; hex2: "30"; hex3: "16" }
                    SyComboBox { hex0: "00"; hex1: midiRoot.panelHex1; hex2: "30"; hex3: "17" }
                }
                Row {
                    x: 24; spacing: 16
                    SyComboBox { hex0: "00"; hex1: midiRoot.panelHex1; hex2: "30"; hex3: "18" }
                    SyComboBox { hex0: "00"; hex1: midiRoot.panelHex1; hex2: "30"; hex3: "19" }
                    SyComboBox { hex0: "00"; hex1: midiRoot.panelHex1; hex2: "30"; hex3: "1A" }
                }
            }
        }
    }

    // GUITAR TO MIDI: 1 switch + 5 combos
    Component {
        id: guitarToMidiComponent

        Column {
            spacing: 8
            topPadding: 16

            Row {
                x: 24; spacing: 16
                SySwitch { hex0: "00"; hex1: midiRoot.panelHex1b; hex2: "21"; hex3: "00" }
            }

            Item { width: 1; height: 8 }

            Row {
                x: 24; spacing: 16
                SyComboBox { hex0: "00"; hex1: midiRoot.panelHex1b; hex2: "21"; hex3: "01" }
                SyComboBox { hex0: "00"; hex1: midiRoot.panelHex1b; hex2: "21"; hex3: "02" }
                SyComboBox { hex0: "00"; hex1: midiRoot.panelHex1b; hex2: "21"; hex3: "03" }
            }

            Row {
                x: 24; spacing: 16
                SyComboBox { hex0: "00"; hex1: midiRoot.panelHex1b; hex2: "21"; hex3: "04" }
                SyComboBox { hex0: "00"; hex1: midiRoot.panelHex1b; hex2: "21"; hex3: "05" }
            }
        }
    }
}
