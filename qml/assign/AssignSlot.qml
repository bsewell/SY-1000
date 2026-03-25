import QtQuick
import ".."

Item {
    id: slotRoot
    anchors.fill: parent

    property string panelHex1: parent ? parent.panelHex1 : "00"
    property string assignHex2: parent ? parent.assignHex2 : "04"
    property string assignBase: parent ? parent.assignBase : "00"
    property int assignNumber: parent ? parent.assignNumber : 1

    // Compute hex3 by adding offset to base
    function hex3(offset) {
        var base = parseInt(assignBase, 16)
        var val = base + offset
        var h = val.toString(16).toUpperCase()
        return h.length < 2 ? "0" + h : h
    }

    // Source value for conditional visibility
    property int sourceValue: 0

    Component.onCompleted: {
        sourceValue = paramBridge.getValue("10", panelHex1, assignHex2, hex3(0x0D))
    }

    Connections {
        target: sourceCombo
        function onValueChanged() {
            slotRoot.sourceValue = sourceCombo.value
        }
    }

    Flickable {
        anchors.fill: parent
        contentHeight: mainCol.height + 24
        clip: true

        Column {
            id: mainCol
            width: parent.width
            spacing: 0

            // Enable switch row
            Item {
                width: parent.width; height: 50
                Text {
                    x: 16; anchors.verticalCenter: parent.verticalCenter
                    text: "ASSIGN " + slotRoot.assignNumber
                    color: "#ccffffff"
                    font.pixelSize: 14; font.family: "Roboto Condensed"
                    font.bold: true
                }
                SySwitch {
                    x: 160; y: 6
                    hex0: "10"; hex1: slotRoot.panelHex1
                    hex2: slotRoot.assignHex2; hex3: slotRoot.hex3(0x00)
                }
            }

            Rectangle { width: parent.width - 32; height: 1; color: "#26ffffff"; x: 16 }

            // Target row
            Item {
                width: parent.width; height: 32
                Text {
                    x: 16; anchors.verticalCenter: parent.verticalCenter
                    text: "TARGET"
                    color: "#888888"
                    font.pixelSize: 12; font.family: "Roboto Condensed"
                }
                SyComboBox {
                    x: 160; width: parent.width - 176
                    anchors.verticalCenter: parent.verticalCenter
                    hex0: "10"; hex1: slotRoot.panelHex1
                    hex2: slotRoot.assignHex2; hex3: slotRoot.hex3(0x01)
                }
            }

            // Source row
            Item {
                width: parent.width; height: 32
                Text {
                    x: 16; anchors.verticalCenter: parent.verticalCenter
                    text: "SOURCE"
                    color: "#888888"
                    font.pixelSize: 12; font.family: "Roboto Condensed"
                }
                SyComboBox {
                    id: sourceCombo
                    x: 160; width: parent.width - 176
                    anchors.verticalCenter: parent.verticalCenter
                    hex0: "10"; hex1: slotRoot.panelHex1
                    hex2: slotRoot.assignHex2; hex3: slotRoot.hex3(0x0D)
                }
            }

            // Source Mode row
            Item {
                width: parent.width; height: 32
                Text {
                    x: 16; anchors.verticalCenter: parent.verticalCenter
                    text: "SOURCE MODE"
                    color: "#888888"
                    font.pixelSize: 12; font.family: "Roboto Condensed"
                }
                SyComboBox {
                    x: 160; width: parent.width - 176
                    anchors.verticalCenter: parent.verticalCenter
                    hex0: "10"; hex1: slotRoot.panelHex1
                    hex2: slotRoot.assignHex2; hex3: slotRoot.hex3(0x0E)
                }
            }

            Rectangle { width: parent.width - 32; height: 1; color: "#26ffffff"; x: 16 }

            // Range knobs
            Item {
                width: parent.width; height: 100

                Text {
                    x: 16; y: 8
                    text: "RANGE"
                    color: "#888888"
                    font.pixelSize: 12; font.family: "Roboto Condensed"
                }

                Row {
                    x: 16; y: 24
                    spacing: 16

                    FilmstripKnob {
                        hex0: "10"; hex1: slotRoot.panelHex1
                        hex2: slotRoot.assignHex2; hex3: slotRoot.hex3(0x0F)
                        filmstrip: "knobs/knob_48.png"; frameSize: 48
                    }
                }
            }

            Rectangle { width: parent.width - 32; height: 1; color: "#26ffffff"; x: 16 }

            // Switch timing (visible when source < 0x12, i.e. physical switches)
            Item {
                width: parent.width; height: 100
                visible: slotRoot.sourceValue < 0x12

                Text {
                    x: 16; y: 8
                    text: "SWITCH TIMING"
                    color: "#888888"
                    font.pixelSize: 12; font.family: "Roboto Condensed"
                }

                Row {
                    x: 16; y: 24
                    spacing: 16

                    FilmstripKnob {
                        hex0: "10"; hex1: slotRoot.panelHex1
                        hex2: slotRoot.assignHex2; hex3: slotRoot.hex3(0x17)
                        filmstrip: "knobs/knob_48.png"; frameSize: 48
                    }
                    FilmstripKnob {
                        hex0: "10"; hex1: slotRoot.panelHex1
                        hex2: slotRoot.assignHex2; hex3: slotRoot.hex3(0x18)
                        filmstrip: "knobs/knob_48.png"; frameSize: 48
                    }
                }
            }

            // Wave pedal (visible when source == 0x12, i.e. internal pedal)
            Item {
                width: parent.width; height: 100
                visible: slotRoot.sourceValue === 0x12

                Text {
                    x: 16; y: 8
                    text: "WAVE PEDAL"
                    color: "#888888"
                    font.pixelSize: 12; font.family: "Roboto Condensed"
                }

                Row {
                    x: 16; y: 24
                    spacing: 16

                    FilmstripKnob {
                        hex0: "10"; hex1: slotRoot.panelHex1
                        hex2: slotRoot.assignHex2; hex3: slotRoot.hex3(0x19)
                        filmstrip: "knobs/knob_48.png"; frameSize: 48
                    }
                    SyComboBox {
                        anchors.verticalCenter: parent.verticalCenter
                        hex0: "10"; hex1: slotRoot.panelHex1
                        hex2: slotRoot.assignHex2; hex3: slotRoot.hex3(0x1A)
                    }
                }
            }

            // Input Level (visible when source >= 0x12 AND source < 0x1B)
            Item {
                width: parent.width; height: 100
                visible: slotRoot.sourceValue >= 0x12 && slotRoot.sourceValue < 0x1B

                Text {
                    x: 16; y: 8
                    text: "INPUT LEVEL"
                    color: "#888888"
                    font.pixelSize: 12; font.family: "Roboto Condensed"
                }

                Row {
                    x: 16; y: 24
                    spacing: 16

                    FilmstripKnob {
                        hex0: "10"; hex1: slotRoot.panelHex1
                        hex2: "12"; hex3: "44"
                        filmstrip: "knobs/knob_48.png"; frameSize: 48
                    }
                }
            }

            // MIDI CC# (visible when source > 0x1A)
            Item {
                width: parent.width; height: visible ? 100 : 0
                visible: slotRoot.sourceValue > 0x1A

                Text {
                    x: 16; y: 8
                    text: "MIDI CC#"
                    color: "#888888"
                    font.pixelSize: 12; font.family: "Roboto Condensed"
                }

                Row {
                    x: 16; y: 24
                    spacing: 16

                    SyComboBox {
                        anchors.verticalCenter: parent.verticalCenter
                        hex0: "10"; hex1: slotRoot.panelHex1
                        hex2: slotRoot.assignHex2; hex3: slotRoot.hex3(0x1B)
                    }
                    FilmstripKnob {
                        hex0: "10"; hex1: slotRoot.panelHex1
                        hex2: slotRoot.assignHex2; hex3: slotRoot.hex3(0x1C)
                        filmstrip: "knobs/knob_48.png"; frameSize: 48
                    }
                    FilmstripKnob {
                        hex0: "10"; hex1: slotRoot.panelHex1
                        hex2: slotRoot.assignHex2; hex3: slotRoot.hex3(0x1D)
                        filmstrip: "knobs/knob_48.png"; frameSize: 48
                    }
                }
            }
        }
    }
}
