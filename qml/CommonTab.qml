import QtQuick

Rectangle {
    id: root
    color: "#1a1a1a"

    property string hex1: "00"  // inst1="00", inst2="01", inst3="02"

    Flickable {
        anchors.fill: parent
        anchors.margins: 12
        contentWidth: grid.width
        contentHeight: grid.height
        clip: true

        Grid {
            id: grid
            columns: 6
            columnSpacing: 14
            rowSpacing: 10

            // Row 0: Inst Level, Nor Mix SW, Nor Mix Level
            FilmstripKnob {
                hex0: "10"; hex1: root.hex1; hex2: "15"; hex3: "02"
                filmstrip: "knobs/knob_56.png"; frameSize: 56
            }
            SySwitch {
                hex0: "10"; hex1: root.hex1; hex2: "15"; hex3: "03"
            }
            FilmstripKnob {
                hex0: "10"; hex1: root.hex1; hex2: "15"; hex3: "04"
            }
            Item { width: 48; height: 1 }
            Item { width: 48; height: 1 }
            Item { width: 48; height: 1 }

            // Row 1: String Levels 6-1
            FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "15"; hex3: "0B" }
            FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "15"; hex3: "0A" }
            FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "15"; hex3: "09" }
            FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "15"; hex3: "08" }
            FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "15"; hex3: "07" }
            FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "15"; hex3: "06" }

            // Row 2: String Pans 6-1
            FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "15"; hex3: "11" }
            FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "15"; hex3: "10" }
            FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "15"; hex3: "0F" }
            FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "15"; hex3: "0E" }
            FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "15"; hex3: "0D" }
            FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "15"; hex3: "0C" }
        }
    }
}
