import QtQuick

// ParaEQ knob columns aligned under graph frequency regions
// 4 columns: LOW | LOW MID | HIGH MID | HIGH (cut + gain stacked)
Item {
    id: root

    property string hex0: "10"
    property string hex1: "00"
    property string hex2: "00"
    property string baseHex3: "01"

    property var eqGraph: null
    property bool _ready: false
    Component.onCompleted: _ready = true

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
    }

    function hexAdd(base, offset) {
        return (parseInt(base, 16) + offset).toString(16).toUpperCase().padStart(2, '0')
    }

    implicitHeight: lowMidCol.height

    // Graph coordinate system:
    // Plot area starts at plotLeft, width = plotW
    // Log-scale: 100Hz=0.233, 1kHz=0.566, 5kHz=0.799
    readonly property real plotLeft: 56
    readonly property real plotW: Math.max(1, width - 80)
    readonly property real knobW: 108

    // === LOW: centered under 20-100Hz (0 to 0.233) ===
    Column {
        id: lowCol
        x: Math.max(0, root.plotLeft + root.plotW * 0.117 - root.knobW / 2)
        width: root.knobW
        spacing: 2
        Text {
            text: "LOW"; color: SyTheme.textSection; anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: SyTheme.fontSmall; font.family: SyTheme.fontFamily
        }
        FilmstripKnob {
            id: lowCutKnob
            labelPosition: "left"
            hex0: root.hex0; hex1: root.hex1; hex2: root.hex2
            hex3: root.hexAdd(root.baseHex3, 8)
            filmstrip: "knobs/knob_48.png"; frameSize: 48
            onValueChanged: if (root.eqGraph) root.eqGraph.lowCut = value
        }
        FilmstripKnob {
            id: lowGainKnob
            labelPosition: "left"
            hex0: root.hex0; hex1: root.hex1; hex2: root.hex2
            hex3: root.hexAdd(root.baseHex3, 0)
            filmstrip: "knobs/knob_48.png"; frameSize: 48
            onValueChanged: if (root.eqGraph) root.eqGraph.lowGain = value
        }
    }

    // Divider at 100Hz
    Rectangle {
        x: root.plotLeft + root.plotW * 0.233
        width: 1; height: lowMidCol.height; color: SyTheme.bgControl
    }

    // === LOW MID: centered under 100Hz-1kHz (0.233 to 0.566) ===
    Column {
        id: lowMidCol
        x: root.plotLeft + root.plotW * 0.40 - root.knobW / 2
        width: root.knobW
        spacing: 2
        Text {
            text: "LOW MID"; color: SyTheme.textSection; anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: SyTheme.fontSmall; font.family: SyTheme.fontFamily
        }
        FilmstripKnob {
            id: lowMidFreqKnob
            labelPosition: "left"
            hex0: root.hex0; hex1: root.hex1; hex2: root.hex2
            hex3: root.hexAdd(root.baseHex3, 2)
            filmstrip: "knobs/knob_48.png"; frameSize: 48
            onValueChanged: if (root.eqGraph) root.eqGraph.lowMidFreq = value
        }
        FilmstripKnob {
            id: lowMidQKnob
            labelPosition: "left"
            hex0: root.hex0; hex1: root.hex1; hex2: root.hex2
            hex3: root.hexAdd(root.baseHex3, 3)
            filmstrip: "knobs/knob_48.png"; frameSize: 48
            onValueChanged: if (root.eqGraph) root.eqGraph.lowMidQ = value
        }
        FilmstripKnob {
            id: lowMidGainKnob
            labelPosition: "left"
            hex0: root.hex0; hex1: root.hex1; hex2: root.hex2
            hex3: root.hexAdd(root.baseHex3, 4)
            filmstrip: "knobs/knob_48.png"; frameSize: 48
            onValueChanged: if (root.eqGraph) root.eqGraph.lowMidGain = value
        }
    }

    // Divider at 1kHz
    Rectangle {
        x: root.plotLeft + root.plotW * 0.566
        width: 1; height: lowMidCol.height; color: SyTheme.bgControl
    }

    // === HIGH MID: centered under 1kHz-5kHz (0.566 to 0.799) ===
    Column {
        id: highMidCol
        x: root.plotLeft + root.plotW * 0.683 - root.knobW / 2
        width: root.knobW
        spacing: 2
        Text {
            text: "HIGH MID"; color: SyTheme.textSection; anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: SyTheme.fontSmall; font.family: SyTheme.fontFamily
        }
        FilmstripKnob {
            id: highMidFreqKnob
            labelPosition: "left"
            hex0: root.hex0; hex1: root.hex1; hex2: root.hex2
            hex3: root.hexAdd(root.baseHex3, 5)
            filmstrip: "knobs/knob_48.png"; frameSize: 48
            onValueChanged: if (root.eqGraph) root.eqGraph.highMidFreq = value
        }
        FilmstripKnob {
            id: highMidQKnob
            labelPosition: "left"
            hex0: root.hex0; hex1: root.hex1; hex2: root.hex2
            hex3: root.hexAdd(root.baseHex3, 6)
            filmstrip: "knobs/knob_48.png"; frameSize: 48
            onValueChanged: if (root.eqGraph) root.eqGraph.highMidQ = value
        }
        FilmstripKnob {
            id: highMidGainKnob
            labelPosition: "left"
            hex0: root.hex0; hex1: root.hex1; hex2: root.hex2
            hex3: root.hexAdd(root.baseHex3, 7)
            filmstrip: "knobs/knob_48.png"; frameSize: 48
            onValueChanged: if (root.eqGraph) root.eqGraph.highMidGain = value
        }
    }

    // Divider at 5kHz
    Rectangle {
        x: root.plotLeft + root.plotW * 0.799
        width: 1; height: lowMidCol.height; color: SyTheme.bgControl
    }

    // === HIGH: cut on top, gain below — centered between 5kHz divider and right edge ===
    Column {
        id: highCol
        x: root.plotLeft + root.plotW * 0.799 + (root.width - (root.plotLeft + root.plotW * 0.799) - root.knobW) / 2
        width: root.knobW
        spacing: 2
        Text {
            text: "HIGH CUT"; color: SyTheme.textSection; anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: SyTheme.fontSmall; font.family: SyTheme.fontFamily
        }
        FilmstripKnob {
            id: highCutKnob
            labelPosition: "left"
            hex0: root.hex0; hex1: root.hex1; hex2: root.hex2
            hex3: root.hexAdd(root.baseHex3, 9)
            filmstrip: "knobs/knob_48.png"; frameSize: 48
            onValueChanged: if (root.eqGraph) root.eqGraph.highCut = value
        }
        Text {
            text: "HIGH GAIN"; color: SyTheme.textSection
            font.pixelSize: SyTheme.fontSmall; font.family: SyTheme.fontFamily
        }
        FilmstripKnob {
            id: highGainKnob
            labelPosition: "left"
            hex0: root.hex0; hex1: root.hex1; hex2: root.hex2
            hex3: root.hexAdd(root.baseHex3, 1)
            filmstrip: "knobs/knob_48.png"; frameSize: 48
            onValueChanged: if (root.eqGraph) root.eqGraph.highGain = value
        }
    }
}
