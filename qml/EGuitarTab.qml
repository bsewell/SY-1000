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

            // Guitar Type
            SyComboBox { hex0: "10"; hex1: root.hex1; hex2: "1A"; hex3: "00" }

            Grid {
                columns: 8
                columnSpacing: 14
                rowSpacing: 10

                // PU Select, Tone Type
                SyComboBox    { hex0: "10"; hex1: root.hex1; hex2: "1A"; hex3: "01"; implicitWidth: 100 }
                SyComboBox    { hex0: "10"; hex1: root.hex1; hex2: "1A"; hex3: "02"; implicitWidth: 100 }

                // Sense, Depth, Attack, Resonance, Direct
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1A"; hex3: "03" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1A"; hex3: "04" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1A"; hex3: "05" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1A"; hex3: "06" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1A"; hex3: "07" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1A"; hex3: "08" }
            }

            // Volume, Tone
            Grid {
                columns: 2
                columnSpacing: 14
                rowSpacing: 10
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1A"; hex3: "08" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1A"; hex3: "09" }
            }
        }
    }
}
