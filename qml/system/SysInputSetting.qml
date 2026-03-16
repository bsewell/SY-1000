import QtQuick
import ".."

Item {
    id: inputRoot
    anchors.fill: parent
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
            height: 32; spacing: 0

            Repeater {
                model: ["GK INPUT", "NORMAL INPUT"]
                Rectangle {
                    width: inputRoot.width / 2; height: 32
                    color: inputRoot.currentTab === index ? "#313A47" : "#222222"
                    Rectangle {
                        width: parent.width; height: 2
                        anchors.bottom: parent.bottom
                        color: inputRoot.currentTab === index ? "#66aacc" : "transparent"
                    }
                    Text {
                        anchors.centerIn: parent; text: modelData
                        color: inputRoot.currentTab === index ? "#ffffff" : "#888888"
                        font.pixelSize: 12; font.family: "Roboto Condensed"
                        font.bold: inputRoot.currentTab === index
                    }
                    MouseArea { anchors.fill: parent; onClicked: inputRoot.currentTab = index }
                }
            }
        }
        Rectangle { width: parent.width; height: 1; color: "#333333" }

        // Content
        Loader {
            width: parent.width; height: parent.height - 33
            sourceComponent: inputRoot.currentTab === 0 ? gkComponent : normalComponent
        }
    }

    // ===================== GK INPUT =====================
    Component {
        id: gkComponent

        Column {
            spacing: 0

            // Set selector bar
            Rectangle {
                width: parent.width; height: 36
                color: "#313A47"

                Row {
                    anchors.verticalCenter: parent.verticalCenter
                    x: 16; spacing: 6

                    Text {
                        text: "SETTING"
                        color: "rgba(255,255,255,0.8)"; font.pixelSize: 12
                        font.family: "Roboto Condensed"
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    Repeater {
                        model: 10
                        Rectangle {
                            width: 28; height: 22; radius: 3
                            color: inputRoot.gkSetIndex === index ? "#66aacc" : "#444444"
                            Text {
                                anchors.centerIn: parent
                                text: (index + 1).toString()
                                color: inputRoot.gkSetIndex === index ? "#000000" : "#cccccc"
                                font.pixelSize: 11; font.family: "Roboto Condensed"
                                font.bold: inputRoot.gkSetIndex === index
                            }
                            MouseArea {
                                anchors.fill: parent
                                onClicked: inputRoot.gkSetIndex = index
                            }
                        }
                    }
                }
            }

            Rectangle { width: parent.width; height: 1; color: "#333333" }

            // GK Set content — Loader forces recreate on set change
            Loader {
                id: gkSetLoader
                width: parent.width
                height: parent.parent.height - 70
                active: true
                sourceComponent: gkSetContent
                property int setIdx: inputRoot.gkSetIndex
                onSetIdxChanged: { active = false; active = true }
            }
        }
    }

    Component {
        id: gkSetContent

        Flickable {
            id: gkFlick
            contentHeight: gkCol.height + 32; clip: true

            property var addr: inputRoot.gkSetAddr(inputRoot.gkSetIndex)
            property string h1: addr[0]
            property string h2: addr[1]

            Column {
                id: gkCol; width: parent.width; spacing: 0

                // GK TYPE
                Item {
                    width: parent.width; height: 44
                    Text { x: 32; y: 14; text: "GK TYPE"; color: "rgba(255,255,255,0.8)"; font.pixelSize: 12; font.family: "Roboto Condensed" }
                    SyComboBox { x: 200; y: 8; hex0: "00"; hex1: gkFlick.h1; hex2: gkFlick.h2; hex3: "08" }
                }
                // PU PHASE
                Item {
                    width: parent.width; height: 44
                    Text { x: 32; y: 14; text: "PU PHASE"; color: "rgba(255,255,255,0.8)"; font.pixelSize: 12; font.family: "Roboto Condensed" }
                    SyComboBox { x: 200; y: 8; hex0: "00"; hex1: gkFlick.h1; hex2: gkFlick.h2; hex3: inputRoot.isBass ? "18" : "17" }
                }
                // PU DIRECTION
                Item {
                    width: parent.width; height: 44
                    Text { x: 32; y: 14; text: "PU DIRECTION"; color: "rgba(255,255,255,0.8)"; font.pixelSize: 12; font.family: "Roboto Condensed" }
                    SyComboBox { x: 200; y: 8; hex0: "00"; hex1: gkFlick.h1; hex2: gkFlick.h2; hex3: inputRoot.isBass ? "19" : "18" }
                }
                // PU POSITION
                Item {
                    width: parent.width; height: 44
                    Text { x: 32; y: 14; text: "PU POSITION"; color: "rgba(255,255,255,0.8)"; font.pixelSize: 12; font.family: "Roboto Condensed" }
                    SyComboBox { x: 200; y: 8; hex0: "00"; hex1: gkFlick.h1; hex2: gkFlick.h2; hex3: inputRoot.isBass ? "0B" : "1B" }
                }

                Rectangle { width: parent.width - 32; height: 1; color: "rgba(255,255,255,0.15)"; x: 16 }

                // SCALE knob
                Text { x: 16; height: 28; verticalAlignment: Text.AlignVCenter; text: "SCALE"; color: "rgba(255,255,255,0.8)"; font.pixelSize: 12; font.family: "Roboto Condensed" }
                Row {
                    x: 32; spacing: 32; height: 80
                    FilmstripKnob {
                        hex0: "00"; hex1: gkFlick.h1; hex2: gkFlick.h2; hex3: "09"
                        filmstrip: "knobs/knob_56.png"; frameSize: 56
                    }
                    // NORMAL PU GAIN
                    FilmstripKnob {
                        hex0: "00"; hex1: gkFlick.h1; hex2: gkFlick.h2
                        hex3: inputRoot.isBass ? "1D" : "1C"
                        filmstrip: "knobs/knob_56.png"; frameSize: 56
                    }
                }

                Item { width: 1; height: 8 }
                Rectangle { width: parent.width - 32; height: 1; color: "rgba(255,255,255,0.15)"; x: 16 }

                // DISTANCE 1-6
                Text { x: 16; height: 28; verticalAlignment: Text.AlignVCenter; text: "DISTANCE"; color: "rgba(255,255,255,0.8)"; font.pixelSize: 12; font.family: "Roboto Condensed" }
                Row {
                    x: 32; spacing: 16; height: 80
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

                Item { width: 1; height: 8 }
                Rectangle { width: parent.width - 32; height: 1; color: "rgba(255,255,255,0.15)"; x: 16 }

                // LEVEL / SENS 1-6
                Text { x: 16; height: 28; verticalAlignment: Text.AlignVCenter; text: "LEVEL / SENS"; color: "rgba(255,255,255,0.8)"; font.pixelSize: 12; font.family: "Roboto Condensed" }
                Row {
                    x: 32; spacing: 16; height: 80
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

                Item { width: 1; height: 8 }
                Rectangle { width: parent.width - 32; height: 1; color: "rgba(255,255,255,0.15)"; x: 16 }

                // PIEZO TONE
                Text { x: 16; height: 28; verticalAlignment: Text.AlignVCenter; text: "PIEZO TONE"; color: "rgba(255,255,255,0.8)"; font.pixelSize: 12; font.family: "Roboto Condensed" }
                Row {
                    x: 32; spacing: 32; height: 80
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

            // Set selector bar
            Rectangle {
                width: parent.width; height: 36
                color: "#313A47"

                Row {
                    anchors.verticalCenter: parent.verticalCenter
                    x: 16; spacing: 6

                    Text {
                        text: "SETTING"
                        color: "rgba(255,255,255,0.8)"; font.pixelSize: 12
                        font.family: "Roboto Condensed"
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    Repeater {
                        model: 10
                        Rectangle {
                            width: 28; height: 22; radius: 3
                            color: inputRoot.normalSetIndex === index ? "#66aacc" : "#444444"
                            Text {
                                anchors.centerIn: parent
                                text: (index + 1).toString()
                                color: inputRoot.normalSetIndex === index ? "#000000" : "#cccccc"
                                font.pixelSize: 11; font.family: "Roboto Condensed"
                                font.bold: inputRoot.normalSetIndex === index
                            }
                            MouseArea {
                                anchors.fill: parent
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
                height: parent.parent.height - 70
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
            contentHeight: nCol.height + 32; clip: true

            property var addr: inputRoot.normalSetAddr(inputRoot.normalSetIndex)
            property string h1: addr[0]
            property string h2: addr[1]

            Column {
                id: nCol; width: parent.width; spacing: 0

                // COMP ON/OFF
                Item {
                    width: parent.width; height: 60
                    Text {
                        x: 32; anchors.verticalCenter: parent.verticalCenter
                        text: "COMP ON/OFF"; color: "rgba(255,255,255,0.8)"; font.pixelSize: 12; font.family: "Roboto Condensed"
                    }
                    SySwitch {
                        x: 200; y: -4
                        hex0: "00"; hex1: nFlick.h1; hex2: nFlick.h2; hex3: "08"
                    }
                }

                Rectangle { width: parent.width - 32; height: 1; color: "rgba(255,255,255,0.15)"; x: 16 }

                // Knobs: Attack, Sustain, Level
                Text { x: 16; height: 28; verticalAlignment: Text.AlignVCenter; text: "ATTACK / SUSTAIN / LEVEL"; color: "rgba(255,255,255,0.8)"; font.pixelSize: 12; font.family: "Roboto Condensed" }
                Row {
                    x: 32; spacing: 32; height: 80
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

                Item { width: 1; height: 8 }
                Rectangle { width: parent.width - 32; height: 1; color: "rgba(255,255,255,0.15)"; x: 16 }

                // Knobs: Sense, Low, High
                Text { x: 16; height: 28; verticalAlignment: Text.AlignVCenter; text: "SENSE / LOW / HIGH"; color: "rgba(255,255,255,0.8)"; font.pixelSize: 12; font.family: "Roboto Condensed" }
                Row {
                    x: 32; spacing: 32; height: 80
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
