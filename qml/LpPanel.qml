import QtQuick

Rectangle {
    id: root
    color: "#1a1a1a"
    implicitWidth: 800
    implicitHeight: 480

    property string hex1: "00"
    property string hex2: "12"
    property color accentColor: Qt.rgba(0, 0.7, 0.2, 1)

    Column {
        anchors.fill: parent
        spacing: 0

        StompHeader {
            width: parent.width
            accentColor: root.accentColor
            title: "SEND/RETURN"
            powerHex0: "10"; powerHex1: root.hex1; powerHex2: "12"; powerHex3: "2E"
        }

        Rectangle { width: parent.width; height: 1; color: "#333333" }

        Item {
            width: parent.width
            height: parent.height - 37

            Flow {
                x: 12
                y: 16
                width: parent.width - 24
                spacing: 16

                SyComboBox {
                    hex0: "10"; hex1: root.hex1; hex2: "12"; hex3: "2F"
                }

                FilmstripKnob {
                    hex0: "10"; hex1: root.hex1; hex2: "12"; hex3: "30"
                    filmstrip: "knobs/knob_56.png"; frameSize: 56
                }

                FilmstripKnob {
                    hex0: "10"; hex1: root.hex1; hex2: "12"; hex3: "32"
                    filmstrip: "knobs/knob_56.png"; frameSize: 56
                }

                FilmstripKnob {
                    hex0: "10"; hex1: root.hex1; hex2: "12"; hex3: "34"
                    filmstrip: "knobs/knob_48.png"; frameSize: 48
                }

                SyComboBox {
                    hex0: "10"; hex1: root.hex1; hex2: "12"; hex3: "35"
                }
            }
        }
    }
}
