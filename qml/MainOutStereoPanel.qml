import QtQuick

Rectangle {
    id: root
    color: "#1a1a1a"
    implicitWidth: 800
    implicitHeight: 480

    property string hex1: "04"
    property string hex2: "00"
    property color accentColor: Qt.rgba(0.6, 0.2, 0.8, 1)

    Column {
        anchors.fill: parent
        spacing: 0

        StompHeader {
            width: parent.width
            accentColor: root.accentColor
            title: "MAIN OUT STEREO EQ"
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
                    hex0: "00"; hex1: root.hex1; hex2: "00"; baseHex3: "02"
                }

                Rectangle { width: parent.width - 24; height: 1; color: "#2a2a2a"; x: 12 }

                ParaEqBands {
                    width: parent.width
                    hex0: "00"; hex1: root.hex1; hex2: "00"; baseHex3: "02"
                    eqGraph: eqGraph
                }
            }
        }
    }
}
