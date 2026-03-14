import QtQuick

Rectangle {
    id: root
    color: "#1a1a1a"
    implicitWidth: 800
    implicitHeight: 480

    property string hex1: "00"
    property string hex2: "3B"
    property color accentColor: Qt.rgba(0, 0.7, 0.2, 1)

    Column {
        anchors.fill: parent
        spacing: 0

        StompHeader {
            width: parent.width
            accentColor: root.accentColor
            title: "EQ 2"
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

                Repeater {
                    model: 4

                    Column {
                        width: parent.width
                        spacing: 4

                        Text {
                            x: 12
                            text: "BAND " + (index + 1)
                            color: "#666666"
                            font.pixelSize: 10
                            font.family: "Roboto Condensed"
                        }

                        Flow {
                            width: parent.width - 24
                            x: 12
                            spacing: 12

                            SyComboBox {
                                hex0: "10"; hex1: root.hex1; hex2: root.hex2
                                hex3: {
                                    var v = 1 + index * 4
                                    var h = v.toString(16).toUpperCase()
                                    return h.length < 2 ? "0" + h : h
                                }
                            }

                            FilmstripKnob {
                                hex0: "10"; hex1: root.hex1; hex2: root.hex2
                                hex3: {
                                    var v = 2 + index * 4
                                    var h = v.toString(16).toUpperCase()
                                    return h.length < 2 ? "0" + h : h
                                }
                                filmstrip: "knobs/knob_48.png"; frameSize: 48
                            }

                            FilmstripKnob {
                                hex0: "10"; hex1: root.hex1; hex2: root.hex2
                                hex3: {
                                    var v = 3 + index * 4
                                    var h = v.toString(16).toUpperCase()
                                    return h.length < 2 ? "0" + h : h
                                }
                                filmstrip: "knobs/knob_48.png"; frameSize: 48
                            }

                            FilmstripKnob {
                                hex0: "10"; hex1: root.hex1; hex2: root.hex2
                                hex3: {
                                    var v = 4 + index * 4
                                    var h = v.toString(16).toUpperCase()
                                    return h.length < 2 ? "0" + h : h
                                }
                                filmstrip: "knobs/knob_48.png"; frameSize: 48
                            }
                        }

                        Rectangle {
                            width: parent.width - 24; height: 1; color: "#2a2a2a"; x: 12
                            visible: index < 3
                        }
                    }
                }

                Text {
                    x: 12
                    text: "CUT"
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
                }
            }
        }
    }
}
