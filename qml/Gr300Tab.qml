import QtQuick

Rectangle {
    id: root
    color: "#1a1a1a"
    property string hex1: "00"

    Flickable {
        anchors.fill: parent
        anchors.margins: 12
        contentWidth: col.width
        contentHeight: col.height
        clip: true
        interactive: contentWidth > width || contentHeight > height

        Column {
            id: col
            spacing: 10

            // Mode, Comp, Filter Cutoff, Resonance, Env Mod
            Row {
                spacing: 14
                SyComboBox { hex0: "10"; hex1: root.hex1; hex2: "19"; hex3: "00" }
                SySwitch   { hex0: "10"; hex1: root.hex1; hex2: "19"; hex3: "01" }
            }

            Grid {
                columns: 8
                columnSpacing: 14
                rowSpacing: 10

                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "19"; hex3: "02" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "19"; hex3: "03" }
                SyComboBox    { hex0: "10"; hex1: root.hex1; hex2: "19"; hex3: "04"; implicitWidth: 100 }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "19"; hex3: "05" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "19"; hex3: "06" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "19"; hex3: "13" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "19"; hex3: "14" }
            }

            Rectangle { width: 600; height: 1; color: "#333" }

            // Pitch section
            Text { text: "PITCH"; color: "#00ccff"; font.pixelSize: 12; font.family: "Roboto Condensed"; font.bold: true }
            Row {
                spacing: 14
                SyComboBox { hex0: "10"; hex1: root.hex1; hex2: "19"; hex3: "07" }
            }
            Grid {
                columns: 8
                columnSpacing: 14
                rowSpacing: 10

                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "19"; hex3: "08" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "19"; hex3: "09" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "19"; hex3: "0A" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "19"; hex3: "0B" }
                SySwitch      { hex0: "10"; hex1: root.hex1; hex2: "19"; hex3: "0C" }
                SySwitch      { hex0: "10"; hex1: root.hex1; hex2: "19"; hex3: "0D" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "19"; hex3: "0E" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "19"; hex3: "0F" }
            }

            // Vibrato
            Grid {
                columns: 3
                columnSpacing: 14
                rowSpacing: 10
                SySwitch      { hex0: "10"; hex1: root.hex1; hex2: "19"; hex3: "10" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "19"; hex3: "11" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "19"; hex3: "12" }
            }
        }
    }
}
