import QtQuick
import ".."

Item {
    id: root
    property string hex1: "00"
    property string hex2: "4D"  // guitar="4D", bass="4A"

    property int harmMode: 0

    Component.onCompleted: {
        harmMode = paramBridge.getValue("10", hex1, hex2, "00")
    }

    Connections {
        target: harmModeCombo
        function onValueChanged() { root.harmMode = harmModeCombo.value }
    }

    Flickable {
        anchors.fill: parent
        contentHeight: contentCol.height
        clip: true

        Column {
            id: contentCol
            width: parent.width
            spacing: 8
            topPadding: 12

            // Voice config selector
            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 16

                SyComboBox {
                    id: harmModeCombo
                    hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "00"
                }
            }

            Rectangle { width: parent.width - 24; height: 1; color: "#333333"; anchors.horizontalCenter: parent.horizontalCenter }

            // Mode 0: 1 voice
            Column {
                width: parent.width
                spacing: 8
                visible: root.harmMode === 0

                Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 12
                    SyComboBox { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "01" }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "04"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "08"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "03"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "09"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                }
            }

            // Mode 1 or 2: 2 voices
            Column {
                width: parent.width
                spacing: 8
                visible: root.harmMode === 1 || root.harmMode === 2

                // Voice 1
                Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 12
                    SyComboBox { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "01" }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "04"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                }

                // Voice 2
                Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 12
                    SyComboBox { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "02" }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0B"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                }

                // Shared
                Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 12
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "08"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "03"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0A"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "09"; filmstrip: "knobs/knob_48.png"; frameSize: 48 }
                }
            }

            Rectangle { width: parent.width - 24; height: 1; color: "#333333"; anchors.horizontalCenter: parent.horizontalCenter }

            // User Harmony rows (visible when harmMode != "USER" which is value 29 decimal = 0x1D)
            // User Harmony 1
            Text {
                text: "USER HARMONY 1"
                color: "#666"
                font.pixelSize: 10
                font.family: "Roboto Condensed"
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Flow {
                width: parent.width - 24
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 4

                Repeater {
                    model: 12
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: root.hex2
                        hex3: {
                            var v = 0x0F + index
                            var h = v.toString(16).toUpperCase()
                            return h.length < 2 ? "0" + h : h
                        }
                        implicitWidth: 60
                    }
                }
            }

            // User Harmony 2
            Text {
                text: "USER HARMONY 2"
                color: "#666"
                font.pixelSize: 10
                font.family: "Roboto Condensed"
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Flow {
                width: parent.width - 24
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 4

                Repeater {
                    model: 12
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: root.hex2
                        hex3: {
                            var v = 0x1B + index
                            var h = v.toString(16).toUpperCase()
                            return h.length < 2 ? "0" + h : h
                        }
                        implicitWidth: 60
                    }
                }
            }
        }
    }
}
