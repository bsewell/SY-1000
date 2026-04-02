import QtQuick

Rectangle {
    id: root
    color: SyTheme.bgPanel
    property string hex1: "00"
    property string hex2: "18"

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

            // LFO 1
            Text { text: "LFO 1"; color: SyTheme.accent; font.pixelSize: 12; font.family: "Roboto Condensed"; font.bold: true }
            Row {
                spacing: SyTheme.gridColSpacing
                SySwitch   { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "2D" }
                SyComboBox { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "2E" }
            }
            Grid {
                columns: 8
                columnSpacing: SyTheme.gridColSpacing
                rowSpacing: SyTheme.gridRowSpacing
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "2F" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "30" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "31" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "32" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "33" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "34" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "35" }
                SyComboBox { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "36"; implicitWidth: 100 }
            }

            Rectangle { width: 600; height: 1; color: SyTheme.divider }

            // LFO 2
            Text { text: "LFO 2"; color: SyTheme.accent; font.pixelSize: 12; font.family: "Roboto Condensed"; font.bold: true }
            Row {
                spacing: SyTheme.gridColSpacing
                SySwitch   { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "37" }
                SyComboBox { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "38" }
            }
            Grid {
                columns: 8
                columnSpacing: SyTheme.gridColSpacing
                rowSpacing: SyTheme.gridRowSpacing
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "39" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "3A" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "3B" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "3C" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "3D" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "3E" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "3F" }
                SyComboBox { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "40"; implicitWidth: 100 }
            }
        }
    }
}
