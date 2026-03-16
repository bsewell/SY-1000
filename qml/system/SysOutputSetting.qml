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
            x: 0
            height: 32
            spacing: 0

            Repeater {
                model: ["MAIN OUT", "SUB OUT"]

                Rectangle {
                    width: 140
                    height: 32
                    color: outputRoot.currentTab === index ? "#333333" : mouseArea.containsMouse ? "#2a2a2a" : "#222222"

                    Rectangle {
                        width: parent.width
                        height: 2
                        anchors.bottom: parent.bottom
                        color: outputRoot.currentTab === index ? "#66aacc" : "transparent"
                    }

                    Text {
                        anchors.centerIn: parent
                        text: modelData
                        color: outputRoot.currentTab === index ? "#ffffff" : "#888888"
                        font.pixelSize: 11
                        font.family: "Roboto Condensed"
                        font.bold: outputRoot.currentTab === index
                    }

                    MouseArea {
                        id: mouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: outputRoot.currentTab = index
                    }
                }
            }
        }

        Rectangle { width: parent.width; height: 1; color: "#333333" }

        // Content
        Loader {
            width: parent.width
            height: parent.height - 33
            sourceComponent: outputRoot.currentTab === 0 ? mainOutComponent : subOutComponent
        }
    }

    // MAIN OUT
    Component {
        id: mainOutComponent

        Flickable {
            contentHeight: mainCol.height + 24
            clip: true

            Column {
                id: mainCol
                width: parent.width
                spacing: 8
                topPadding: 12

                // Output Select + Stereo Link + Phase
                Row {
                    x: 24; spacing: 16
                    SyComboBox { hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; hex3: "00" }
                    SyComboBox { hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; hex3: "01" }
                    SyComboBox { hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; hex3: "36" }
                    SyComboBox { hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; hex3: "34" }
                }

                // Output Levels
                Row {
                    x: 24; spacing: 24
                    FilmstripKnob {
                        hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; hex3: "2C"
                        filmstrip: "knobs/knob_56.png"; frameSize: 56
                    }
                    FilmstripKnob {
                        hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; hex3: "2E"
                        filmstrip: "knobs/knob_56.png"; frameSize: 56
                    }
                }

                Item { width: 1; height: 8 }
                Rectangle { width: parent.width - 32; height: 1; color: "#2a2a2a"; x: 16 }
                Item { width: 1; height: 4 }

                // EQ L
                Text {
                    x: 16; text: "EQ L"
                    color: "#888888"; font.pixelSize: 11
                    font.family: "Roboto Condensed"; font.capitalization: Font.AllUppercase
                }

                ParaEqGraph {
                    id: mainEqGraphL
                    width: parent.width - 24; height: 160; x: 12
                    hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; baseHex3: "02"
                }

                ParaEqBands {
                    width: parent.width
                    hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; baseHex3: "02"
                    eqGraph: mainEqGraphL
                }

                Item { width: 1; height: 8 }
                Rectangle { width: parent.width - 32; height: 1; color: "#2a2a2a"; x: 16 }
                Item { width: 1; height: 4 }

                // EQ R
                Text {
                    x: 16; text: "EQ R"
                    color: "#888888"; font.pixelSize: 11
                    font.family: "Roboto Condensed"; font.capitalization: Font.AllUppercase
                }

                ParaEqGraph {
                    id: mainEqGraphR
                    width: parent.width - 24; height: 160; x: 12
                    hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; baseHex3: "0C"
                }

                ParaEqBands {
                    width: parent.width
                    hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; baseHex3: "0C"
                    eqGraph: mainEqGraphR
                }
            }
        }
    }

    // SUB OUT
    Component {
        id: subOutComponent

        Flickable {
            contentHeight: subCol.height + 24
            clip: true

            Column {
                id: subCol
                width: parent.width
                spacing: 8
                topPadding: 12

                // Output Select + Phase
                Row {
                    x: 24; spacing: 16
                    SyComboBox { hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; hex3: "16" }
                    SyComboBox { hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; hex3: "17" }
                    SyComboBox { hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; hex3: "35" }
                }

                // Output Levels
                Row {
                    x: 24; spacing: 24
                    FilmstripKnob {
                        hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; hex3: "30"
                        filmstrip: "knobs/knob_56.png"; frameSize: 56
                    }
                    FilmstripKnob {
                        hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; hex3: "32"
                        filmstrip: "knobs/knob_56.png"; frameSize: 56
                    }
                }

                Item { width: 1; height: 8 }
                Rectangle { width: parent.width - 32; height: 1; color: "#2a2a2a"; x: 16 }
                Item { width: 1; height: 4 }

                // EQ L
                Text {
                    x: 16; text: "EQ L"
                    color: "#888888"; font.pixelSize: 11
                    font.family: "Roboto Condensed"; font.capitalization: Font.AllUppercase
                }

                ParaEqGraph {
                    id: subEqGraphL
                    width: parent.width - 24; height: 160; x: 12
                    hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; baseHex3: "18"
                }

                ParaEqBands {
                    width: parent.width
                    hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; baseHex3: "18"
                    eqGraph: subEqGraphL
                }

                Item { width: 1; height: 8 }
                Rectangle { width: parent.width - 32; height: 1; color: "#2a2a2a"; x: 16 }
                Item { width: 1; height: 4 }

                // EQ R
                Text {
                    x: 16; text: "EQ R"
                    color: "#888888"; font.pixelSize: 11
                    font.family: "Roboto Condensed"; font.capitalization: Font.AllUppercase
                }

                ParaEqGraph {
                    id: subEqGraphR
                    width: parent.width - 24; height: 160; x: 12
                    hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; baseHex3: "22"
                }

                ParaEqBands {
                    width: parent.width
                    hex0: "00"; hex1: outputRoot.panelHex1b; hex2: "00"; baseHex3: "22"
                    eqGraph: subEqGraphR
                }
            }
        }
    }
}
