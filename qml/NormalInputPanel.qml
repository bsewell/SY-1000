import QtQuick

Rectangle {
    id: root
    color: "#1a1a1a"
    implicitWidth: 800
    implicitHeight: 480

    property string hex1: "00"  // guitar="00", bass="02"
    property string hex2: "12"  // normal pickup always "12"
    property string instLabel: "NORMAL INPUT"
    property color accentColor: Qt.rgba(0.2, 0.4, 1.0, 1)

    Column {
        anchors.fill: parent
        spacing: 0

        StompHeader {
            width: parent.width
            accentColor: root.accentColor
            title: root.instLabel
            powerHex0: "10"; powerHex1: root.hex1; powerHex2: "12"; powerHex3: "01"
        }

        // Divider
        Rectangle {
            width: parent.width
            height: 1
            color: "#333333"
        }

        // Content area
        Item {
            width: parent.width
            height: parent.height - 37

            Row {
                anchors.centerIn: parent
                spacing: 24

                // Level knob
                FilmstripKnob {
                    hex0: "10"; hex1: root.hex1; hex2: "12"; hex3: "02"
                    filmstrip: "knobs/knob_56.png"; frameSize: 56
                }

                // Cable Type
                SyComboBox {
                    hex0: "10"; hex1: root.hex1; hex2: "12"; hex3: "04"
                }

                // Phase
                SyComboBox {
                    hex0: "10"; hex1: root.hex1; hex2: "12"; hex3: "05"
                }
            }
        }
    }
}
