import QtQuick

Rectangle {
    id: root
    color: "#1a1a1a"
    implicitWidth: 800
    implicitHeight: 480

    property string hex1: "00"
    property string hex2: "36"
    property color accentColor: Qt.rgba(0.2, 0.4, 1.0, 1)

    Column {
        anchors.fill: parent
        spacing: 0

        StompHeader {
            width: parent.width
            accentColor: root.accentColor
            title: "COMPRESSOR"
            powerHex0: "10"; powerHex1: root.hex1; powerHex2: root.hex2; powerHex3: "00"
        }

        Rectangle { width: parent.width; height: 1; color: "#333333" }

        Item {
            width: parent.width
            height: parent.height - 37

            Column {
                anchors.centerIn: parent
                spacing: 16

                // Type combo
                Row {
                    anchors.horizontalCenter: parent.horizontalCenter

                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "01"
                    }
                }

                // Knobs row
                Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 20

                    FilmstripKnob {
                        hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "03"
                        filmstrip: "knobs/knob_56.png"; frameSize: 56
                    }

                    FilmstripKnob {
                        hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "02"
                        filmstrip: "knobs/knob_56.png"; frameSize: 56
                    }

                    FilmstripKnob {
                        hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "06"
                        filmstrip: "knobs/knob_56.png"; frameSize: 56
                    }

                    FilmstripKnob {
                        hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "04"
                        filmstrip: "knobs/knob_56.png"; frameSize: 56
                    }

                    FilmstripKnob {
                        hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "05"
                        filmstrip: "knobs/knob_56.png"; frameSize: 56
                    }
                }
            }
        }
    }
}
