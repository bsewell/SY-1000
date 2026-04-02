import QtQuick

Rectangle {
    id: root
    color: SyTheme.bgPanel

    property string hex1: "00"
    property string hex2: "15"  // inst1="15", inst2="20"/"1F", inst3="2B"/"29"

    Flickable {
        anchors.fill: parent
        anchors.margins: SyTheme.panelPadding
        contentWidth: grid.width
        contentHeight: grid.height
        clip: true
        // Only allow scrolling when content overflows, so knob drags aren't hijacked
        interactive: contentWidth > width || contentHeight > height

        Grid {
            id: grid
            columns: 6
            columnSpacing: SyTheme.gridColSpacing
            rowSpacing: SyTheme.gridRowSpacing

            // Row 0: Inst Level, Nor Mix SW, Nor Mix Level
            FilmstripKnob {
                hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "02"
                filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge
            }
            SySwitch {
                hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "03"
            }
            FilmstripKnob {
                hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "04"
            }
            Item { width: 48; height: 1 }
            Item { width: 48; height: 1 }
            Item { width: 48; height: 1 }

            // Row 1: String Levels 6-1
            FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0B" }
            FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0A" }
            FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "09" }
            FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "08" }
            FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "07" }
            FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "06" }

            // Row 2: String Pans 6-1
            FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "11" }
            FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "10" }
            FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0F" }
            FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0E" }
            FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0D" }
            FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0C" }
        }
    }
}
