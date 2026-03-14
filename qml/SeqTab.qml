import QtQuick

Rectangle {
    id: root
    color: "#1a1a1a"
    property string hex1: "00"
    // SeqTab spans two hex2 pages: hex2a (DynSynth) and hex2b (DynSynthCont)
    property string hex2a: "16"
    property string hex2b: "17"

    // Compute hex2 for level steps that straddle the page boundary
    function levelStepHex(index) {
        var addr = 0x7E + index * 2
        if (addr > 0x7F) {
            return { hex2: root.hex2b, hex3: (addr - 0x80).toString(16).toUpperCase().padStart(2, '0') }
        }
        return { hex2: root.hex2a, hex3: addr.toString(16).toUpperCase().padStart(2, '0') }
    }

    Flickable {
        anchors.fill: parent
        anchors.margins: 12
        contentWidth: col.width
        contentHeight: col.height
        clip: true
        interactive: contentWidth > width || contentHeight > height

        Column {
            id: col
            spacing: 14

            // ---- SEQ 1 ----
            Text {
                text: "SEQ 1"
                color: "#00ccff"
                font.pixelSize: 12
                font.family: "Roboto Condensed"
                font.bold: true
            }

            Row {
                spacing: 14

                SySwitch      { hex0: "10"; hex1: root.hex1; hex2: root.hex2b; hex3: "1E" }  // SEQ1 SW
                SyComboBox    { hex0: "10"; hex1: root.hex1; hex2: root.hex2a; hex3: "3B"; implicitWidth: 140 }  // Pitch Target
                SyComboBox    { hex0: "10"; hex1: root.hex1; hex2: root.hex2a; hex3: "3C"; implicitWidth: 140 }  // Cutoff Target
                SyComboBox    { hex0: "10"; hex1: root.hex1; hex2: root.hex2a; hex3: "3D"; implicitWidth: 140 }  // Level Target
                SySwitch      { hex0: "10"; hex1: root.hex1; hex2: root.hex2b; hex3: "1F" }  // Sync1
                SySwitch      { hex0: "10"; hex1: root.hex1; hex2: root.hex2b; hex3: "22" }  // 1Shot1
                SySwitch      { hex0: "10"; hex1: root.hex1; hex2: root.hex2b; hex3: "23" }  // Turbo1
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2b; hex3: "21" }  // Rate1
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2b; hex3: "20" }  // Length1
            }

            // SEQ1 Pitch Steps
            Text { text: "PITCH STEPS"; color: "#666"; font.pixelSize: 9; font.family: "Roboto Condensed" }
            Row {
                spacing: 4
                Repeater {
                    model: 16
                    FilmstripKnob {
                        hex0: "10"; hex1: root.hex1; hex2: root.hex2a
                        hex3: {
                            var base = 0x3E + index * 2
                            return base.toString(16).toUpperCase().padStart(2, '0')
                        }
                        frameSize: 32
                        filmstrip: "knobs/knob_48.png"
                    }
                }
            }

            // SEQ1 Cutoff Steps
            Text { text: "CUTOFF STEPS"; color: "#666"; font.pixelSize: 9; font.family: "Roboto Condensed" }
            Row {
                spacing: 4
                Repeater {
                    model: 16
                    FilmstripKnob {
                        hex0: "10"; hex1: root.hex1; hex2: root.hex2a
                        hex3: {
                            var base = 0x5E + index * 2
                            return base.toString(16).toUpperCase().padStart(2, '0')
                        }
                        frameSize: 32
                        filmstrip: "knobs/knob_48.png"
                    }
                }
            }

            // SEQ1 Level Steps
            Text { text: "LEVEL STEPS"; color: "#666"; font.pixelSize: 9; font.family: "Roboto Condensed" }
            Row {
                spacing: 4
                Repeater {
                    model: 16
                    FilmstripKnob {
                        hex0: "10"; hex1: root.hex1
                        hex2: root.levelStepHex(index).hex2
                        hex3: root.levelStepHex(index).hex3
                        frameSize: 32
                        filmstrip: "knobs/knob_48.png"
                    }
                }
            }

            // Divider
            Rectangle { width: 800; height: 1; color: "#333" }

            // ---- SEQ 2 ----
            Text {
                text: "SEQ 2"
                color: "#00ccff"
                font.pixelSize: 12
                font.family: "Roboto Condensed"
                font.bold: true
            }

            Row {
                spacing: 14

                SySwitch      { hex0: "10"; hex1: root.hex1; hex2: root.hex2b; hex3: "34" }  // SEQ2 SW
                SySwitch      { hex0: "10"; hex1: root.hex1; hex2: root.hex2b; hex3: "35" }  // Sync2
                SySwitch      { hex0: "10"; hex1: root.hex1; hex2: root.hex2b; hex3: "38" }  // 1Shot2
                SySwitch      { hex0: "10"; hex1: root.hex1; hex2: root.hex2b; hex3: "39" }  // Turbo2
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2b; hex3: "37" }  // Rate2
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2b; hex3: "36" }  // Length2
            }
        }
    }
}
