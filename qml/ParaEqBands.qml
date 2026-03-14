import QtQuick

// Reusable ParaEQ band columns aligned to match graph frequency layout
// Columns positioned under their corresponding frequency regions
Item {
    id: root

    property string hex0: "10"
    property string hex1: "00"
    property string hex2: "00"
    property string baseHex3: "01"

    // Optional reference to ParaEqGraph for bidirectional sync
    property var eqGraph: null
    property bool _ready: false
    Component.onCompleted: _ready = true

    // Graph → Knob: when graph nodes are dragged, update knob values
    Connections {
        target: root.eqGraph
        enabled: root._ready && root.eqGraph !== null
        function onLowGainChanged() {
            lowGainKnob.value = root.eqGraph.lowGain
            lowGainKnob.displayValue = paramBridge.getDisplayValue(lowGainKnob.hex0, lowGainKnob.hex1, lowGainKnob.hex2, lowGainKnob.hex3, root.eqGraph.lowGain)
        }
        function onLowCutChanged() {
            lowCutKnob.value = root.eqGraph.lowCut
            lowCutKnob.displayValue = paramBridge.getDisplayValue(lowCutKnob.hex0, lowCutKnob.hex1, lowCutKnob.hex2, lowCutKnob.hex3, root.eqGraph.lowCut)
        }
        function onLowMidFreqChanged() {
            lowMidFreqKnob.value = root.eqGraph.lowMidFreq
            lowMidFreqKnob.displayValue = paramBridge.getDisplayValue(lowMidFreqKnob.hex0, lowMidFreqKnob.hex1, lowMidFreqKnob.hex2, lowMidFreqKnob.hex3, root.eqGraph.lowMidFreq)
        }
        function onLowMidQChanged() {
            lowMidQKnob.value = root.eqGraph.lowMidQ
            lowMidQKnob.displayValue = paramBridge.getDisplayValue(lowMidQKnob.hex0, lowMidQKnob.hex1, lowMidQKnob.hex2, lowMidQKnob.hex3, root.eqGraph.lowMidQ)
        }
        function onLowMidGainChanged() {
            lowMidGainKnob.value = root.eqGraph.lowMidGain
            lowMidGainKnob.displayValue = paramBridge.getDisplayValue(lowMidGainKnob.hex0, lowMidGainKnob.hex1, lowMidGainKnob.hex2, lowMidGainKnob.hex3, root.eqGraph.lowMidGain)
        }
        function onHighMidFreqChanged() {
            highMidFreqKnob.value = root.eqGraph.highMidFreq
            highMidFreqKnob.displayValue = paramBridge.getDisplayValue(highMidFreqKnob.hex0, highMidFreqKnob.hex1, highMidFreqKnob.hex2, highMidFreqKnob.hex3, root.eqGraph.highMidFreq)
        }
        function onHighMidQChanged() {
            highMidQKnob.value = root.eqGraph.highMidQ
            highMidQKnob.displayValue = paramBridge.getDisplayValue(highMidQKnob.hex0, highMidQKnob.hex1, highMidQKnob.hex2, highMidQKnob.hex3, root.eqGraph.highMidQ)
        }
        function onHighMidGainChanged() {
            highMidGainKnob.value = root.eqGraph.highMidGain
            highMidGainKnob.displayValue = paramBridge.getDisplayValue(highMidGainKnob.hex0, highMidGainKnob.hex1, highMidGainKnob.hex2, highMidGainKnob.hex3, root.eqGraph.highMidGain)
        }
        function onHighGainChanged() {
            highGainKnob.value = root.eqGraph.highGain
            highGainKnob.displayValue = paramBridge.getDisplayValue(highGainKnob.hex0, highGainKnob.hex1, highGainKnob.hex2, highGainKnob.hex3, root.eqGraph.highGain)
        }
        function onHighCutChanged() {
            highCutKnob.value = root.eqGraph.highCut
            highCutKnob.displayValue = paramBridge.getDisplayValue(highCutKnob.hex0, highCutKnob.hex1, highCutKnob.hex2, highCutKnob.hex3, root.eqGraph.highCut)
        }
        function onLevelChanged() {
            levelKnob.value = root.eqGraph.level
            levelKnob.displayValue = paramBridge.getDisplayValue(levelKnob.hex0, levelKnob.hex1, levelKnob.hex2, levelKnob.hex3, root.eqGraph.level)
        }
    }

    function hexAdd(base, offset) {
        return (parseInt(base, 16) + offset).toString(16).toUpperCase().padStart(2, '0')
    }

    implicitHeight: lowCol.height

    // Match graph coordinate system:
    // Graph is at x:12, width: parent.width-24, marginLeft:44, marginRight:12
    // So plot area within ParaEqBands starts at 12+44=56, width=parent.width-80
    readonly property real plotLeft: 56
    readonly property real plotW: Math.max(1, width - 80)

    // Band center positions (matching ParaEqGraph curve computation)
    // Low shelf node: ~0.10 of plotW
    // Low mid center: 0.20 + (freq/30)*0.18, midpoint ~0.29
    // High mid center: 0.58 + (freq/30)*0.18, midpoint ~0.67
    // High shelf node: ~0.90 of plotW
    readonly property real knobW: 72  // approx knob column width

    // LOW band: centered at 0.10 of plotW
    Column {
        id: lowCol
        x: Math.max(0, root.plotLeft + root.plotW * 0.10 - root.knobW / 2)
        width: root.knobW
        spacing: 2
        Text {
            text: "LOW"; color: "#666666"; anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 10; font.family: "Roboto Condensed"
        }
        FilmstripKnob {
            id: lowCutKnob
            hex0: root.hex0; hex1: root.hex1; hex2: root.hex2
            hex3: root.hexAdd(root.baseHex3, 8)
            filmstrip: "knobs/knob_48.png"; frameSize: 48
            onValueChanged: if (root.eqGraph) root.eqGraph.lowCut = value
        }
        FilmstripKnob {
            id: lowGainKnob
            hex0: root.hex0; hex1: root.hex1; hex2: root.hex2
            hex3: root.hexAdd(root.baseHex3, 0)
            filmstrip: "knobs/knob_48.png"; frameSize: 48
            onValueChanged: if (root.eqGraph) root.eqGraph.lowGain = value
        }
    }

    // Divider after LOW
    Rectangle {
        x: root.plotLeft + root.plotW * 0.195
        width: 1; height: lowCol.height; color: "#2a2a2a"
    }

    // LOW MID band: centered at 0.29 of plotW
    Column {
        id: lowMidCol
        x: root.plotLeft + root.plotW * 0.29 - root.knobW / 2
        width: root.knobW
        spacing: 2
        Text {
            text: "LOW MID"; color: "#666666"; anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 10; font.family: "Roboto Condensed"
        }
        FilmstripKnob {
            id: lowMidFreqKnob
            hex0: root.hex0; hex1: root.hex1; hex2: root.hex2
            hex3: root.hexAdd(root.baseHex3, 2)
            filmstrip: "knobs/knob_48.png"; frameSize: 48
            onValueChanged: if (root.eqGraph) root.eqGraph.lowMidFreq = value
        }
        FilmstripKnob {
            id: lowMidQKnob
            hex0: root.hex0; hex1: root.hex1; hex2: root.hex2
            hex3: root.hexAdd(root.baseHex3, 3)
            filmstrip: "knobs/knob_48.png"; frameSize: 48
            onValueChanged: if (root.eqGraph) root.eqGraph.lowMidQ = value
        }
        FilmstripKnob {
            id: lowMidGainKnob
            hex0: root.hex0; hex1: root.hex1; hex2: root.hex2
            hex3: root.hexAdd(root.baseHex3, 4)
            filmstrip: "knobs/knob_48.png"; frameSize: 48
            onValueChanged: if (root.eqGraph) root.eqGraph.lowMidGain = value
        }
    }

    // Divider after LOW MID
    Rectangle {
        x: root.plotLeft + root.plotW * 0.48
        width: 1; height: lowCol.height; color: "#2a2a2a"
    }

    // HIGH MID band: centered at 0.67 of plotW
    Column {
        id: highMidCol
        x: root.plotLeft + root.plotW * 0.67 - root.knobW / 2
        width: root.knobW
        spacing: 2
        Text {
            text: "HIGH MID"; color: "#666666"; anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 10; font.family: "Roboto Condensed"
        }
        FilmstripKnob {
            id: highMidFreqKnob
            hex0: root.hex0; hex1: root.hex1; hex2: root.hex2
            hex3: root.hexAdd(root.baseHex3, 5)
            filmstrip: "knobs/knob_48.png"; frameSize: 48
            onValueChanged: if (root.eqGraph) root.eqGraph.highMidFreq = value
        }
        FilmstripKnob {
            id: highMidQKnob
            hex0: root.hex0; hex1: root.hex1; hex2: root.hex2
            hex3: root.hexAdd(root.baseHex3, 6)
            filmstrip: "knobs/knob_48.png"; frameSize: 48
            onValueChanged: if (root.eqGraph) root.eqGraph.highMidQ = value
        }
        FilmstripKnob {
            id: highMidGainKnob
            hex0: root.hex0; hex1: root.hex1; hex2: root.hex2
            hex3: root.hexAdd(root.baseHex3, 7)
            filmstrip: "knobs/knob_48.png"; frameSize: 48
            onValueChanged: if (root.eqGraph) root.eqGraph.highMidGain = value
        }
    }

    // Divider after HIGH MID
    Rectangle {
        x: root.plotLeft + root.plotW * 0.805
        width: 1; height: lowCol.height; color: "#2a2a2a"
    }

    // HIGH band: centered at 0.90 of plotW
    Column {
        id: highCol
        x: Math.min(root.plotLeft + root.plotW * 0.90 - root.knobW / 2, root.width - root.knobW * 2 - 8)
        width: root.knobW
        spacing: 2
        Text {
            text: "HIGH"; color: "#666666"; anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 10; font.family: "Roboto Condensed"
        }
        FilmstripKnob {
            id: highCutKnob
            hex0: root.hex0; hex1: root.hex1; hex2: root.hex2
            hex3: root.hexAdd(root.baseHex3, 9)
            filmstrip: "knobs/knob_48.png"; frameSize: 48
            onValueChanged: if (root.eqGraph) root.eqGraph.highCut = value
        }
        FilmstripKnob {
            id: highGainKnob
            hex0: root.hex0; hex1: root.hex1; hex2: root.hex2
            hex3: root.hexAdd(root.baseHex3, 1)
            filmstrip: "knobs/knob_48.png"; frameSize: 48
            onValueChanged: if (root.eqGraph) root.eqGraph.highGain = value
        }
    }

    // LEVEL: right edge
    Column {
        id: levelCol
        x: root.width - root.knobW - 4
        width: root.knobW
        spacing: 2
        Text {
            text: "LEVEL"; color: "#666666"; anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 10; font.family: "Roboto Condensed"
        }
        FilmstripKnob {
            id: levelKnob
            hex0: root.hex0; hex1: root.hex1; hex2: root.hex2
            hex3: root.hexAdd(root.baseHex3, 10)
            filmstrip: "knobs/knob_48.png"; frameSize: 48
            onValueChanged: if (root.eqGraph) root.eqGraph.level = value
        }
    }
}
