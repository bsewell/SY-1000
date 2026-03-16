import QtQuick
import ".."

Item {
    id: knobRoot
    property string panelHex1: parent ? parent.panelHex1 : "01"
    property string panelHex1b: parent ? parent.panelHex1b : "04"

    Column {
        anchors.fill: parent
        spacing: 0
        topPadding: 16

        // Section: Knob Lock
        Text {
            x: 16
            text: "KNOB LOCK 1~6"
            color: "#888888"
            font.pixelSize: 11
            font.family: "Roboto Condensed"
            font.capitalization: Font.AllUppercase
        }

        Item { width: 1; height: 12 }

        Row {
            x: 24
            spacing: 16

            SyComboBox {
                hex0: "00"; hex1: knobRoot.panelHex1; hex2: "00"; hex3: "0C"
            }
        }

        Item { width: 1; height: 24 }
        Rectangle { width: parent.width - 32; height: 1; color: "#2a2a2a"; x: 16 }
        Item { width: 1; height: 16 }

        // Section: Knob Function 1~6
        Text {
            x: 16
            text: "KNOB FUNCTION 1~6"
            color: "#888888"
            font.pixelSize: 11
            font.family: "Roboto Condensed"
            font.capitalization: Font.AllUppercase
        }

        Item { width: 1; height: 12 }

        Flickable {
            width: parent.width
            height: parent.height - 130
            contentHeight: knobCol.height + 16
            clip: true

            Column {
                id: knobCol
                width: parent.width
                spacing: 8

                // Knob 1-6 function combos (data_4Byte)
                Row {
                    x: 24; spacing: 16
                    SyComboBox { hex0: "00"; hex1: knobRoot.panelHex1; hex2: "00"; hex3: "0E" }
                    SyComboBox { hex0: "00"; hex1: knobRoot.panelHex1; hex2: "00"; hex3: "12" }
                }
                Row {
                    x: 24; spacing: 16
                    SyComboBox { hex0: "00"; hex1: knobRoot.panelHex1; hex2: "00"; hex3: "16" }
                    SyComboBox { hex0: "00"; hex1: knobRoot.panelHex1; hex2: "00"; hex3: "1A" }
                }
                Row {
                    x: 24; spacing: 16
                    SyComboBox { hex0: "00"; hex1: knobRoot.panelHex1; hex2: "00"; hex3: "1E" }
                    SyComboBox { hex0: "00"; hex1: knobRoot.panelHex1; hex2: "00"; hex3: "22" }
                }
            }
        }
    }
}
