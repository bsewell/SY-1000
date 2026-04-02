import QtQuick

Rectangle {
    id: root
    color: SyTheme.bgPanel
    property string hex1: "00"
    property string hex2: "1D"

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
            spacing: 6

            // Stacked dropdowns — all same width, left-aligned
            SyComboBox { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "00"; implicitWidth: root.comboWidth }
            SyComboBox { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "01"; implicitWidth: root.comboWidth }
            SyComboBox { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "02"; implicitWidth: root.comboWidth }

            Item { width: 1; height: 4 }

            Grid {
                columns: 8
                columnSpacing: SyTheme.gridColSpacing
                rowSpacing: SyTheme.gridRowSpacing
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "03" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "04" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "05" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "06" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "07" }
            }

            Row {
                spacing: SyTheme.gridColSpacing
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "08" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "09" }
            }

            Rectangle { width: 600; height: 1; color: SyTheme.divider }

            // Filter section
            Text { text: "FILTER"; color: SyTheme.accent; font.pixelSize: 12; font.family: "Roboto Condensed"; font.bold: true }
            Grid {
                columns: 8
                columnSpacing: SyTheme.gridColSpacing
                rowSpacing: SyTheme.gridRowSpacing
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0E" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0F" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "10" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "11" }
            }
            SyComboBox    { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "12"; implicitWidth: root.comboWidth }
            Grid {
                columns: 3
                columnSpacing: SyTheme.gridColSpacing
                rowSpacing: SyTheme.gridRowSpacing
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "13" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "14" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "15" }
            }

            Rectangle { width: 600; height: 1; color: SyTheme.divider }

            // NS
            Text { text: "NOISE SUPPRESSOR"; color: SyTheme.accent; font.pixelSize: 12; font.family: "Roboto Condensed"; font.bold: true }
            Row {
                spacing: SyTheme.gridColSpacing
                SySwitch      { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "16" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "17" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "18" }
            }
        }
    }
}
