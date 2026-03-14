import QtQuick

Item {
    id: root
    property string hex1: "00"
    property string hex2: "58"  // guitar="58", bass="55"

    property int psMode: 0

    Component.onCompleted: {
        psMode = paramBridge.getValue("10", hex1, hex2, "00")
    }

    Connections {
        target: psModeCombo
        function onValueChanged() { root.psMode = psModeCombo.value }
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
                id: psModeCombo
                hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "00"
            }
        }

        Rectangle { width: parent.width - 24; height: 1; color: "#333333"; anchors.horizontalCenter: parent.horizontalCenter }

        Item {
            width: parent.width
            height: parent.height - 60

            // Mode 0: 1 voice
            Column {
                anchors.centerIn: parent
                spacing: 8
                visible: root.psMode === 0

                Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 12
                    SyComboBox { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "04" }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "06"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "01"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "05"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0B"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0A"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "03"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                }
            }

            // Mode 1 or 2: 2 voices
            Column {
                anchors.centerIn: parent
                spacing: 8
                visible: root.psMode === 1 || root.psMode === 2

                // Voice 1
                Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 12
                    SyComboBox { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "04" }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "06"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "01"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "05"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0B"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                }

                Rectangle { width: parent.width - 48; height: 1; color: "#333333"; anchors.horizontalCenter: parent.horizontalCenter }

                // Voice 2
                Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 12
                    SyComboBox { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0C" }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0E"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "02"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0D"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                }

                // Level
                Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 12
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0A"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "12"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "03"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                }
            }
        }
    }
}
