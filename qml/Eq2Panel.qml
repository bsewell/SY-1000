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
                spacing: 4
                topPadding: 8

                ParaEqGraph {
                    id: eqGraph
                    width: parent.width - 24; height: 200; x: 12
                    hex0: "10"; hex1: root.hex1; hex2: root.hex2; baseHex3: "01"
                }

                Rectangle { width: parent.width - 24; height: 1; color: "#2a2a2a"; x: 12 }

                ParaEqBands {
                    width: parent.width
                    hex0: "10"; hex1: root.hex1; hex2: root.hex2; baseHex3: "01"
                    eqGraph: eqGraph
                }
            }
        }
    }
}
