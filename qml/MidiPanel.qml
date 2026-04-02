import QtQuick
import QtQuick.Controls

Rectangle {
    id: root
    color: SyTheme.bgPanel
    implicitWidth: SyTheme.panelWidth
    implicitHeight: SyTheme.panelHeight

    // hex1 = sys1 ("01" guitar / "08" bass), hex2 = "30" (MIDI system block)
    property string hex1: "01"
    property string hex2: "30"

    property int currentTab: 0

    Column {
        anchors.fill: parent
        spacing: 0

        // Header
        Rectangle {
            width: parent.width; height: SyTheme.headerHeight
            color: SyTheme.bgSectionHead
            Text {
                anchors.centerIn: parent
                text: "MIDI"
                color: SyTheme.textPrimary
                font.pixelSize: 14; font.family: SyTheme.fontFamily
                font.bold: true; font.capitalization: Font.AllUppercase
            }
        }

        Rectangle { width: parent.width; height: 1; color: SyTheme.divider }

        // Tab bar
        Row {
            width: parent.width; height: SyTheme.modeSelectorH

            Repeater {
                model: ["SETTING 1", "SETTING 2"]
                Rectangle {
                    width: root.width / 2; height: SyTheme.modeSelectorH
                    color: "transparent"
                    Rectangle {
                        width: parent.width; height: 2
                        anchors.bottom: parent.bottom
                        color: root.currentTab === index ? SyTheme.accent : "transparent"
                    }
                    Text {
                        anchors.centerIn: parent
                        text: modelData
                        color: root.currentTab === index ? SyTheme.textPrimary : SyTheme.textDimmed
                        font.pixelSize: SyTheme.fontLabel; font.family: SyTheme.fontFamily
                        font.bold: root.currentTab === index
                    }
                    MouseArea { anchors.fill: parent; onClicked: root.currentTab = index }
                }
            }
        }

        Rectangle { width: parent.width; height: 1; color: SyTheme.divider }

        Loader {
            width: parent.width
            height: parent.height - SyTheme.headerHeight - 1 - SyTheme.modeSelectorH - 1
            sourceComponent: root.currentTab === 0 ? setting1 : setting2
        }
    }

    // ── SETTING 1: basic MIDI I/O ─────────────────────────────────────────
    Component {
        id: setting1
        Flickable {
            contentHeight: s1col.height + SyTheme.sectionPadding
            clip: true
            ScrollBar.vertical: ScrollBar { policy: ScrollBar.AsNeeded }
            Column {
                id: s1col
                width: parent.width; spacing: 0

                SySettingRow { label: "RX CHANNEL"
                    SyComboBox { hex0: "00"; hex1: root.hex1; hex2: root.hex2; hex3: "00" }
                }
                Rectangle { width: parent.width; height: 1; color: SyTheme.dividerLight }

                SySettingRow { label: "OMNI MODE"; tall: true
                    SySwitch { hex0: "00"; hex1: root.hex1; hex2: root.hex2; hex3: "01" }
                }
                Rectangle { width: parent.width; height: 1; color: SyTheme.dividerLight }

                SySettingRow { label: "TX CHANNEL"
                    SyComboBox { hex0: "00"; hex1: root.hex1; hex2: root.hex2; hex3: "02" }
                }
                Rectangle { width: parent.width; height: 1; color: SyTheme.dividerLight }

                SySettingRow { label: "MIDI IN THRU"
                    SyComboBox { hex0: "00"; hex1: root.hex1; hex2: root.hex2; hex3: "03" }
                }
                Rectangle { width: parent.width; height: 1; color: SyTheme.dividerLight }

                SySettingRow { label: "USB IN THRU"
                    SyComboBox { hex0: "00"; hex1: root.hex1; hex2: root.hex2; hex3: "04" }
                }
                Rectangle { width: parent.width; height: 1; color: SyTheme.dividerLight }

                SySettingRow { label: "SYNC CLOCK"
                    SyComboBox { hex0: "00"; hex1: root.hex1; hex2: root.hex2; hex3: "05" }
                }
                Rectangle { width: parent.width; height: 1; color: SyTheme.dividerLight }

                SySettingRow { label: "CLOCK OUT"; tall: true
                    SySwitch { hex0: "00"; hex1: root.hex1; hex2: root.hex2; hex3: "06" }
                }
                Rectangle { width: parent.width; height: 1; color: SyTheme.dividerLight }

                SySettingRow { label: "PC OUT"; tall: true
                    SySwitch { hex0: "00"; hex1: root.hex1; hex2: root.hex2; hex3: "07" }
                }
                Rectangle { width: parent.width; height: 1; color: SyTheme.dividerLight }

                SySettingRow { label: "TX PC MAP"; tall: true
                    SySwitch { hex0: "00"; hex1: root.hex1; hex2: root.hex2; hex3: "08" }
                }
                Rectangle { width: parent.width; height: 1; color: SyTheme.dividerLight }

                SySettingRow { label: "RX PC MAP"; tall: true
                    SySwitch { hex0: "00"; hex1: root.hex1; hex2: root.hex2; hex3: "09" }
                }
            }
        }
    }

    // ── SETTING 2: CC# assignments ───────────────────────────────────────
    Component {
        id: setting2
        Flickable {
            contentHeight: s2col.height + SyTheme.sectionPadding
            clip: true
            ScrollBar.vertical: ScrollBar { policy: ScrollBar.AsNeeded }
            Column {
                id: s2col
                width: parent.width; spacing: 0

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
                    Column {
                        width: s2col.width; spacing: 0
                        SySettingRow {
                            label: modelData.label
                            SyComboBox { hex0: "00"; hex1: root.hex1; hex2: root.hex2; hex3: modelData.hex3 }
                        }
                        Rectangle { width: parent.width; height: 1; color: SyTheme.dividerLight }
                    }
                }
            }
        }
    }
}
