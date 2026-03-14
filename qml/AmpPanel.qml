import QtQuick

Rectangle {
    id: root
    color: "#1a1a1a"
    implicitWidth: 800
    implicitHeight: 480

    property string hex1: "00"
    property string hex2: "38"
    property color accentColor: Qt.rgba(0.9, 0.8, 0.0, 1)

    Column {
        anchors.fill: parent
        spacing: 0

        StompHeader {
            width: parent.width
            accentColor: root.accentColor
            title: "AMP"
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
                spacing: 8
                topPadding: 12

                // Section label
                Text {
                    x: 12
                    text: "AMP"
                    color: "#666666"
                    font.pixelSize: 10
                    font.family: "Roboto Condensed"
                    font.capitalization: Font.AllUppercase
                }

                // Row 1: Amp type + effect level + bright + solo
                Flow {
                    width: parent.width - 24
                    x: 12
                    spacing: 12

                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "01"
                    }

                    FilmstripKnob {
                        hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "05"
                        filmstrip: "knobs/knob_48.png"; frameSize: 48
                    }

                    SySwitch {
                        hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0A"
                    }

                    SySwitch {
                        hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0C"
                    }

                    FilmstripKnob {
                        hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0D"
                        filmstrip: "knobs/knob_48.png"; frameSize: 48
                    }
                }

                // Divider
                Rectangle { width: parent.width - 24; height: 1; color: "#333333"; x: 12 }

                // Section label
                Text {
                    x: 12
                    text: "TONE"
                    color: "#666666"
                    font.pixelSize: 10
                    font.family: "Roboto Condensed"
                    font.capitalization: Font.AllUppercase
                }

                // Row 2: Gain section + tone stack
                Flow {
                    width: parent.width - 24
                    x: 12
                    spacing: 12

                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0B"
                    }

                    FilmstripKnob {
                        hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "02"
                        filmstrip: "knobs/knob_48_gain.png"; frameSize: 48
                    }

                    FilmstripKnob {
                        hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "03"
                        filmstrip: "knobs/knob_48.png"; frameSize: 48
                    }

                    FilmstripKnob {
                        hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "04"
                        filmstrip: "knobs/knob_48.png"; frameSize: 48
                    }

                    FilmstripKnob {
                        hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "06"
                        filmstrip: "knobs/knob_48.png"; frameSize: 48
                    }

                    FilmstripKnob {
                        hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "07"
                        filmstrip: "knobs/knob_48.png"; frameSize: 48
                    }

                    FilmstripKnob {
                        hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "08"
                        filmstrip: "knobs/knob_48.png"; frameSize: 48
                    }

                    FilmstripKnob {
                        hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "09"
                        filmstrip: "knobs/knob_48.png"; frameSize: 48
                    }
                }

                // Divider
                Rectangle { width: parent.width - 24; height: 1; color: "#333333"; x: 12 }

                // Section label
                Text {
                    x: 12
                    text: "SPEAKER"
                    color: "#666666"
                    font.pixelSize: 10
                    font.family: "Roboto Condensed"
                    font.capitalization: Font.AllUppercase
                }

                // Row 3: Speaker section
                Flow {
                    width: parent.width - 24
                    x: 12
                    spacing: 12

                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0E"
                    }

                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0F"
                    }

                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "10"
                    }

                    FilmstripKnob {
                        hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "11"
                        filmstrip: "knobs/knob_48.png"; frameSize: 48
                    }

                    FilmstripKnob {
                        hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "12"
                        filmstrip: "knobs/knob_48.png"; frameSize: 48
                    }

                    FilmstripKnob {
                        hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "13"
                        filmstrip: "knobs/knob_48.png"; frameSize: 48
                    }
                }
            }
        }
    }
}
