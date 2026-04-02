import QtQuick

Rectangle {
    id: root
    color: SyTheme.bgPanel
    property string hex1: "00"
    property string hex2: "18"

    readonly property int comboWidth: 340

    Flickable {
        anchors.fill: parent
        anchors.margins: SyTheme.panelPadding
        contentWidth: col.width
        contentHeight: col.height
        clip: true
        interactive: contentWidth > width || contentHeight > height

        Column {
            id: col
            spacing: SyTheme.gridRowSpacing

            // Filter Section
            Text { text: "FILTER"; color: SyTheme.accent; font.pixelSize: 12; font.family: "Roboto Condensed"; font.bold: true }
            Row {
                spacing: SyTheme.gridColSpacing
                SySwitch   { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "1A" }
                SyComboBox { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "1B"; implicitWidth: 240; labelWidth: 0 }
            }
            Grid {
                columns: 8
                columnSpacing: SyTheme.gridColSpacing
                rowSpacing: SyTheme.gridRowSpacing
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "1C" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "1D" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "1E" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "1F" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "20" }
            }

            Rectangle { width: 600; height: 1; color: SyTheme.divider }

            // Filter Envelope: A, D, S, R, Depth
            Text { text: "FILTER ENV"; color: SyTheme.textDimmed; font.pixelSize: 10; font.family: "Roboto Condensed" }
            Grid {
                columns: 5
                columnSpacing: SyTheme.gridColSpacing
                rowSpacing: SyTheme.gridRowSpacing
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "21" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "22" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "23" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "24" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "25" }
            }

            Rectangle { width: 600; height: 1; color: SyTheme.divider }

            // Amp Envelope: A, D, S, R, Vel Sense
            Text { text: "AMP ENV"; color: SyTheme.textDimmed; font.pixelSize: 10; font.family: "Roboto Condensed" }
            Grid {
                columns: 7
                columnSpacing: SyTheme.gridColSpacing
                rowSpacing: SyTheme.gridRowSpacing
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "27" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "28" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "29" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "2A" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "26" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "2B" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "2C" }
            }
        }
    }
}
