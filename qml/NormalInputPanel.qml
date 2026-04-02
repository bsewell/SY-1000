import QtQuick

Rectangle {
    id: root
    color: SyTheme.bgPanel
    implicitWidth: SyTheme.panelWidth
    implicitHeight: SyTheme.panelHeight

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

        Rectangle { width: parent.width; height: 1; color: SyTheme.divider }

        // Cable Sim row — matches Boss layout
        Rectangle {
            width: parent.width
            height: SyTheme.modeSelectorH
            color: SyTheme.bgControl

            Row {
                anchors.fill: parent
                anchors.leftMargin: 12
                spacing: 8

                Text {
                    text: "CABLE SIM"
                    color: SyTheme.textDimmed
                    font.pixelSize: SyTheme.fontLabel
                    font.family: SyTheme.fontFamily
                    font.capitalization: Font.AllUppercase
                    anchors.verticalCenter: parent.verticalCenter
                }

                SyComboBox {
                    hex0: "10"; hex1: root.hex1; hex2: "12"; hex3: "04"
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }

        Rectangle { width: parent.width; height: 1; color: SyTheme.divider }

        // Knobs — left-aligned, top-aligned
        // Total fixed rows above: StompHeader + divider + Cable Sim row (modeSelectorH) + divider
        Item {
            width: parent.width
            height: parent.height - SyTheme.headerHeight - 1 - SyTheme.modeSelectorH - 1

            Flow {
                x: 8
                y: 16
                width: parent.width - 16
                spacing: 16

                // Normal Level knob
                FilmstripKnob {
                    hex0: "10"; hex1: root.hex1; hex2: "12"; hex3: "02"
                    filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge
                }

                // Phase knob
                FilmstripKnob {
                    hex0: "10"; hex1: root.hex1; hex2: "12"; hex3: "05"
                    filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge
                }
            }
        }
    }
}
