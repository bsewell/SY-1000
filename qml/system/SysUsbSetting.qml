import QtQuick
import ".."

Item {
    anchors.fill: parent
    property string panelHex1: parent ? parent.panelHex1 : "01"
    property string panelHex1b: parent ? parent.panelHex1b : "04"

    Flickable {
        anchors.fill: parent
        contentHeight: col.height + 32
        clip: true

        Column {
            id: col
            width: parent.width
            spacing: 0

            // Header
            Rectangle {
                width: parent.width; height: 32
                color: "#313A47"
                Text {
                    x: 16; anchors.verticalCenter: parent.verticalCenter
                    text: "USB SETTING"
                    color: "#ffffff"; font.pixelSize: 14
                    font.family: "Roboto Condensed"
                }
            }

            // AUDIO ROUTING
            Item {
                width: parent.width; height: 44
                Text {
                    x: 32; anchors.verticalCenter: parent.verticalCenter
                    text: "AUDIO ROUTING"
                    color: "rgba(255,255,255,0.8)"; font.pixelSize: 12
                    font.family: "Roboto Condensed"
                }
                SyComboBox {
                    x: 240; y: 8
                    hex0: "00"; hex1: panelHex1b; hex2: "00"; hex3: "43"
                }
            }

            // IN POSITION
            Item {
                width: parent.width; height: 44
                Text {
                    x: 32; anchors.verticalCenter: parent.verticalCenter
                    text: "IN POSITION"
                    color: "rgba(255,255,255,0.8)"; font.pixelSize: 12
                    font.family: "Roboto Condensed"
                }
                SyComboBox {
                    x: 240; y: 8
                    hex0: "00"; hex1: panelHex1b; hex2: "00"; hex3: "44"
                }
            }

            // OUT POSITION
            Item {
                width: parent.width; height: 44
                Text {
                    x: 32; anchors.verticalCenter: parent.verticalCenter
                    text: "OUT POSITION"
                    color: "rgba(255,255,255,0.8)"; font.pixelSize: 12
                    font.family: "Roboto Condensed"
                }
                SyComboBox {
                    x: 240; y: 8
                    hex0: "00"; hex1: panelHex1b; hex2: "00"; hex3: "45"
                }
            }

            Rectangle { width: parent.width - 32; height: 1; color: "rgba(255,255,255,0.15)"; x: 16 }
            Item { width: 1; height: 16 }

            // Level knobs row
            Row {
                x: 32
                spacing: 32

                FilmstripKnob {
                    hex0: "00"; hex1: panelHex1b; hex2: "00"; hex3: "41"
                    filmstrip: "knobs/knob_56.png"; frameSize: 56
                }

                FilmstripKnob {
                    hex0: "00"; hex1: panelHex1b; hex2: "00"; hex3: "42"
                    filmstrip: "knobs/knob_56.png"; frameSize: 56
                }
            }
        }
    }
}
