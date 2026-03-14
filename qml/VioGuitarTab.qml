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

            // Type
            SyComboBox { hex0: "10"; hex1: root.hex1; hex2: "1D"; hex3: "00" }

            Grid {
                columns: 8
                columnSpacing: 14
                rowSpacing: 10

                SyComboBox    { hex0: "10"; hex1: root.hex1; hex2: "1D"; hex3: "01"; implicitWidth: 100 }
                SyComboBox    { hex0: "10"; hex1: root.hex1; hex2: "1D"; hex3: "02"; implicitWidth: 100 }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1D"; hex3: "03" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1D"; hex3: "04" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1D"; hex3: "05" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1D"; hex3: "06" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1D"; hex3: "07" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1D"; hex3: "08" }
            }

            Row {
                spacing: 14
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1D"; hex3: "08" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1D"; hex3: "09" }
            }

            Rectangle { width: 600; height: 1; color: "#333" }

            // Filter section
            Text { text: "FILTER"; color: "#00ccff"; font.pixelSize: 12; font.family: "Roboto Condensed"; font.bold: true }
            Grid {
                columns: 8
                columnSpacing: 14
                rowSpacing: 10
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1D"; hex3: "0E" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1D"; hex3: "0F" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1D"; hex3: "10" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1D"; hex3: "11" }
                SyComboBox    { hex0: "10"; hex1: root.hex1; hex2: "1D"; hex3: "12"; implicitWidth: 100 }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1D"; hex3: "13" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1D"; hex3: "14" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1D"; hex3: "15" }
            }

            Rectangle { width: 600; height: 1; color: "#333" }

            // NS
            Text { text: "NOISE SUPPRESSOR"; color: "#00ccff"; font.pixelSize: 12; font.family: "Roboto Condensed"; font.bold: true }
            Row {
                spacing: 14
                SySwitch      { hex0: "10"; hex1: root.hex1; hex2: "1D"; hex3: "16" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1D"; hex3: "17" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "1D"; hex3: "18" }
            }
        }
    }
}
