import QtQuick
import ".."

Item {
    id: root
    property string hex1: "00"
    property string hex2: "48"  // guitar="48", bass="45"

    property int delayMode: 0

    Component.onCompleted: {
        delayMode = paramBridge.getValue("10", hex1, hex2, "01")
    }

    Connections {
        target: delayModeCombo
        function onValueChanged() { root.delayMode = delayModeCombo.value }
    }

    Column {
        anchors.fill: parent
        spacing: 8
        topPadding: 12

        // Mode selector
        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 16

            SyComboBox {
                id: delayModeCombo
                hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "01"
            }
        }

        Rectangle { width: parent.width - 24; height: 1; color: SyTheme.divider; anchors.horizontalCenter: parent.horizontalCenter }

        Item {
            width: parent.width
            height: parent.height - 60

            // Simple delay modes (STEREO1=0, STEREO2=1, REVERSE=6, ANALOG=7, TAPE=8, WARP=9)
            Row {
                anchors.centerIn: parent
                spacing: 12
                visible: root.delayMode <= 1 || (root.delayMode >= 6 && root.delayMode <= 9)

                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "02"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0E"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "07"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "08"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "09"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "20"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
            }

            // PAN mode (2) — adds tap time %
            Row {
                anchors.centerIn: parent
                spacing: 12
                visible: root.delayMode === 2

                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "02"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0E"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "1F"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "07"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "08"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "09"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "20"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
            }

            // DUAL modes (SERIES=3, PARALLEL=4, L/R=5)
            Column {
                anchors.centerIn: parent
                spacing: 8
                visible: root.delayMode >= 3 && root.delayMode <= 5

                // Delay 1
                Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 12
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0A"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0E"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0F"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "10"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                }

                Rectangle { width: parent.width; height: 1; color: SyTheme.divider }

                // Delay 2
                Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 12
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "11"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "15"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "16"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "17"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                }

                // Direct + carry over
                Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 12
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "09"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "20"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                }
            }

            // MODULATE mode (10)
            Row {
                anchors.centerIn: parent
                spacing: 12
                visible: root.delayMode === 10

                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "02"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0E"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "18"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "19"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "1A"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "07"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "08"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "09"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "20"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
            }

            // TWIST mode (11)
            Column {
                anchors.centerIn: parent
                spacing: 8
                visible: root.delayMode === 11

                property int twistMode: 0
                Component.onCompleted: {
                    twistMode = paramBridge.getValue("10", root.hex1, root.hex2, "1B")
                }

                Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 12
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "02"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0E"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                    SyComboBox { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "1B" }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "1C"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "1D"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "07"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "08"; filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "09"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "20"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                }
            }
        }
    }
}
