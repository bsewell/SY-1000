import QtQuick
import ".."

Item {
    id: outputRoot
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
                    color: outputRoot.currentTab === index ? "#313A47" : "#222222"
                    Rectangle {
                        width: parent.width; height: 2
                        anchors.bottom: parent.bottom
                        color: outputRoot.currentTab === index ? "#66aacc" : "transparent"
                    }
                    Text {
                        anchors.centerIn: parent; text: modelData
                        color: outputRoot.currentTab === index ? "#ffffff" : "#888888"
                        font.pixelSize: 12; font.family: "Roboto Condensed"
                        font.bold: outputRoot.currentTab === index
                    }
                    MouseArea { anchors.fill: parent; onClicked: outputRoot.currentTab = index }
                }
            }
        }
        Rectangle { width: parent.width; height: 1; color: "#333333" }

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
                Item {
                    width: parent.width; height: 44
                    Text { x: 32; y: 14; text: "OUTPUT SELECT L"; color: "rgba(255,255,255,0.8)"; font.pixelSize: 12; font.family: "Roboto Condensed" }
                    SyComboBox { x: 200; y: 8; hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; hex3: "00" }
                }
                Item {
                    width: parent.width; height: 44
                    Text { x: 32; y: 14; text: "OUTPUT SELECT R"; color: "rgba(255,255,255,0.8)"; font.pixelSize: 12; font.family: "Roboto Condensed" }
                    SyComboBox { x: 200; y: 8; hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; hex3: "01" }
                }
                Item {
                    width: parent.width; height: 44
                    Text { x: 32; y: 14; text: "STEREO LINK"; color: "rgba(255,255,255,0.8)"; font.pixelSize: 12; font.family: "Roboto Condensed" }
                    SyComboBox { x: 200; y: 8; hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; hex3: "36" }
                }
                Item {
                    width: parent.width; height: 44
                    Text { x: 32; y: 14; text: "PHASE L"; color: "rgba(255,255,255,0.8)"; font.pixelSize: 12; font.family: "Roboto Condensed" }
                    SyComboBox { x: 200; y: 8; hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; hex3: "34" }
                }

                // Level knobs
                Row {
                    x: 32; spacing: 32; height: 100
                    FilmstripKnob { hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; hex3: "2C"; filmstrip: "knobs/knob_56.png"; frameSize: 56 }
                    FilmstripKnob { hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; hex3: "2E"; filmstrip: "knobs/knob_56.png"; frameSize: 56 }
                }

                Rectangle { width: parent.width - 32; height: 1; color: "rgba(255,255,255,0.15)"; x: 16 }

                // EQ L
                Text { x: 16; height: 24; verticalAlignment: Text.AlignVCenter; text: "GLOBAL EQ L"; color: "rgba(255,255,255,0.8)"; font.pixelSize: 12; font.family: "Roboto Condensed" }
                ParaEqGraph { id: meqL; width: parent.width - 32; height: 140; x: 16; hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; baseHex3: "02" }
                ParaEqBands { width: parent.width; hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; baseHex3: "02"; eqGraph: meqL }

                Item { width: 1; height: 12 }
                Rectangle { width: parent.width - 32; height: 1; color: "rgba(255,255,255,0.15)"; x: 16 }

                // EQ R
                Text { x: 16; height: 24; verticalAlignment: Text.AlignVCenter; text: "GLOBAL EQ R"; color: "rgba(255,255,255,0.8)"; font.pixelSize: 12; font.family: "Roboto Condensed" }
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

                Item {
                    width: parent.width; height: 44
                    Text { x: 32; y: 14; text: "OUTPUT SELECT L"; color: "rgba(255,255,255,0.8)"; font.pixelSize: 12; font.family: "Roboto Condensed" }
                    SyComboBox { x: 200; y: 8; hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; hex3: "16" }
                }
                Item {
                    width: parent.width; height: 44
                    Text { x: 32; y: 14; text: "OUTPUT SELECT R"; color: "rgba(255,255,255,0.8)"; font.pixelSize: 12; font.family: "Roboto Condensed" }
                    SyComboBox { x: 200; y: 8; hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; hex3: "17" }
                }
                Item {
                    width: parent.width; height: 44
                    Text { x: 32; y: 14; text: "PHASE L"; color: "rgba(255,255,255,0.8)"; font.pixelSize: 12; font.family: "Roboto Condensed" }
                    SyComboBox { x: 200; y: 8; hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; hex3: "35" }
                }

                Row {
                    x: 32; spacing: 32; height: 100
                    FilmstripKnob { hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; hex3: "30"; filmstrip: "knobs/knob_56.png"; frameSize: 56 }
                    FilmstripKnob { hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; hex3: "32"; filmstrip: "knobs/knob_56.png"; frameSize: 56 }
                }

                Rectangle { width: parent.width - 32; height: 1; color: "rgba(255,255,255,0.15)"; x: 16 }

                Text { x: 16; height: 24; verticalAlignment: Text.AlignVCenter; text: "GLOBAL EQ L"; color: "rgba(255,255,255,0.8)"; font.pixelSize: 12; font.family: "Roboto Condensed" }
                ParaEqGraph { id: seqL; width: parent.width - 32; height: 140; x: 16; hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; baseHex3: "18" }
                ParaEqBands { width: parent.width; hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; baseHex3: "18"; eqGraph: seqL }

                Item { width: 1; height: 12 }
                Rectangle { width: parent.width - 32; height: 1; color: "rgba(255,255,255,0.15)"; x: 16 }

                Text { x: 16; height: 24; verticalAlignment: Text.AlignVCenter; text: "GLOBAL EQ R"; color: "rgba(255,255,255,0.8)"; font.pixelSize: 12; font.family: "Roboto Condensed" }
                ParaEqGraph { id: seqR; width: parent.width - 32; height: 140; x: 16; hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; baseHex3: "22" }
                ParaEqBands { width: parent.width; hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; baseHex3: "22"; eqGraph: seqR }
            }
        }
    }
}
