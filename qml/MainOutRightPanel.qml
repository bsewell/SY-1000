import QtQuick

Rectangle {
    id: root
    color: "#1a1a1a"
    implicitWidth: 800
    implicitHeight: 480

    property string hex1: "00"
    property string hex2: "00"
    property color accentColor: Qt.rgba(0.6, 0.2, 0.8, 1)

    property string sys1: hex1 === "02" ? "0B" : "04"

    property int currentTab: 0

    Column {
        anchors.fill: parent
        spacing: 0

        // Header bar — title + OUTPUT SELECT / GLOBAL EQ tabs
        Rectangle {
            width: parent.width
            height: 40
            color: Qt.rgba(root.accentColor.r * 0.35, root.accentColor.g * 0.35, root.accentColor.b * 0.35, 1.0)

            Text {
                anchors.left: parent.left
                anchors.leftMargin: 12
                anchors.verticalCenter: parent.verticalCenter
                text: "MAIN OUT RIGHT"
                color: "#ffffff"
                font.pixelSize: 16
                font.family: "Roboto Condensed"
                font.bold: true
            }

            Row {
                anchors.right: parent.right
                anchors.rightMargin: 12
                anchors.verticalCenter: parent.verticalCenter
                spacing: 8

                Rectangle {
                    width: outSelText.implicitWidth + 32
                    height: 26
                    radius: 3
                    color: "transparent"
                    border.color: root.currentTab === 0 ? "#ffffff" : "#666"
                    border.width: 1

                    Text {
                        id: outSelText
                        anchors.centerIn: parent
                        text: "OUTPUT SELECT"
                        color: root.currentTab === 0 ? "#ffffff" : "#999999"
                        font.pixelSize: 11
                        font.family: "Roboto Condensed"
                        font.bold: root.currentTab === 0
                    }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: root.currentTab = 0
                    }
                }

                Rectangle {
                    width: eqText.implicitWidth + 32
                    height: 26
                    radius: 3
                    color: "transparent"
                    border.color: root.currentTab === 1 ? "#ffffff" : "#666"
                    border.width: 1

                    Text {
                        id: eqText
                        anchors.centerIn: parent
                        text: "GLOBAL EQ"
                        color: root.currentTab === 1 ? "#ffffff" : "#999999"
                        font.pixelSize: 11
                        font.family: "Roboto Condensed"
                        font.bold: root.currentTab === 1
                    }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: root.currentTab = 1
                    }
                }
            }
        }

        Rectangle { width: parent.width; height: 1; color: "#333333" }

        // Content area
        Item {
            width: parent.width
            height: parent.height - 41

            // OUTPUT SELECT tab
            Flickable {
                anchors.fill: parent
                contentHeight: outCol.height + 24
                clip: true
                visible: root.currentTab === 0
                interactive: contentHeight > height

                Column {
                    id: outCol
                    width: parent.width
                    spacing: 12
                    topPadding: 16

                    Row {
                        x: 8
                        spacing: 16

                        FilmstripKnob {
                            hex0: "00"; hex1: root.sys1; hex2: "00"; hex3: "2E"
                            filmstrip: "knobs/knob_56.png"; frameSize: 56
                        }

                        Column {
                            anchors.verticalCenter: parent.verticalCenter
                            spacing: 8

                            SyComboBox {
                                hex0: "00"; hex1: root.sys1; hex2: "00"; hex3: "01"
                            }
                        }
                    }

                    Rectangle { width: parent.width - 16; height: 1; color: "#2a2a2a"; x: 8 }

                    Flow {
                        width: parent.width - 16; x: 8; spacing: 8

                        SyComboBox {
                            hex0: "10"; hex1: root.hex1; hex2: "12"; hex3: "38"
                        }
                    }
                }
            }

            // GLOBAL EQ tab
            Flickable {
                anchors.fill: parent
                contentHeight: eqCol.height + 24
                clip: true
                visible: root.currentTab === 1
                interactive: contentHeight > height

                Column {
                    id: eqCol
                    width: parent.width
                    spacing: 8
                    topPadding: 12

                    ParaEqGraph {
                        id: eqGraph
                        width: parent.width - 16; height: 200; x: 8
                        hex0: "00"; hex1: root.sys1; hex2: "00"; baseHex3: "0C"
                    }

                    Rectangle { width: parent.width - 16; height: 1; color: "#2a2a2a"; x: 8 }

                    ParaEqBands {
                        width: parent.width
                        hex0: "00"; hex1: root.sys1; hex2: "00"; baseHex3: "0C"
                        eqGraph: eqGraph
                    }
                }
            }
        }
    }
}
