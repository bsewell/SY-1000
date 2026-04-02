import QtQuick

Rectangle {
    id: root
    color: SyTheme.bgPanel
    property string hex1: "00"
    property string hex2: "1C"

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

            // Bass Type
            SyComboBox { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "00"; implicitWidth: root.comboWidth }

            Grid {
                columns: 8
                columnSpacing: SyTheme.gridColSpacing
                rowSpacing: SyTheme.gridRowSpacing
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "01" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "02" }
                SyComboBox    { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "03"; implicitWidth: 100 }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "04" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "05" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "06" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "07" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "08" }
            }

            Row {
                spacing: SyTheme.gridColSpacing
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "09" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0A" }
            }

            Rectangle { width: 600; height: 1; color: SyTheme.divider }

            // Noise Suppressor
            Text { text: "NOISE SUPPRESSOR"; color: SyTheme.accent; font.pixelSize: 12; font.family: "Roboto Condensed"; font.bold: true }
            Row {
                spacing: SyTheme.gridColSpacing
                SySwitch      { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "1F" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "20" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "21" }
            }
        }
    }
}
