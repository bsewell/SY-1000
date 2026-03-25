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

        Rectangle { width: parent.width - 24; height: 1; color: "#333333"; anchors.horizontalCenter: parent.horizontalCenter }

        Item {
            width: parent.width
            height: parent.height - 60

            // Simple delay modes (STEREO1=0, STEREO2=1, REVERSE=6, ANALOG=7, TAPE=8, WARP=9)
            Row {
                anchors.centerIn: parent
                spacing: 12
                visible: root.delayMode <= 1 || (root.delayMode >= 6 && root.delayMode <= 9)

                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "02"; filmstrip: "knobs/knob_56.png"; frameSize: 56 }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0E"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "07"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "08"; filmstrip: "knobs/knob_56.png"; frameSize: 56 }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "09"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "20"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
            }

            // PAN mode (2) — adds tap time %
            Row {
                anchors.centerIn: parent
                spacing: 12
                visible: root.delayMode === 2

                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "02"; filmstrip: "knobs/knob_56.png"; frameSize: 56 }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0E"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "1F"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "07"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "08"; filmstrip: "knobs/knob_56.png"; frameSize: 56 }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "09"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "20"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
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
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0A"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0E"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0F"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "10"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                }

                Rectangle { width: parent.width; height: 1; color: "#333333" }

                // Delay 2
                Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 12
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "11"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "15"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "16"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "17"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                }

                // Direct + carry over
                Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 12
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "09"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "20"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                }
            }

            // MODULATE mode (10)
            Row {
                anchors.centerIn: parent
                spacing: 12
                visible: root.delayMode === 10

                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "02"; filmstrip: "knobs/knob_56.png"; frameSize: 56 }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0E"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "18"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "19"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "1A"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "07"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "08"; filmstrip: "knobs/knob_56.png"; frameSize: 56 }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "09"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "20"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
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
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "02"; filmstrip: "knobs/knob_56.png"; frameSize: 56 }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0E"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                    SyComboBox { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "1B" }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "1C"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "1D"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "07"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "08"; filmstrip: "knobs/knob_56.png"; frameSize: 56 }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "09"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "20"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                }
            }
        }
    }
}
