import QtQuick

Rectangle {
    id: root
    color: SyTheme.bgPanel
    property string hex1: "00"
    property string hex2: "1A"

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

            // Amp SW
            SySwitch { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0A" }

            // PreAmp
            Text { text: "PREAMP"; color: SyTheme.accent; font.pixelSize: 12; font.family: "Roboto Condensed"; font.bold: true }
            SyComboBox { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0B"; implicitWidth: root.comboWidth }
            SyComboBox { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "15"; implicitWidth: root.comboWidth }
            Grid {
                columns: 8
                columnSpacing: SyTheme.gridColSpacing
                rowSpacing: SyTheme.gridRowSpacing
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0C" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0D" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0E" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "10" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "11" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "12" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "13" }
                SySwitch      { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "14" }
            }
            FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0F" }

            Rectangle { width: 600; height: 1; color: SyTheme.divider }

            // Speaker
            Text { text: "SPEAKER"; color: SyTheme.accent; font.pixelSize: 12; font.family: "Roboto Condensed"; font.bold: true }
            SyComboBox { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "18"; implicitWidth: root.comboWidth }
            SyComboBox { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "19"; implicitWidth: root.comboWidth }
            SyComboBox { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "1A"; implicitWidth: root.comboWidth }
            Grid {
                columns: 3
                columnSpacing: SyTheme.gridColSpacing
                rowSpacing: SyTheme.gridRowSpacing
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "1B" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "1C" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "1D" }
            }

            Rectangle { width: 600; height: 1; color: SyTheme.divider }

            // Solo
            Text { text: "SOLO"; color: SyTheme.accent; font.pixelSize: 12; font.family: "Roboto Condensed"; font.bold: true }
            Row {
                spacing: SyTheme.gridColSpacing
                SySwitch      { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "16" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "17" }
            }
        }
    }
}
