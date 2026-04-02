import QtQuick

Rectangle {
    id: root
    color: SyTheme.bgPanel
    property string hex1: "00"
    property string hex2a: "16"
    property string hex2b: "17"

    // Tab state: 0=PITCH, 1=CUTOFF, 2=LEVEL
    property int stepTab: 0
    // Curve tab: 0=SEQ1, 1=SEQ2
    property int curveTab: 0

    // Dynamic step cell width — fits 16 cells + label into available width
    readonly property real labelW: 36
    readonly property real gridSpacing: 1
    readonly property real stepCellW: Math.max(38, Math.floor((width - 16 - labelW - 15 * gridSpacing) / 16))

    // Compute hex2/hex3 for level steps that straddle the page boundary
    function levelStepAddr(byteOffset) {
        var addr = 0x7E + byteOffset
        if (addr > 0x7F) {
            return { hex2: root.hex2b, hex3: (addr - 0x80).toString(16).toUpperCase().padStart(2, '0') }
        }
        return { hex2: root.hex2a, hex3: addr.toString(16).toUpperCase().padStart(2, '0') }
    }

    // Step address helpers
    function pitchMaxHex3(step) { return (0x3E + step * 2).toString(16).toUpperCase().padStart(2, '0') }
    function pitchMinHex3(step) { return (0x3F + step * 2).toString(16).toUpperCase().padStart(2, '0') }
    function cutoffMaxHex3(step) { return (0x5E + step * 2).toString(16).toUpperCase().padStart(2, '0') }
    function cutoffMinHex3(step) { return (0x5F + step * 2).toString(16).toUpperCase().padStart(2, '0') }

    // Curve address: SEQ1 = hex2b 0x24+step, SEQ2 = hex2b 0x3A+step
    function curveHex3(seq, step) {
        var base = seq === 0 ? 0x24 : 0x3A
        return (base + step).toString(16).toUpperCase().padStart(2, '0')
    }

    Flickable {
        anchors.fill: parent
        anchors.margins: 8
        contentWidth: mainCol.width
        contentHeight: mainCol.height
        clip: true
        interactive: contentHeight > height

        Column {
            id: mainCol
            width: root.width - 16
            spacing: 8

            // ======== SECTION 1: Target selectors + BPM ========
            // Boss ref: 3 target combos + BPM knob in a row at top
            Row {
                spacing: 12

                SyComboBox {
                    hex0: "10"; hex1: root.hex1; hex2: root.hex2a; hex3: "3B"
                    implicitWidth: 180
                }
                SyComboBox {
                    hex0: "10"; hex1: root.hex1; hex2: root.hex2a; hex3: "3C"
                    implicitWidth: 180
                }
                SyComboBox {
                    hex0: "10"; hex1: root.hex1; hex2: root.hex2a; hex3: "3D"
                    implicitWidth: 180
                }
                FilmstripKnob {
                    hex0: "10"; hex1: root.hex1; hex2: "12"; hex3: "3E"
                    filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall
                }
            }

            // ======== SECTION 2: SEQ1 + SEQ2 side by side ========
            // Boss ref: SEQ1 left half, SEQ2 right half, each with header line
            Row {
                spacing: 12
                width: parent.width

                // --- SEQ1 ---
                Column {
                    width: (parent.width - 12) / 2
                    spacing: 4

                    Row {
                        width: parent.width
                        spacing: 8
                        Text {
                            text: "SEQ1"; color: SyTheme.accent
                            font.pixelSize: 12; font.family: "Roboto Condensed"; font.bold: true
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        Rectangle {
                            height: 1; width: parent.width - 50; color: "#444"
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }

                    Row {
                        spacing: 4
                        SySwitch { hex0: "10"; hex1: root.hex1; hex2: root.hex2b; hex3: "1E"; implicitWidth: 52 }
                        SySwitch { hex0: "10"; hex1: root.hex1; hex2: root.hex2b; hex3: "1F"; implicitWidth: 52 }
                        SySwitch { hex0: "10"; hex1: root.hex1; hex2: root.hex2b; hex3: "22"; implicitWidth: 52 }
                        SySwitch { hex0: "10"; hex1: root.hex1; hex2: root.hex2b; hex3: "23"; implicitWidth: 52 }
                        FilmstripKnob {
                            hex0: "10"; hex1: root.hex1; hex2: root.hex2b; hex3: "21"
                            filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall
                        }
                        FilmstripKnob {
                            hex0: "10"; hex1: root.hex1; hex2: root.hex2b; hex3: "20"
                            filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall
                        }
                    }
                }

                // --- SEQ2 ---
                Column {
                    width: (parent.width - 12) / 2
                    spacing: 4

                    Row {
                        width: parent.width
                        spacing: 8
                        Text {
                            text: "SEQ2"; color: SyTheme.accent
                            font.pixelSize: 12; font.family: "Roboto Condensed"; font.bold: true
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        Rectangle {
                            height: 1; width: parent.width - 50; color: "#444"
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }

                    Row {
                        spacing: 4
                        SySwitch { hex0: "10"; hex1: root.hex1; hex2: root.hex2b; hex3: "34"; implicitWidth: 52 }
                        SySwitch { hex0: "10"; hex1: root.hex1; hex2: root.hex2b; hex3: "35"; implicitWidth: 52 }
                        SySwitch { hex0: "10"; hex1: root.hex1; hex2: root.hex2b; hex3: "38"; implicitWidth: 52 }
                        SySwitch { hex0: "10"; hex1: root.hex1; hex2: root.hex2b; hex3: "39"; implicitWidth: 52 }
                        FilmstripKnob {
                            hex0: "10"; hex1: root.hex1; hex2: root.hex2b; hex3: "37"
                            filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall
                        }
                        FilmstripKnob {
                            hex0: "10"; hex1: root.hex1; hex2: root.hex2b; hex3: "36"
                            filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall
                        }
                    }
                }
            }

            // ======== SECTION 3: PITCH / CUTOFF / LEVEL tab bar ========
            // Boss ref: tab underline style, PITCH selected cyan
            Row {
                spacing: 0
                Repeater {
                    model: ["PITCH", "CUTOFF", "LEVEL"]
                    Rectangle {
                        width: tabLabel.implicitWidth + 24
                        height: 26
                        color: "transparent"
                        Rectangle {
                            anchors.bottom: parent.bottom
                            width: parent.width; height: 2; color: SyTheme.accent
                            visible: index === root.stepTab
                        }
                        Text {
                            id: tabLabel
                            anchors.centerIn: parent
                            text: modelData
                            color: index === root.stepTab ? SyTheme.accent : SyTheme.textDimmed
                            font.pixelSize: 11; font.family: "Roboto Condensed"
                            font.bold: index === root.stepTab
                        }
                        MouseArea { anchors.fill: parent; onClicked: root.stepTab = index }
                    }
                }
            }

            // ======== SECTION 4: Step grid — 16 columns, dynamic width ========
            // Boss ref: numbered columns 1-16, MAX row, MIN row
            Column {
                spacing: 1
                width: parent.width

                // Step numbers
                Row {
                    spacing: root.gridSpacing
                    Item { width: root.labelW; height: 1 }
                    Repeater {
                        model: 16
                        Text {
                            width: root.stepCellW
                            text: String(index + 1)
                            color: SyTheme.textDimmed
                            font.pixelSize: 9; font.family: "Roboto Condensed"
                            horizontalAlignment: Text.AlignHCenter
                        }
                    }
                }

                // MAX row
                Row {
                    spacing: root.gridSpacing
                    Text {
                        width: root.labelW; text: "MAX"
                        color: SyTheme.textLabel; font.pixelSize: 10; font.family: "Roboto Condensed"; font.bold: true
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    Repeater {
                        model: 16
                        SeqStepValue {
                            width: root.stepCellW
                            hex0: "10"; hex1: root.hex1
                            hex2: {
                                if (root.stepTab === 2) {
                                    var a = root.levelStepAddr(index * 2)
                                    return a.hex2
                                }
                                return root.hex2a
                            }
                            hex3: {
                                if (root.stepTab === 0) return root.pitchMaxHex3(index)
                                if (root.stepTab === 1) return root.cutoffMaxHex3(index)
                                var a = root.levelStepAddr(index * 2)
                                return a.hex3
                            }
                        }
                    }
                }

                // MIN row
                Row {
                    spacing: root.gridSpacing
                    Text {
                        width: root.labelW; text: "MIN"
                        color: SyTheme.textLabel; font.pixelSize: 10; font.family: "Roboto Condensed"; font.bold: true
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    Repeater {
                        model: 16
                        SeqStepValue {
                            width: root.stepCellW
                            hex0: "10"; hex1: root.hex1
                            hex2: {
                                if (root.stepTab === 2) {
                                    var a = root.levelStepAddr(index * 2 + 1)
                                    return a.hex2
                                }
                                return root.hex2a
                            }
                            hex3: {
                                if (root.stepTab === 0) return root.pitchMinHex3(index)
                                if (root.stepTab === 1) return root.cutoffMinHex3(index)
                                var a = root.levelStepAddr(index * 2 + 1)
                                return a.hex3
                            }
                        }
                    }
                }
            }

            // ======== SECTION 5: SEQ1 / SEQ2 curve tab bar ========
            // Boss ref: SEQ1/SEQ2 tabs switch the curve strip
            Row {
                spacing: 0
                Repeater {
                    model: ["SEQ1", "SEQ2"]
                    Rectangle {
                        width: curveTabLabel.implicitWidth + 24
                        height: 26
                        color: "transparent"
                        Rectangle {
                            anchors.bottom: parent.bottom
                            width: parent.width; height: 2; color: SyTheme.accent
                            visible: index === root.curveTab
                        }
                        Text {
                            id: curveTabLabel
                            anchors.centerIn: parent
                            text: modelData
                            color: index === root.curveTab ? SyTheme.accent : SyTheme.textDimmed
                            font.pixelSize: 11; font.family: "Roboto Condensed"
                            font.bold: index === root.curveTab
                        }
                        MouseArea { anchors.fill: parent; onClicked: root.curveTab = index }
                    }
                }
            }

            // ======== SECTION 6: Curve visualization — 16 cells ========
            // Boss ref: "CURVE" label + 16 rectangular cells showing transition shapes
            Row {
                spacing: root.gridSpacing

                Text {
                    width: root.labelW; text: "CURVE"
                    color: SyTheme.textLabel; font.pixelSize: 9; font.family: "Roboto Condensed"; font.bold: true
                    anchors.verticalCenter: parent.verticalCenter
                }

                Repeater {
                    model: 16
                    Rectangle {
                        id: curveCell
                        width: root.stepCellW
                        height: 40
                        color: "#222"
                        border.color: "#444"
                        border.width: 1
                        radius: 2

                        property string curveAddr: root.curveHex3(root.curveTab, index)
                        property int curveVal: 0

                        function reload() {
                            curveVal = paramBridge.getValue("10", root.hex1, root.hex2b, curveAddr)
                        }

                        Component.onCompleted: reload()
                        onCurveAddrChanged: reload()

                        Image {
                            anchors.centerIn: parent
                            width: parent.width - 4
                            height: parent.height - 4
                            source: "qrc:/images/seq_curve_" + curveCell.curveVal + ".png"
                            fillMode: Image.PreserveAspectFit
                            opacity: 0.8
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                curveCell.curveVal = (curveCell.curveVal + 1) % 13
                                paramBridge.setValue("10", root.hex1, root.hex2b, curveCell.curveAddr, curveCell.curveVal)
                            }
                        }
                    }
                }
            }
        }
    }
}
