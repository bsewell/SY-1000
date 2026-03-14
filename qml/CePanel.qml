import QtQuick

Rectangle {
    id: root
    color: "#1a1a1a"
    implicitWidth: 800
    implicitHeight: 480

    property string hex1: "00"
    property string hex2: "3F"
    property color accentColor: Qt.rgba(0, 0.7, 0.8, 1)

    property int ceMode: 0

    Component.onCompleted: {
        ceMode = paramBridge.getValue("10", hex1, hex2, "01")
    }

    Connections {
        target: modeCombo
        function onValueChanged() {
            root.ceMode = modeCombo.value
        }
    }

    Column {
        anchors.fill: parent
        spacing: 0

        StompHeader {
            width: parent.width
            accentColor: root.accentColor
            title: "CHORUS"
            powerHex0: "10"; powerHex1: root.hex1; powerHex2: root.hex2; powerHex3: "00"
        }

        Rectangle { width: parent.width; height: 1; color: "#333333" }

        // Mode selector
        Rectangle {
            width: parent.width
            height: 32
            color: "#2a2a2a"

            Row {
                anchors.fill: parent
                anchors.leftMargin: 12
                spacing: 8

                Text {
                    text: "MODE"
                    color: "#888888"
                    font.pixelSize: 11
                    font.family: "Roboto Condensed"
                    font.capitalization: Font.AllUppercase
                    anchors.verticalCenter: parent.verticalCenter
                }

                SyComboBox {
                    id: modeCombo
                    hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "01"
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }

        Rectangle { width: parent.width; height: 1; color: "#333333" }

        // Content area
        Flickable {
            width: parent.width
            height: parent.height - 70
            contentHeight: contentCol.height + 24
            clip: true

            Column {
                id: contentCol
                width: parent.width
                spacing: 8
                topPadding: 12

                // Single chorus controls (modes 0-2)
                Flow {
                    width: parent.width - 24
                    x: 12
                    spacing: 12
                    visible: root.ceMode <= 2

                    FilmstripKnob {
                        hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "02"
                        filmstrip: "knobs/knob_56.png"; frameSize: 56
                    }

                    FilmstripKnob {
                        hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "03"
                        filmstrip: "knobs/knob_56.png"; frameSize: 56
                    }

                    FilmstripKnob {
                        hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "04"
                        filmstrip: "knobs/knob_48.png"; frameSize: 48
                    }

                    FilmstripKnob {
                        hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "05"
                        filmstrip: "knobs/knob_56.png"; frameSize: 56
                    }

                    FilmstripKnob {
                        hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "06"
                        filmstrip: "knobs/knob_48.png"; frameSize: 48
                    }

                    FilmstripKnob {
                        hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "07"
                        filmstrip: "knobs/knob_48.png"; frameSize: 48
                    }

                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "08"
                    }

                    SyComboBox {
                        hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "09"
                    }

                    FilmstripKnob {
                        hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "18"
                        filmstrip: "knobs/knob_56.png"; frameSize: 56
                    }
                }

                // Dual chorus controls (mode 3+)
                Column {
                    width: parent.width
                    spacing: 8
                    visible: root.ceMode > 2

                    Text {
                        x: 12
                        text: "CHORUS 1"
                        color: "#666666"
                        font.pixelSize: 10
                        font.family: "Roboto Condensed"
                    }

                    Flow {
                        width: parent.width - 24
                        x: 12
                        spacing: 12

                        FilmstripKnob {
                            hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0A"
                            filmstrip: "knobs/knob_48.png"; frameSize: 48
                        }

                        FilmstripKnob {
                            hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0B"
                            filmstrip: "knobs/knob_48.png"; frameSize: 48
                        }

                        FilmstripKnob {
                            hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0C"
                            filmstrip: "knobs/knob_48.png"; frameSize: 48
                        }

                        FilmstripKnob {
                            hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0D"
                            filmstrip: "knobs/knob_48.png"; frameSize: 48
                        }

                        FilmstripKnob {
                            hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0E"
                            filmstrip: "knobs/knob_48.png"; frameSize: 48
                        }

                        FilmstripKnob {
                            hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0F"
                            filmstrip: "knobs/knob_48.png"; frameSize: 48
                        }

                        SyComboBox {
                            hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "10"
                        }

                        SyComboBox {
                            hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "09"
                        }
                    }

                    Rectangle { width: parent.width - 24; height: 1; color: "#333333"; x: 12 }

                    Text {
                        x: 12
                        text: "CHORUS 2"
                        color: "#666666"
                        font.pixelSize: 10
                        font.family: "Roboto Condensed"
                    }

                    Flow {
                        width: parent.width - 24
                        x: 12
                        spacing: 12

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

                        FilmstripKnob {
                            hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "14"
                            filmstrip: "knobs/knob_48.png"; frameSize: 48
                        }

                        FilmstripKnob {
                            hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "15"
                            filmstrip: "knobs/knob_48.png"; frameSize: 48
                        }

                        FilmstripKnob {
                            hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "16"
                            filmstrip: "knobs/knob_48.png"; frameSize: 48
                        }

                        SyComboBox {
                            hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "17"
                        }

                        FilmstripKnob {
                            hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "18"
                            filmstrip: "knobs/knob_56.png"; frameSize: 56
                        }
                    }
                }
            }
        }
    }
}
