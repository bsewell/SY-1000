import QtQuick

Rectangle {
    id: root
    color: "#1a1a1a"
    implicitWidth: 800
    implicitHeight: 480

    property string hex1: "00"
    property string hex2: "00"
    property color accentColor: Qt.rgba(0, 0.7, 0.2, 1)

    Column {
        anchors.fill: parent
        spacing: 0

        StompHeader {
            width: parent.width
            accentColor: root.accentColor
            title: "MASTER"
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

                // Page 1: Patch settings (hex0="10", hex1, hex2="12")
                Text {
                    x: 12; text: "PATCH"; color: "#666666"
                    font.pixelSize: 10; font.family: "Roboto Condensed"
                }

                Flow {
                    width: parent.width - 24; x: 12; spacing: 12

                    FilmstripKnob {
                        hex0: "10"; hex1: root.hex1; hex2: "12"; hex3: "3C"
                        filmstrip: "knobs/knob_56.png"; frameSize: 56
                    }

                    FilmstripKnob {
                        hex0: "10"; hex1: root.hex1; hex2: "12"; hex3: "3E"
                        filmstrip: "knobs/knob_48.png"; frameSize: 48
                    }

                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "12"; hex3: "43"
                    }

                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "12"; hex3: "42"
                    }

                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "12"; hex3: "00"
                    }
                }

                Rectangle { width: parent.width - 24; height: 1; color: "#2a2a2a"; x: 12 }

                Text {
                    x: 12; text: "GK / NORMAL SET"; color: "#666666"
                    font.pixelSize: 10; font.family: "Roboto Condensed"
                }

                Flow {
                    width: parent.width - 24; x: 12; spacing: 12

                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "02"; hex3: "00"
                    }

                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "02"; hex3: "01"
                    }
                }

                Rectangle { width: parent.width - 24; height: 1; color: "#2a2a2a"; x: 12 }

                // Page 2: MIDI channels (hex0="10", hex1, hex2="00")
                Text {
                    x: 12; text: "PATCH MIDI 1"; color: "#666666"
                    font.pixelSize: 10; font.family: "Roboto Condensed"
                }

                Flow {
                    width: parent.width - 24; x: 12; spacing: 12

                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "00"; hex3: "39"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "00"; hex3: "3E"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "00"; hex3: "3A"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "00"; hex3: "3C"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "00"; hex3: "40"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "00"; hex3: "42"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "00"; hex3: "43"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "00"; hex3: "45"
                    }
                }

                Rectangle { width: parent.width - 24; height: 1; color: "#2a2a2a"; x: 12 }

                Text {
                    x: 12; text: "PATCH MIDI 2"; color: "#666666"
                    font.pixelSize: 10; font.family: "Roboto Condensed"
                }

                Flow {
                    width: parent.width - 24; x: 12; spacing: 12

                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "00"; hex3: "46"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "00"; hex3: "4B"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "00"; hex3: "47"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "00"; hex3: "49"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "00"; hex3: "4D"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "00"; hex3: "4F"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "00"; hex3: "50"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "00"; hex3: "52"
                    }
                }

                Rectangle { width: parent.width - 24; height: 1; color: "#2a2a2a"; x: 12 }

                Text {
                    x: 12; text: "PATCH MIDI 3"; color: "#666666"
                    font.pixelSize: 10; font.family: "Roboto Condensed"
                }

                Flow {
                    width: parent.width - 24; x: 12; spacing: 12

                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "00"; hex3: "53"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "00"; hex3: "58"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "00"; hex3: "54"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "00"; hex3: "56"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "00"; hex3: "5A"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "00"; hex3: "5C"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "00"; hex3: "5D"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "00"; hex3: "5F"
                    }
                }

                Rectangle { width: parent.width - 24; height: 1; color: "#2a2a2a"; x: 12 }

                Text {
                    x: 12; text: "PATCH MIDI 4"; color: "#666666"
                    font.pixelSize: 10; font.family: "Roboto Condensed"
                }

                Flow {
                    width: parent.width - 24; x: 12; spacing: 12

                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "00"; hex3: "60"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "00"; hex3: "65"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "00"; hex3: "61"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "00"; hex3: "63"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "00"; hex3: "67"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "00"; hex3: "69"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "00"; hex3: "6A"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "00"; hex3: "6C"
                    }
                }

                Rectangle { width: parent.width - 24; height: 1; color: "#2a2a2a"; x: 12 }

                Text {
                    x: 12; text: "MIDI CC CONTROLLERS"; color: "#666666"
                    font.pixelSize: 10; font.family: "Roboto Condensed"
                }

                Flow {
                    width: parent.width - 24; x: 12; spacing: 12

                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "00"; hex3: "6D"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "00"; hex3: "6E"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "00"; hex3: "6F"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "00"; hex3: "70"
                    }
                }

                Rectangle { width: parent.width - 24; height: 1; color: "#2a2a2a"; x: 12 }

                // Page 3: Guitar to MIDI (hex0="10", hex1, hex2="14")
                Text {
                    x: 12; text: "GUITAR TO MIDI"; color: "#666666"
                    font.pixelSize: 10; font.family: "Roboto Condensed"
                }

                Flow {
                    width: parent.width - 24; x: 12; spacing: 12

                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "14"; hex3: "00"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "14"; hex3: "01"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "14"; hex3: "02"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "14"; hex3: "03"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "14"; hex3: "04"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "14"; hex3: "05"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "14"; hex3: "06"
                    }
                }

                Rectangle { width: parent.width - 24; height: 1; color: "#2a2a2a"; x: 12 }

                Text {
                    x: 12; text: "POLY MIDI PATCH"; color: "#666666"
                    font.pixelSize: 10; font.family: "Roboto Condensed"
                }

                Flow {
                    width: parent.width - 24; x: 12; spacing: 12

                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "00"; hex3: "71"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "00"; hex3: "73"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "00"; hex3: "75"
                    }
                }

                Rectangle { width: parent.width - 24; height: 1; color: "#2a2a2a"; x: 12 }

                Text {
                    x: 12; text: "STRING PATCHES"; color: "#666666"
                    font.pixelSize: 10; font.family: "Roboto Condensed"
                }

                Flow {
                    width: parent.width - 24; x: 12; spacing: 12

                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "00"; hex3: "77"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "00"; hex3: "79"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "00"; hex3: "7B"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "00"; hex3: "7D"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "01"; hex3: "01"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "01"; hex3: "03"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "01"; hex3: "05"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "01"; hex3: "07"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "01"; hex3: "09"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "01"; hex3: "0B"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "01"; hex3: "0D"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "01"; hex3: "0F"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "01"; hex3: "11"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "01"; hex3: "13"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "01"; hex3: "15"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "01"; hex3: "17"
                    }
                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: "01"; hex3: "19"
                    }
                }
            }
        }
    }
}
