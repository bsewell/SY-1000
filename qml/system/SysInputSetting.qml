import QtQuick
import ".."

Item {
    id: inputRoot
    property string panelHex1: parent ? parent.panelHex1 : "01"
    property string panelHex1b: parent ? parent.panelHex1b : "04"

    property bool isBass: (panelHex1 === "08")
    property int currentTab: 0
    property int gkSetIndex: 0
    property int normalSetIndex: 0

    // GK Set address table — [hex1, hex2] per set (guitar mode)
    // Bass mode: hex1 += 7 (01->08, 02->09)
    function gkSetAddr(setIdx) {
        var table = [
            ["01", "40"], ["01", "50"], ["01", "60"], ["01", "70"],
            ["02", "00"], ["02", "10"], ["02", "20"], ["02", "30"],
            ["02", "40"], ["02", "50"]
        ]
        var addr = table[setIdx]
        if (isBass) {
            var h1 = parseInt(addr[0], 16) + 7
            var h = h1.toString(16).toUpperCase()
            return [h.length < 2 ? "0" + h : h, addr[1]]
        }
        return addr
    }

    // Normal Set address table — [hex1, hex2] per set (guitar mode)
    // Bass mode: hex1 += 7 (02->09, 03->0A)
    function normalSetAddr(setIdx) {
        var table = [
            ["02", "60"], ["02", "70"],
            ["03", "00"], ["03", "10"], ["03", "20"], ["03", "30"],
            ["03", "40"], ["03", "50"], ["03", "60"], ["03", "70"]
        ]
        var addr = table[setIdx]
        if (isBass) {
            var h1 = parseInt(addr[0], 16) + 7
            var h = h1.toString(16).toUpperCase()
            return [h.length < 2 ? "0" + h : h, addr[1]]
        }
        return addr
    }

    Column {
        anchors.fill: parent
        spacing: 0

        // Tab bar
        Row {
            x: 0
            height: 32
            spacing: 0

            Repeater {
                model: ["GK INPUT", "NORMAL INPUT"]

                Rectangle {
                    width: 140
                    height: 32
                    color: inputRoot.currentTab === index ? "#333333" : mouseArea.containsMouse ? "#2a2a2a" : "#222222"

                    Rectangle {
                        width: parent.width
                        height: 2
                        anchors.bottom: parent.bottom
                        color: inputRoot.currentTab === index ? "#66aacc" : "transparent"
                    }

                    Text {
                        anchors.centerIn: parent
                        text: modelData
                        color: inputRoot.currentTab === index ? "#ffffff" : "#888888"
                        font.pixelSize: 11
                        font.family: "Roboto Condensed"
                        font.bold: inputRoot.currentTab === index
                    }

                    MouseArea {
                        id: mouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: inputRoot.currentTab = index
                    }
                }
            }
        }

        Rectangle { width: parent.width; height: 1; color: "#333333" }

        // Content
        Loader {
            width: parent.width
            height: parent.height - 33
            sourceComponent: inputRoot.currentTab === 0 ? gkComponent : normalComponent
        }
    }

    // ===================== GK INPUT =====================
    Component {
        id: gkComponent

        Column {
            spacing: 0

            // Set selector
            Rectangle {
                width: parent.width
                height: 32
                color: "#222222"

                Row {
                    anchors.verticalCenter: parent.verticalCenter
                    x: 16
                    spacing: 8

                    Text {
                        text: "SETTING"
                        color: "#888888"
                        font.pixelSize: 11
                        font.family: "Roboto Condensed"
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    Repeater {
                        model: 10
                        Rectangle {
                            width: 28; height: 22
                            radius: 3
                            color: inputRoot.gkSetIndex === index ? "#66aacc" : gkMa.containsMouse ? "#444444" : "#333333"
                            Text {
                                anchors.centerIn: parent
                                text: (index + 1).toString()
                                color: inputRoot.gkSetIndex === index ? "#000000" : "#cccccc"
                                font.pixelSize: 11; font.family: "Roboto Condensed"
                                font.bold: inputRoot.gkSetIndex === index
                            }
                            MouseArea {
                                id: gkMa
                                anchors.fill: parent
                                hoverEnabled: true
                                onClicked: inputRoot.gkSetIndex = index
                            }
                        }
                    }
                }
            }

            Rectangle { width: parent.width; height: 1; color: "#333333" }

            // GK Set content — use Loader to force recreate on set change
            Loader {
                id: gkSetLoader
                width: parent.width
                height: parent.parent.height - 65
                active: true

                sourceComponent: gkSetContent

                // Force reload when set index changes
                property int setIdx: inputRoot.gkSetIndex
                onSetIdxChanged: { active = false; active = true }
            }
        }
    }

    Component {
        id: gkSetContent

        Flickable {
            id: gkFlick
            contentHeight: gkCol.height + 24
            clip: true

            property var addr: inputRoot.gkSetAddr(inputRoot.gkSetIndex)
            property string h1: addr[0]
            property string h2: addr[1]

            Column {
                id: gkCol
                width: parent.width
                spacing: 8
                topPadding: 12

                // Top row: GK Type + Scale + PU Phase + PU Direction
                Row {
                    x: 24; spacing: 16
                    SyComboBox { hex0: "00"; hex1: gkFlick.h1; hex2: gkFlick.h2; hex3: "08" }
                    FilmstripKnob {
                        hex0: "00"; hex1: gkFlick.h1; hex2: gkFlick.h2; hex3: "09"
                        filmstrip: "knobs/knob_56.png"; frameSize: 56
                    }
                    SyComboBox { hex0: "00"; hex1: gkFlick.h1; hex2: gkFlick.h2; hex3: inputRoot.isBass ? "18" : "17" }
                    SyComboBox { hex0: "00"; hex1: gkFlick.h1; hex2: gkFlick.h2; hex3: inputRoot.isBass ? "19" : "18" }
                }

                Item { width: 1; height: 4 }
                Rectangle { width: parent.width - 32; height: 1; color: "#2a2a2a"; x: 16 }
                Item { width: 1; height: 4 }

                // Distance 1-6
                Text {
                    x: 16; text: "DISTANCE"
                    color: "#888888"; font.pixelSize: 11
                    font.family: "Roboto Condensed"; font.capitalization: Font.AllUppercase
                }

                Row {
                    x: 24; spacing: 8
                    Repeater {
                        model: 6
                        FilmstripKnob {
                            hex0: "00"; hex1: gkFlick.h1; hex2: gkFlick.h2
                            hex3: {
                                var base = inputRoot.isBass ? 0x0C : 0x0B
                                var v = base + index
                                var h = v.toString(16).toUpperCase()
                                return h.length < 2 ? "0" + h : h
                            }
                            filmstrip: "knobs/knob_48.png"; frameSize: 48
                        }
                    }
                }

                Item { width: 1; height: 4 }

                // Level/Sens 1-6
                Text {
                    x: 16; text: "LEVEL"
                    color: "#888888"; font.pixelSize: 11
                    font.family: "Roboto Condensed"; font.capitalization: Font.AllUppercase
                }

                Row {
                    x: 24; spacing: 8
                    Repeater {
                        model: 6
                        FilmstripKnob {
                            hex0: "00"; hex1: gkFlick.h1; hex2: gkFlick.h2
                            hex3: {
                                var base = inputRoot.isBass ? 0x12 : 0x11
                                var v = base + index
                                var h = v.toString(16).toUpperCase()
                                return h.length < 2 ? "0" + h : h
                            }
                            filmstrip: "knobs/knob_48.png"; frameSize: 48
                        }
                    }
                }

                Item { width: 1; height: 4 }
                Rectangle { width: parent.width - 32; height: 1; color: "#2a2a2a"; x: 16 }
                Item { width: 1; height: 4 }

                // Bottom row: PU Gain, PU Position, Piezo Tone L/H
                Row {
                    x: 24; spacing: 16

                    FilmstripKnob {
                        hex0: "00"; hex1: gkFlick.h1; hex2: gkFlick.h2
                        hex3: inputRoot.isBass ? "1D" : "1C"
                        filmstrip: "knobs/knob_56.png"; frameSize: 56
                    }

                    SyComboBox {
                        hex0: "00"; hex1: gkFlick.h1; hex2: gkFlick.h2
                        hex3: inputRoot.isBass ? "0B" : "1B"
                    }

                    FilmstripKnob {
                        hex0: "00"; hex1: gkFlick.h1; hex2: gkFlick.h2
                        hex3: inputRoot.isBass ? "1A" : "19"
                        filmstrip: "knobs/knob_56.png"; frameSize: 56
                    }

                    FilmstripKnob {
                        hex0: "00"; hex1: gkFlick.h1; hex2: gkFlick.h2
                        hex3: inputRoot.isBass ? "1B" : "1A"
                        filmstrip: "knobs/knob_56.png"; frameSize: 56
                    }
                }
            }
        }
    }

    // ===================== NORMAL INPUT =====================
    Component {
        id: normalComponent

        Column {
            spacing: 0

            // Set selector
            Rectangle {
                width: parent.width
                height: 32
                color: "#222222"

                Row {
                    anchors.verticalCenter: parent.verticalCenter
                    x: 16
                    spacing: 8

                    Text {
                        text: "SETTING"
                        color: "#888888"
                        font.pixelSize: 11
                        font.family: "Roboto Condensed"
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    Repeater {
                        model: 10
                        Rectangle {
                            width: 28; height: 22
                            radius: 3
                            color: inputRoot.normalSetIndex === index ? "#66aacc" : nMa.containsMouse ? "#444444" : "#333333"
                            Text {
                                anchors.centerIn: parent
                                text: (index + 1).toString()
                                color: inputRoot.normalSetIndex === index ? "#000000" : "#cccccc"
                                font.pixelSize: 11; font.family: "Roboto Condensed"
                                font.bold: inputRoot.normalSetIndex === index
                            }
                            MouseArea {
                                id: nMa
                                anchors.fill: parent
                                hoverEnabled: true
                                onClicked: inputRoot.normalSetIndex = index
                            }
                        }
                    }
                }
            }

            Rectangle { width: parent.width; height: 1; color: "#333333" }

            // Normal Set content
            Loader {
                id: normalSetLoader
                width: parent.width
                height: parent.parent.height - 65
                active: true

                sourceComponent: normalSetContent

                property int setIdx: inputRoot.normalSetIndex
                onSetIdxChanged: { active = false; active = true }
            }
        }
    }

    Component {
        id: normalSetContent

        Flickable {
            id: nFlick
            contentHeight: nCol.height + 24
            clip: true

            property var addr: inputRoot.normalSetAddr(inputRoot.normalSetIndex)
            property string h1: addr[0]
            property string h2: addr[1]

            Column {
                id: nCol
                width: parent.width
                spacing: 8
                topPadding: 12

                // Comp On/Off
                Row {
                    x: 24; spacing: 16
                    SySwitch {
                        hex0: "00"; hex1: nFlick.h1; hex2: nFlick.h2; hex3: "08"
                    }
                }

                Item { width: 1; height: 4 }

                // Attack, Sustain, Level
                Row {
                    x: 24; spacing: 16
                    FilmstripKnob {
                        hex0: "00"; hex1: nFlick.h1; hex2: nFlick.h2; hex3: "09"
                        filmstrip: "knobs/knob_56.png"; frameSize: 56
                    }
                    FilmstripKnob {
                        hex0: "00"; hex1: nFlick.h1; hex2: nFlick.h2; hex3: "0A"
                        filmstrip: "knobs/knob_56.png"; frameSize: 56
                    }
                    FilmstripKnob {
                        hex0: "00"; hex1: nFlick.h1; hex2: nFlick.h2; hex3: "0B"
                        filmstrip: "knobs/knob_56.png"; frameSize: 56
                    }
                }

                // Sense, Low, High
                Row {
                    x: 24; spacing: 16
                    FilmstripKnob {
                        hex0: "00"; hex1: nFlick.h1; hex2: nFlick.h2; hex3: "0C"
                        filmstrip: "knobs/knob_56.png"; frameSize: 56
                    }
                    FilmstripKnob {
                        hex0: "00"; hex1: nFlick.h1; hex2: nFlick.h2; hex3: "0D"
                        filmstrip: "knobs/knob_56.png"; frameSize: 56
                    }
                    FilmstripKnob {
                        hex0: "00"; hex1: nFlick.h1; hex2: nFlick.h2; hex3: "0E"
                        filmstrip: "knobs/knob_56.png"; frameSize: 56
                    }
                }
            }
        }
    }
}
