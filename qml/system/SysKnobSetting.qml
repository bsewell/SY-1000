import QtQuick
import ".."

Item {
    id: knobRoot
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

            // KNOB LOCK section
            Rectangle {
                width: parent.width
                height: 32
                color: "#313A47"
                Text {
                    x: 16; anchors.verticalCenter: parent.verticalCenter
                    text: "KNOB LOCK 1~6"
                    color: "#ffffff"; font.pixelSize: 14
                    font.family: "Roboto Condensed"
                }
            }

            Item {
                width: parent.width; height: 60
                Text {
                    x: 32; y: 20
                    text: "KNOB LOCK"
                    color: "rgba(255,255,255,0.8)"; font.pixelSize: 12
                    font.family: "Roboto Condensed"
                }
                SyComboBox {
                    x: 160; y: 14
                    hex0: "00"; hex1: knobRoot.panelHex1; hex2: "00"; hex3: "0C"
                }
            }

            // Divider
            Rectangle { width: parent.width - 32; height: 1; color: "rgba(255,255,255,0.15)"; x: 16 }

            // KNOB FUNCTION section
            Rectangle {
                width: parent.width
                height: 32
                color: "#313A47"
                Text {
                    x: 16; anchors.verticalCenter: parent.verticalCenter
                    text: "KNOB FUNCTION 1~6"
                    color: "#ffffff"; font.pixelSize: 14
                    font.family: "Roboto Condensed"
                }
            }

            // 6 knob function rows — each on its own line to avoid overlap
            Repeater {
                model: [
                    { label: "KNOB 1 SETTING", hex3: "0E" },
                    { label: "KNOB 2 SETTING", hex3: "12" },
                    { label: "KNOB 3 SETTING", hex3: "16" },
                    { label: "KNOB 4 SETTING", hex3: "1A" },
                    { label: "KNOB 5 SETTING", hex3: "1E" },
                    { label: "KNOB 6 SETTING", hex3: "22" }
                ]

                Item {
                    width: col.width; height: 44
                    Text {
                        x: 32; anchors.verticalCenter: parent.verticalCenter
                        text: modelData.label
                        color: "rgba(255,255,255,0.8)"; font.pixelSize: 12
                        font.family: "Roboto Condensed"
                    }
                    SyComboBox {
                        x: 200; y: 8
                        hex0: "00"; hex1: knobRoot.panelHex1; hex2: "00"; hex3: modelData.hex3
                    }
                }
            }
        }
    }
}
