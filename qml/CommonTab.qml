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
            columnSpacing: 20
            rowSpacing: 14

            // Row 0: Inst Level, Nor Mix SW, Nor Mix Level
            SyKnob {
                hex0: "10"; hex1: root.hex1; hex2: "15"; hex3: "02"
                knobScale: 1.5
            }
            SySwitch {
                hex0: "10"; hex1: root.hex1; hex2: "15"; hex3: "03"
            }
            SyKnob {
                hex0: "10"; hex1: root.hex1; hex2: "15"; hex3: "04"
                knobScale: 1.25
            }
            Item { width: 90; height: 1 }  // spacers
            Item { width: 90; height: 1 }
            Item { width: 90; height: 1 }

            // Row 1: String Levels 6-1
            SyKnob {
                hex0: "10"; hex1: root.hex1; hex2: "15"; hex3: "0B"
                knobScale: 1.25
            }
            SyKnob {
                hex0: "10"; hex1: root.hex1; hex2: "15"; hex3: "0A"
                knobScale: 1.25
            }
            SyKnob {
                hex0: "10"; hex1: root.hex1; hex2: "15"; hex3: "09"
                knobScale: 1.25
            }
            SyKnob {
                hex0: "10"; hex1: root.hex1; hex2: "15"; hex3: "08"
                knobScale: 1.25
            }
            SyKnob {
                hex0: "10"; hex1: root.hex1; hex2: "15"; hex3: "07"
                knobScale: 1.25
            }
            SyKnob {
                hex0: "10"; hex1: root.hex1; hex2: "15"; hex3: "06"
                knobScale: 1.25
            }

            // Row 2: String Pans 6-1
            SyKnob {
                hex0: "10"; hex1: root.hex1; hex2: "15"; hex3: "11"
                knobScale: 1.25
            }
            SyKnob {
                hex0: "10"; hex1: root.hex1; hex2: "15"; hex3: "10"
                knobScale: 1.25
            }
            SyKnob {
                hex0: "10"; hex1: root.hex1; hex2: "15"; hex3: "0F"
                knobScale: 1.25
            }
            SyKnob {
                hex0: "10"; hex1: root.hex1; hex2: "15"; hex3: "0E"
                knobScale: 1.25
            }
            SyKnob {
                hex0: "10"; hex1: root.hex1; hex2: "15"; hex3: "0D"
                knobScale: 1.25
            }
            SyKnob {
                hex0: "10"; hex1: root.hex1; hex2: "15"; hex3: "0C"
                knobScale: 1.25
            }
        }
    }
}
