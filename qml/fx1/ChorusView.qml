import QtQuick

Item {
    id: root
    property string hex1: "00"
    property string hex2: "43"  // guitar="43", bass="40"

    property int modeIndex: 0

    Component.onCompleted: {
        modeIndex = paramBridge.getValue("10", hex1, hex2, "01")
    }

    Connections {
        target: modeCombo
        function onValueChanged() { root.modeIndex = modeCombo.value }
    }

    Column {
        anchors.fill: parent
        spacing: 8
        topPadding: 12

        // Mode + Level row
        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 16

            SyComboBox {
                id: modeCombo
                hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "01"
            }

            FilmstripKnob {
                hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "17"
                filmstrip: "knobs/knob_48.png"; frameSize: 48
            }

            SyComboBox {
                hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "18"
            }
        }

        Rectangle { width: parent.width - 24; height: 1; color: "#333333"; anchors.horizontalCenter: parent.horizontalCenter }

        // Chorus controls — modes 0-2 share same params, mode 3 has dual chorus
        Item {
            width: parent.width
            height: parent.height - 80

            // Single chorus (modes 0, 1, 2)
            Row {
                anchors.centerIn: parent
                spacing: 12
                visible: root.modeIndex < 3

                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "02"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "03"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "04"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "05"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                SyComboBox { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "06" }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "07"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "08"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
            }

            // Dual chorus (mode 3)
            Column {
                anchors.centerIn: parent
                spacing: 8
                visible: root.modeIndex >= 3

                // Chorus 1
                Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 12
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "09"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0A"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0B"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0C"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                    SyComboBox { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0D" }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0E"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0F"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                }

                Rectangle { width: parent.width; height: 1; color: "#333333" }

                // Chorus 2
                Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 12
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "10"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "11"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "12"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "13"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                    SyComboBox { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "14" }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "15"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "16"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                }
            }
        }
    }
}
