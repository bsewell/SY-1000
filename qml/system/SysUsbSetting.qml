import QtQuick
import ".."

Item {
    property string panelHex1: parent ? parent.panelHex1 : "01"
    property string panelHex1b: parent ? parent.panelHex1b : "04"

    Column {
        anchors.fill: parent
        spacing: 0
        topPadding: 16

        Text {
            x: 16
            text: "USB SETTING"
            color: "#888888"
            font.pixelSize: 11
            font.family: "Roboto Condensed"
            font.capitalization: Font.AllUppercase
        }

        Item { width: 1; height: 12 }

        Row {
            x: 24
            spacing: 24

            FilmstripKnob {
                hex0: "00"; hex1: panelHex1b; hex2: "00"; hex3: "41"
                filmstrip: "knobs/knob_56.png"; frameSize: 56
            }

            FilmstripKnob {
                hex0: "00"; hex1: panelHex1b; hex2: "00"; hex3: "42"
                filmstrip: "knobs/knob_56.png"; frameSize: 56
            }
        }

        Item { width: 1; height: 16 }

        Row {
            x: 24
            spacing: 16

            SyComboBox {
                hex0: "00"; hex1: panelHex1b; hex2: "00"; hex3: "43"
            }

            SyComboBox {
                hex0: "00"; hex1: panelHex1b; hex2: "00"; hex3: "44"
            }

            SyComboBox {
                hex0: "00"; hex1: panelHex1b; hex2: "00"; hex3: "45"
            }
        }
    }
}
