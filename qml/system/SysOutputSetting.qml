import QtQuick
import ".."

Item {
    id: outputRoot
    anchors.fill: parent
    property string panelHex1: parent ? parent.panelHex1 : "01"
    property string panelHex1b: parent ? parent.panelHex1b : "04"

    property int currentTab: 0

    Column {
        anchors.fill: parent
        spacing: 0

        // Tab bar
        Row {
            height: 32; spacing: 0

            Repeater {
                model: ["MAIN OUT", "SUB OUT"]
                Rectangle {
                    width: outputRoot.width / 2; height: 32
                    color: outputRoot.currentTab === index ? SyTheme.bgSectionHead : "#222222"
                    Rectangle {
                        width: parent.width; height: 2
                        anchors.bottom: parent.bottom
                        color: outputRoot.currentTab === index ? "#66aacc" : "transparent"
                    }
                    Text {
                        anchors.centerIn: parent; text: modelData
                        color: outputRoot.currentTab === index ? SyTheme.textPrimary : SyTheme.textDimmed
                        font.pixelSize: SyTheme.fontBody; font.family: SyTheme.fontFamily
                        font.bold: outputRoot.currentTab === index
                    }
                    MouseArea { anchors.fill: parent; onClicked: outputRoot.currentTab = index }
                }
            }
        }
        Rectangle { width: parent.width; height: 1; color: SyTheme.divider }

        Loader {
            width: parent.width; height: parent.height - 33
            sourceComponent: outputRoot.currentTab === 0 ? mainOutComponent : subOutComponent
        }
    }

    // MAIN OUT
    Component {
        id: mainOutComponent
        Flickable {
            contentHeight: mainCol.height + 32; clip: true
            Column {
                id: mainCol; width: parent.width; spacing: 0

                // Controls row
                SySettingRow {
                    label: "OUTPUT SELECT L"
                    SyComboBox { hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; hex3: "00" }
                }
                SySettingRow {
                    label: "OUTPUT SELECT R"
                    SyComboBox { hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; hex3: "01" }
                }
                SySettingRow {
                    label: "STEREO LINK"
                    SyComboBox { hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; hex3: "36" }
                }
                SySettingRow {
                    label: "PHASE L"
                    SyComboBox { hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; hex3: "34" }
                }

                // Level knobs
                Row {
                    x: 32; spacing: 32; height: 100
                    FilmstripKnob { hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; hex3: "2C"; filmstrip: "knobs/knob_56.png"; frameSize: 56 }
                    FilmstripKnob { hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; hex3: "2E"; filmstrip: "knobs/knob_56.png"; frameSize: 56 }
                }

                Rectangle { width: parent.width - 32; height: 1; color: SyTheme.dividerLight; x: 16 }

                // EQ L
                Text { x: 16; height: 24; verticalAlignment: Text.AlignVCenter; text: "GLOBAL EQ L"; color: SyTheme.textSysLabel; font.pixelSize: SyTheme.fontBody; font.family: SyTheme.fontFamily }
                ParaEqGraph { id: meqL; width: parent.width - 32; height: 140; x: 16; hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; baseHex3: "02" }
                ParaEqBands { width: parent.width; hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; baseHex3: "02"; eqGraph: meqL }

                Item { width: 1; height: 12 }
                Rectangle { width: parent.width - 32; height: 1; color: SyTheme.dividerLight; x: 16 }

                // EQ R
                Text { x: 16; height: 24; verticalAlignment: Text.AlignVCenter; text: "GLOBAL EQ R"; color: SyTheme.textSysLabel; font.pixelSize: SyTheme.fontBody; font.family: SyTheme.fontFamily }
                ParaEqGraph { id: meqR; width: parent.width - 32; height: 140; x: 16; hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; baseHex3: "0C" }
                ParaEqBands { width: parent.width; hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; baseHex3: "0C"; eqGraph: meqR }
            }
        }
    }

    // SUB OUT
    Component {
        id: subOutComponent
        Flickable {
            contentHeight: subCol.height + 32; clip: true
            Column {
                id: subCol; width: parent.width; spacing: 0

                SySettingRow {
                    label: "OUTPUT SELECT L"
                    SyComboBox { hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; hex3: "16" }
                }
                SySettingRow {
                    label: "OUTPUT SELECT R"
                    SyComboBox { hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; hex3: "17" }
                }
                SySettingRow {
                    label: "PHASE L"
                    SyComboBox { hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; hex3: "35" }
                }

                Row {
                    x: 32; spacing: 32; height: 100
                    FilmstripKnob { hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; hex3: "30"; filmstrip: "knobs/knob_56.png"; frameSize: 56 }
                    FilmstripKnob { hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; hex3: "32"; filmstrip: "knobs/knob_56.png"; frameSize: 56 }
                }

                Rectangle { width: parent.width - 32; height: 1; color: SyTheme.dividerLight; x: 16 }

                Text { x: 16; height: 24; verticalAlignment: Text.AlignVCenter; text: "GLOBAL EQ L"; color: SyTheme.textSysLabel; font.pixelSize: SyTheme.fontBody; font.family: SyTheme.fontFamily }
                ParaEqGraph { id: seqL; width: parent.width - 32; height: 140; x: 16; hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; baseHex3: "18" }
                ParaEqBands { width: parent.width; hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; baseHex3: "18"; eqGraph: seqL }

                Item { width: 1; height: 12 }
                Rectangle { width: parent.width - 32; height: 1; color: SyTheme.dividerLight; x: 16 }

                Text { x: 16; height: 24; verticalAlignment: Text.AlignVCenter; text: "GLOBAL EQ R"; color: SyTheme.textSysLabel; font.pixelSize: SyTheme.fontBody; font.family: SyTheme.fontFamily }
                ParaEqGraph { id: seqR; width: parent.width - 32; height: 140; x: 16; hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; baseHex3: "22" }
                ParaEqBands { width: parent.width; hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; baseHex3: "22"; eqGraph: seqR }
            }
        }
    }
}
