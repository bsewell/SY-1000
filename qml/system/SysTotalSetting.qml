import QtQuick
import ".."

Item {
    anchors.fill: parent
    property string panelHex1: parent ? parent.panelHex1 : "01"
    property string panelHex1b: parent ? parent.panelHex1b : "04"

    // Header
    Rectangle {
        width: parent.width; height: 32
        color: "#313A47"
        Text {
            x: 16; anchors.verticalCenter: parent.verticalCenter
            text: "TOTAL SETTING"
            color: "#ffffff"; font.pixelSize: 14
            font.family: "Roboto Condensed"
        }
    }

    // Centered knobs — Boss layout shows 2 knobs centered at y=190
    Row {
        anchors.horizontalCenter: parent.horizontalCenter
        y: 160
        spacing: 48

        FilmstripKnob {
            hex0: "00"; hex1: panelHex1b; hex2: "00"; hex3: "3C"
            filmstrip: "knobs/knob_56.png"; frameSize: 56
        }

        FilmstripKnob {
            hex0: "00"; hex1: panelHex1b; hex2: "00"; hex3: "3D"
            filmstrip: "knobs/knob_56.png"; frameSize: 56
        }
    }
}
