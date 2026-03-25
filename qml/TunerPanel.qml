import QtQuick

Rectangle {
    id: root
    anchors.fill: parent
    color: "#050505"
    implicitWidth: 800
    implicitHeight: 480

    property string hex1: "00"
    property string hex2: "00"
    property color accentColor: Qt.rgba(0.0, 0.85, 0.45, 1)

    // System hex addresses differ for tuner type vs ref pitch/output
    property string sysHex1: hex1 === "02" ? "08" : "01"
    property string sys2Hex1: hex1 === "02" ? "0B" : "04"

    // Tuner state
    property string currentNote: "A"
    property int currentOctave: 4
    property real centsOffset: 0.0
    property bool noteDetected: false
    property bool inTune: Math.abs(centsOffset) < 2.0

    // Strobe phase — drives the scrolling stripe offset
    property real strobePhase: 0.0

    // Demo simulation
    Timer {
        id: demoTimer
        interval: 33  // ~30fps
        running: true
        repeat: true
        property real demoTime: 0
        property real targetCents: 15
        property int noteIdx: 9
        property var noteNames: ["C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"]

        onTriggered: {
            demoTime += 0.033
            // Wandering pitch simulation — slowly approaches zero then drifts
            targetCents = Math.sin(demoTime * 0.4) * 25 + Math.sin(demoTime * 1.3) * 8
            root.centsOffset += (targetCents - root.centsOffset) * 0.08

            // Accumulate strobe phase based on cents offset
            root.strobePhase += root.centsOffset * 0.06

            // Occasional note change
            if (Math.random() < 0.002) {
                noteIdx = Math.floor(Math.random() * 12)
                root.currentNote = noteNames[noteIdx]
                root.currentOctave = 3 + Math.floor(Math.random() * 3)
            }
            root.noteDetected = true
        }
    }

    Column {
        anchors.fill: parent
        spacing: 0

        // ── Header bar ──
        Rectangle {
            width: parent.width
            height: 40
            color: Qt.rgba(root.accentColor.r * 0.25,
                           root.accentColor.g * 0.25,
                           root.accentColor.b * 0.25, 1.0)

            Text {
                anchors.left: parent.left
                anchors.leftMargin: 16
                anchors.verticalCenter: parent.verticalCenter
                text: "STROBE TUNER"
                color: "#ffffff"
                font.pixelSize: 16
                font.family: "Roboto Condensed"
                font.bold: true
                font.letterSpacing: 2
            }

            // In-tune indicator light
            Rectangle {
                anchors.right: parent.right
                anchors.rightMargin: 16
                anchors.verticalCenter: parent.verticalCenter
                width: 12; height: 12; radius: 6
                color: root.inTune && root.noteDetected
                       ? root.accentColor
                       : "#333333"
                border.color: Qt.rgba(root.accentColor.r, root.accentColor.g,
                                      root.accentColor.b, 0.4)
                border.width: 1

                Behavior on color { ColorAnimation { duration: 150 } }
            }
        }

        Rectangle { width: parent.width; height: 1; color: "#1a1a1a" }

        // ── Strobe display area ──
        Rectangle {
            width: parent.width
            height: 200
            color: "#020202"

            // Strobe bands — 5 bands at different harmonic multiples
            Column {
                anchors.fill: parent
                anchors.margins: 1
                spacing: 2

                Repeater {
                    model: 5
                    delegate: Item {
                        width: parent.width
                        height: (parent.height - 8) / 5

                        property int harmonic: index + 1
                        property real bandPhase: root.strobePhase * harmonic

                        clip: true

                        // Scrolling stripe row
                        Row {
                            x: -(bandPhase % 40) - 40
                            anchors.verticalCenter: parent.verticalCenter
                            spacing: 0

                            Repeater {
                                model: Math.ceil(parent.parent.width / 20) + 4
                                delegate: Rectangle {
                                    width: 20
                                    height: parent.parent.parent.height
                                    color: modelData % 2 === 0
                                           ? (root.inTune && root.noteDetected
                                              ? root.accentColor
                                              : Qt.rgba(root.accentColor.r * 0.7,
                                                        root.accentColor.g * 0.7,
                                                        root.accentColor.b * 0.7, 1.0))
                                           : "#050505"
                                }
                            }
                        }

                        // Fade edges
                        Rectangle {
                            anchors.left: parent.left
                            width: 60; height: parent.height
                            gradient: Gradient {
                                orientation: Gradient.Horizontal
                                GradientStop { position: 0.0; color: "#020202" }
                                GradientStop { position: 1.0; color: "transparent" }
                            }
                        }
                        Rectangle {
                            anchors.right: parent.right
                            width: 60; height: parent.height
                            gradient: Gradient {
                                orientation: Gradient.Horizontal
                                GradientStop { position: 0.0; color: "transparent" }
                                GradientStop { position: 1.0; color: "#020202" }
                            }
                        }
                    }
                }
            }

            // Center line overlay
            Rectangle {
                anchors.horizontalCenter: parent.horizontalCenter
                width: 2; height: parent.height
                color: Qt.rgba(1, 1, 1, 0.15)
            }

            // Glow when in tune
            Rectangle {
                anchors.fill: parent
                color: "transparent"
                border.color: root.inTune && root.noteDetected
                              ? Qt.rgba(root.accentColor.r, root.accentColor.g,
                                        root.accentColor.b, 0.3)
                              : "transparent"
                border.width: 2
                Behavior on border.color { ColorAnimation { duration: 200 } }
            }
        }

        Rectangle { width: parent.width; height: 1; color: "#1a1a1a" }

        // ── Note display + cents bar ──
        Rectangle {
            width: parent.width
            height: 140
            color: "#0a0a0a"

            // Note name — large centered
            Text {
                id: noteText
                anchors.centerIn: parent
                text: root.noteDetected ? root.currentNote : "---"
                color: root.inTune && root.noteDetected
                       ? root.accentColor : "#ffffff"
                font.pixelSize: 72
                font.family: "Roboto Condensed"
                font.bold: true
                font.letterSpacing: 4

                Behavior on color { ColorAnimation { duration: 200 } }
            }

            // Octave number
            Text {
                anchors.left: noteText.right
                anchors.leftMargin: 4
                anchors.bottom: noteText.bottom
                anchors.bottomMargin: 8
                text: root.noteDetected ? root.currentOctave : ""
                color: "#666666"
                font.pixelSize: 24
                font.family: "Roboto Condensed"
            }

            // Cents readout
            Text {
                anchors.right: parent.right
                anchors.rightMargin: 20
                anchors.verticalCenter: parent.verticalCenter
                text: root.noteDetected
                      ? (root.centsOffset >= 0 ? "+" : "") + root.centsOffset.toFixed(1) + "¢"
                      : ""
                color: root.inTune ? root.accentColor : "#888888"
                font.pixelSize: 20
                font.family: "Roboto Condensed"
                font.bold: true

                Behavior on color { ColorAnimation { duration: 200 } }
            }

            // Flat/Sharp labels
            Text {
                anchors.left: parent.left
                anchors.leftMargin: 20
                anchors.verticalCenter: parent.verticalCenter
                text: "FLAT"
                color: root.centsOffset < -2 && root.noteDetected ? "#ff6666" : "#333333"
                font.pixelSize: 14
                font.family: "Roboto Condensed"
                font.bold: true
                font.letterSpacing: 2
            }
            Text {
                anchors.right: parent.right
                anchors.rightMargin: 80
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: 24
                text: "SHARP"
                color: root.centsOffset > 2 && root.noteDetected ? "#ff6666" : "#333333"
                font.pixelSize: 14
                font.family: "Roboto Condensed"
                font.bold: true
                font.letterSpacing: 2
            }

            // Cents bar — horizontal bar graph
            Item {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 12
                width: 400
                height: 16

                // Background track
                Rectangle {
                    anchors.fill: parent
                    radius: 3
                    color: "#151515"
                    border.color: "#222222"
                    border.width: 1
                }

                // Tick marks
                Repeater {
                    model: 11  // -50 to +50 in steps of 10
                    Rectangle {
                        x: (parent.width / 10) * modelData - 1
                        y: 0
                        width: modelData === 5 ? 2 : 1
                        height: parent.height
                        color: modelData === 5 ? "#444444" : "#222222"
                    }
                }

                // Active bar — grows from center
                Rectangle {
                    property real barWidth: Math.abs(root.centsOffset) / 50.0 * (parent.width / 2)
                    x: root.centsOffset >= 0
                       ? parent.width / 2
                       : parent.width / 2 - barWidth
                    y: 2
                    width: barWidth
                    height: parent.height - 4
                    radius: 2
                    color: root.inTune
                           ? root.accentColor
                           : (Math.abs(root.centsOffset) > 20 ? "#ff4444" : "#ffaa00")

                    Behavior on color { ColorAnimation { duration: 200 } }
                }

                // Center needle
                Rectangle {
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: 2; height: parent.height + 4
                    y: -2
                    color: "#ffffff"
                }
            }
        }

        Rectangle { width: parent.width; height: 1; color: "#1a1a1a" }

        // ── Settings row ──
        Rectangle {
            width: parent.width
            height: parent.height - 383
            color: "#111111"

            Row {
                anchors.fill: parent
                anchors.leftMargin: 16
                anchors.rightMargin: 16
                spacing: 24

                // Tuner Type
                Column {
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 4

                    Text {
                        text: "TUNER TYPE"
                        color: "#666666"
                        font.pixelSize: 10
                        font.family: "Roboto Condensed"
                        font.letterSpacing: 1
                    }
                    SyComboBox {
                        hex0: "00"; hex1: root.sysHex1; hex2: "00"; hex3: "04"
                    }
                }

                // Reference Pitch
                Column {
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 4

                    Text {
                        text: "REFERENCE PITCH"
                        color: "#666666"
                        font.pixelSize: 10
                        font.family: "Roboto Condensed"
                        font.letterSpacing: 1
                    }
                    SyComboBox {
                        hex0: "00"; hex1: root.sys2Hex1; hex2: "20"; hex3: "03"
                    }
                }

                // Output
                Column {
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 4

                    Text {
                        text: "OUTPUT"
                        color: "#666666"
                        font.pixelSize: 10
                        font.family: "Roboto Condensed"
                        font.letterSpacing: 1
                    }
                    SyComboBox {
                        hex0: "00"; hex1: root.sys2Hex1; hex2: "20"; hex3: "06"
                    }
                }
            }
        }
    }
}
