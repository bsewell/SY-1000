import QtQuick

Rectangle {
    id: root
    color: SyTheme.bgPanel
    property string hex1: "00"
    property string hex2: "16"

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

            // Shape dropdown
            SyComboBox {
                hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "32"
            }

            Grid {
                columns: 8
                columnSpacing: SyTheme.gridColSpacing
                rowSpacing: SyTheme.gridRowSpacing

                // Row 0: SW, Rate, Dynamic Depth, Pitch Depth, Filter Depth, Amp Depth, PWM Depth
                SySwitch      { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "31" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "33" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "34" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "35" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "36" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "37" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "38" }

                // Row 1: Fade Time, Sync, BPM
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "39" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "3A" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: "12"; hex3: "3E" }
            }
        }
    }
}
