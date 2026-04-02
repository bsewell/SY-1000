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
            spacing: 6

            // Wave Category + Wave Type — stacked, same width
            SyComboBox { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "00"; implicitWidth: root.comboWidth }
            SyComboBox { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "01"; implicitWidth: root.comboWidth }

            Item { width: 1; height: 4 }

            // OSC 1: Pitch, Fine, PW, PWM Env, Env A/B/C, Level
            Text { text: "OSC 1"; color: SyTheme.accent; font.pixelSize: 12; font.family: "Roboto Condensed"; font.bold: true }
            Grid {
                columns: 8
                columnSpacing: SyTheme.gridColSpacing
                rowSpacing: SyTheme.gridRowSpacing
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "02" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "03" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "04" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "05" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "06" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "07" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "08" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "09" }
            }

            Rectangle { width: 600; height: 1; color: SyTheme.divider }

            // Mono/Poly, Hold Mode — stacked, same width
            SyComboBox { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "13"; implicitWidth: root.comboWidth }
            SyComboBox { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "18"; implicitWidth: root.comboWidth }
            SyComboBox { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "17"; implicitWidth: root.comboWidth }

            Row {
                spacing: SyTheme.gridColSpacing
                SySwitch   { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "15" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "16" }
                SySwitch   { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "14" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "19" }
            }

            Rectangle { width: 600; height: 1; color: SyTheme.divider }

            // OSC 2: Wave Type, Pitch, Fine, PW, PWM Env, Env A/B/C, Level
            Text { text: "OSC 2"; color: SyTheme.accent; font.pixelSize: 12; font.family: "Roboto Condensed"; font.bold: true }
            SyComboBox { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0A"; implicitWidth: root.comboWidth }
            Grid {
                columns: 8
                columnSpacing: SyTheme.gridColSpacing
                rowSpacing: SyTheme.gridRowSpacing
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0B" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0C" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0D" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0E" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0F" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "10" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "11" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "12" }
            }
        }
    }
}
