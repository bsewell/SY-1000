import QtQuick

Rectangle {
    id: root
    color: "#1a1a1a"
    implicitWidth: 800
    implicitHeight: 480

    property string hex1: "01"
    property string hex2: "32"
    property color accentColor: Qt.rgba(0, 0.7, 0.8, 1)

    Column {
        anchors.fill: parent
        spacing: 0

        StompHeader {
            width: parent.width
            accentColor: root.accentColor
            title: "REVERB"
            powerHex0: "10"; powerHex1: root.hex1; powerHex2: root.hex2; powerHex3: "00"
        }

        Rectangle { width: parent.width; height: 1; color: "#333333" }

        Flickable {
            width: parent.width
            height: parent.height - 37
            contentHeight: contentCol.height + 24
            clip: true

            Column {
                id: contentCol
                width: parent.width
                spacing: 12
                topPadding: 12

                Flow {
                    width: parent.width - 24
                    x: 12
                    spacing: 12

                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "01"
                    }

                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "03"
                    }
                }

                Rectangle { width: parent.width - 24; height: 1; color: "#333333"; x: 12 }

                Flow {
                    width: parent.width - 24
                    x: 12
                    spacing: 12

                    FilmstripKnob {
                        hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "04"
                        filmstrip: "knobs/knob_56.png"; frameSize: 56
                    }

                    FilmstripKnob {
                        hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "02"
                        filmstrip: "knobs/knob_56.png"; frameSize: 56
                    }

                    FilmstripKnob {
                        hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0B"
                        filmstrip: "knobs/knob_48.png"; frameSize: 48
                    }

                    FilmstripKnob {
                        hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0D"
                        filmstrip: "knobs/knob_48.png"; frameSize: 48
                    }

                    FilmstripKnob {
                        hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "09"
                        filmstrip: "knobs/knob_48.png"; frameSize: 48
                    }

                    FilmstripKnob {
                        hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0A"
                        filmstrip: "knobs/knob_48.png"; frameSize: 48
                    }
                }

                Rectangle { width: parent.width - 24; height: 1; color: "#333333"; x: 12 }

                Flow {
                    width: parent.width - 24
                    x: 12
                    spacing: 12

                    FilmstripKnob {
                        hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "08"
                        filmstrip: "knobs/knob_56.png"; frameSize: 56
                    }

                    FilmstripKnob {
                        hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0C"
                        filmstrip: "knobs/knob_56.png"; frameSize: 56
                    }
                }
            }
        }
    }
}
