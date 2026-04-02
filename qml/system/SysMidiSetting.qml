import QtQuick
import ".."

Item {
    id: midiRoot
    anchors.fill: parent
    property string panelHex1: parent ? parent.panelHex1 : "01"
    property string panelHex1b: parent ? parent.panelHex1b : "04"

    property int currentTab: 0

    Column {
        anchors.fill: parent
        spacing: 0

        // Tab bar
        Row {
            height: 32
            spacing: 0

            Repeater {
                model: ["SETTING 1", "SETTING 2", "GUITAR TO MIDI"]

                Rectangle {
                    width: midiRoot.width / 3
                    height: 32
                    color: midiRoot.currentTab === index ? SyTheme.bgSectionHead : "#222222"

                    Rectangle {
                        width: parent.width; height: 2
                        anchors.bottom: parent.bottom
                        color: midiRoot.currentTab === index ? "#66aacc" : "transparent"
                    }

                    Text {
                        anchors.centerIn: parent
                        text: modelData
                        color: midiRoot.currentTab === index ? SyTheme.textPrimary : SyTheme.textDimmed
                        font.pixelSize: SyTheme.fontBody; font.family: SyTheme.fontFamily
                        font.bold: midiRoot.currentTab === index
                    }

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: midiRoot.currentTab = index
                    }
                }
            }
        }

        Rectangle { width: parent.width; height: 1; color: SyTheme.divider }

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

    // SETTING 1: label + control rows
    Component {
        id: setting1Component

        Flickable {
            contentHeight: s1Col.height + 32
            clip: true

            Column {
                id: s1Col
                width: parent.width
                spacing: 0

                Repeater {
                    model: [
                        { label: "RX CHANNEL",   hex3: "00", type: "combo" },
                        { label: "OMNI MODE",    hex3: "01", type: "switch" },
                        { label: "TX CHANNEL",   hex3: "02", type: "combo" },
                        { label: "MIDI IN THRU", hex3: "03", type: "combo" },
                        { label: "USB IN THRU",  hex3: "04", type: "combo" },
                        { label: "SYNC CLOCK",   hex3: "05", type: "combo" },
                        { label: "CLOCK OUT",    hex3: "06", type: "switch" },
                        { label: "PC OUT",       hex3: "07", type: "switch" },
                        { label: "TX PC MAP",    hex3: "08", type: "switch" },
                        { label: "RX PC MAP",    hex3: "09", type: "switch" }
                    ]

                    Item {
                        width: s1Col.width; height: modelData.type === "switch" ? 44 : 36; clip: true
                        Text {
                            x: SyTheme.sysLabelX; anchors.verticalCenter: parent.verticalCenter
                            text: modelData.label
                            color: SyTheme.textSysLabel; font.pixelSize: SyTheme.fontLabel
                            font.family: SyTheme.fontFamily
                            visible: modelData.type === "switch"
                        }
                        Loader {
                            x: modelData.type === "combo" ? 16 : SyTheme.sysControlX
                            y: modelData.type === "switch" ? 4 : 4
                            width: modelData.type === "combo" ? 420 : undefined
                            sourceComponent: modelData.type === "combo" ? midiCombo : midiSwitch
                            property string mHex3: modelData.hex3
                        }
                    }
                }
            }
        }
    }

    Component {
        id: midiCombo
        SyComboBox { width: parent ? parent.width : 380; hex0: "00"; hex1: midiRoot.panelHex1; hex2: "30"; hex3: mHex3 }
    }
    Component {
        id: midiSwitch
        SySwitch { hex0: "00"; hex1: midiRoot.panelHex1; hex2: "30"; hex3: mHex3 }
    }

    // SETTING 2: CC# combos in rows
    Component {
        id: setting2Component

        Flickable {
            contentHeight: s2Col.height + 32
            clip: true

            Column {
                id: s2Col
                width: parent.width
                spacing: 0

                Repeater {
                    model: [
                        { label: "CC# No1",       hex3: "0A" },
                        { label: "CC# No2",       hex3: "0B" },
                        { label: "CC# No3",       hex3: "0C" },
                        { label: "CC# No4",       hex3: "0D" },
                        { label: "CC# BANK DOWN", hex3: "0E" },
                        { label: "CC# BANK UP",   hex3: "0F" },
                        { label: "CC# CTL1",      hex3: "10" },
                        { label: "CC# CTL2",      hex3: "11" },
                        { label: "CC# CTL3",      hex3: "12" },
                        { label: "CC# CTL4",      hex3: "13" },
                        { label: "CC# CTL5",      hex3: "14" },
                        { label: "CC# CTL6",      hex3: "15" },
                        { label: "CC# EXP1",      hex3: "16" },
                        { label: "CC# EXP2",      hex3: "17" },
                        { label: "CC# GK VOL",    hex3: "18" },
                        { label: "CC# GK S1",     hex3: "19" },
                        { label: "CC# GK S2",     hex3: "1A" }
                    ]

                    SyComboBox {
                        x: 16; width: 420
                        hex0: "00"; hex1: midiRoot.panelHex1; hex2: "30"; hex3: modelData.hex3
                    }
                }
            }
        }
    }

    // GUITAR TO MIDI
    Component {
        id: guitarToMidiComponent

        Flickable {
            contentHeight: g2mCol.height + 32
            clip: true

            Column {
                id: g2mCol
                width: parent.width
                spacing: 0

                // On/Off switch
                SySettingRow {
                    label: "GUITAR TO MIDI"; tall: true
                    SySwitch {
                        hex0: "00"; hex1: midiRoot.panelHex1b; hex2: "21"; hex3: "00"
                    }
                }

                Rectangle { width: parent.width - 32; height: 1; color: SyTheme.dividerLight; x: 16 }

                Repeater {
                    model: [
                        { label: "PARAMETER 1", hex3: "01" },
                        { label: "PARAMETER 2", hex3: "02" },
                        { label: "PARAMETER 3", hex3: "03" },
                        { label: "PARAMETER 4", hex3: "04" },
                        { label: "PARAMETER 5", hex3: "05" }
                    ]

                    SyComboBox {
                        x: 16; width: 420
                        hex0: "00"; hex1: midiRoot.panelHex1b; hex2: "21"; hex3: modelData.hex3
                    }
                }
            }
        }
    }
}
