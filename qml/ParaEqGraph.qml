import QtQuick

Canvas {
    id: root

    property string hex0: "10"
    property string hex1: "00"
    property string hex2: "00"
    property string baseHex3: "01"  // base address for 11 ParaEQ params

    // Computed hex3 addresses from base (same order as customParaEQGraph::updateSlot)
    // base+0=LowGain, base+1=HighGain, base+2=LowMidFreq, base+3=LowMidQ,
    // base+4=LowMidGain, base+5=HighMidFreq, base+6=HighMidQ, base+7=HighMidGain,
    // base+8=LowCut, base+9=HighCut, base+10=Level
    function hexAdd(base, offset) {
        return (parseInt(base, 16) + offset).toString(16).toUpperCase().padStart(2, '0')
    }

    property string h3LowGain:     hexAdd(baseHex3, 0)
    property string h3HighGain:    hexAdd(baseHex3, 1)
    property string h3LowMidFreq:  hexAdd(baseHex3, 2)
    property string h3LowMidQ:     hexAdd(baseHex3, 3)
    property string h3LowMidGain:  hexAdd(baseHex3, 4)
    property string h3HighMidFreq: hexAdd(baseHex3, 5)
    property string h3HighMidQ:    hexAdd(baseHex3, 6)
    property string h3HighMidGain: hexAdd(baseHex3, 7)
    property string h3LowCut:     hexAdd(baseHex3, 8)
    property string h3HighCut:    hexAdd(baseHex3, 9)
    property string h3Level:      hexAdd(baseHex3, 10)

    // Raw parameter values (0-127 range from SysEx)
    property int lowCut: 0
    property int lowGain: 32
    property int lowMidFreq: 15
    property int lowMidQ: 3
    property int lowMidGain: 32
    property int highMidFreq: 15
    property int highMidQ: 3
    property int highMidGain: 32
    property int highGain: 32
    property int highCut: 0
    property int level: 32

    implicitWidth: 400
    implicitHeight: 200

    // Plot margins
    readonly property real marginLeft: 44
    readonly property real marginRight: 12
    readonly property real marginTop: 12
    readonly property real marginBottom: 32
    readonly property real plotX: marginLeft
    readonly property real plotY: marginTop
    readonly property real plotW: Math.max(1, width - marginLeft - marginRight)
    readonly property real plotH: Math.max(1, height - marginTop - marginBottom)
    readonly property real zeroY: plotY + plotH / 2
    readonly property real gainScale: plotH / 44.0

    // Node positions (computed after paint)
    property var nodePositions: [Qt.point(0,0), Qt.point(0,0), Qt.point(0,0), Qt.point(0,0)]

    Component.onCompleted: {
        if (hex0 !== "") {
            lowCut = paramBridge.getValue(hex0, hex1, hex2, h3LowCut)
            lowGain = paramBridge.getValue(hex0, hex1, hex2, h3LowGain)
            lowMidFreq = paramBridge.getValue(hex0, hex1, hex2, h3LowMidFreq)
            lowMidQ = paramBridge.getValue(hex0, hex1, hex2, h3LowMidQ)
            lowMidGain = paramBridge.getValue(hex0, hex1, hex2, h3LowMidGain)
            highMidFreq = paramBridge.getValue(hex0, hex1, hex2, h3HighMidFreq)
            highMidQ = paramBridge.getValue(hex0, hex1, hex2, h3HighMidQ)
            highMidGain = paramBridge.getValue(hex0, hex1, hex2, h3HighMidGain)
            highGain = paramBridge.getValue(hex0, hex1, hex2, h3HighGain)
            highCut = paramBridge.getValue(hex0, hex1, hex2, h3HighCut)
            level = paramBridge.getValue(hex0, hex1, hex2, h3Level)
            requestPaint()
        }
    }

    onLowCutChanged: requestPaint()
    onLowGainChanged: requestPaint()
    onLowMidFreqChanged: requestPaint()
    onLowMidQChanged: requestPaint()
    onLowMidGainChanged: requestPaint()
    onHighMidFreqChanged: requestPaint()
    onHighMidQChanged: requestPaint()
    onHighMidGainChanged: requestPaint()
    onHighGainChanged: requestPaint()
    onHighCutChanged: requestPaint()
    onLevelChanged: requestPaint()

    function gainValue(raw) { return raw - 32.0 }

    function smoothStep(edge0, edge1, x) {
        if (Math.abs(edge0 - edge1) < 0.001) return x < edge0 ? 0.0 : 1.0
        var t = Math.max(0, Math.min(1, (x - edge0) / (edge1 - edge0)))
        return t * t * (3.0 - 2.0 * t)
    }

    function bellShape(x, center, w) {
        var d = (x - center) / Math.max(w, 0.001)
        return Math.exp(-(d * d))
    }

    onPaint: {
        var ctx = getContext("2d")
        ctx.reset()
        ctx.clearRect(0, 0, width, height)

        var px = plotX, py = plotY, pw = plotW, ph = plotH

        // Background
        var grad = ctx.createLinearGradient(px, py, px, py + ph)
        grad.addColorStop(0, "#141920")
        grad.addColorStop(1, "#090D12")
        ctx.fillStyle = grad
        ctx.strokeStyle = "rgba(110, 126, 146, 0.47)"
        ctx.lineWidth = 1
        ctx.beginPath()
        ctx.roundedRect(px, py, pw, ph, 10, 10)
        ctx.fill()
        ctx.stroke()

        // Save for clipping
        ctx.save()
        ctx.beginPath()
        ctx.rect(px + 1, py + 1, pw - 2, ph - 2)
        ctx.clip()

        // Frequency grid lines
        var freqStops = [0.04, 0.09, 0.16, 0.25, 0.36, 0.50, 0.64, 0.76, 0.86, 0.93, 0.98]
        ctx.strokeStyle = "rgba(185, 197, 214, 0.10)"
        ctx.lineWidth = 1
        for (var fi = 0; fi < freqStops.length; fi++) {
            var fx = px + pw * freqStops[fi]
            ctx.beginPath()
            ctx.moveTo(fx, py)
            ctx.lineTo(fx, py + ph)
            ctx.stroke()
        }

        // Horizontal gain lines
        for (var gi = 0; gi <= 4; gi++) {
            var gy = py + (ph / 4.0) * gi
            ctx.strokeStyle = (gi === 2) ? "rgba(94, 206, 255, 0.33)" : "rgba(185, 197, 214, 0.08)"
            ctx.lineWidth = (gi === 2) ? 1.2 : 1
            ctx.beginPath()
            ctx.moveTo(px, gy)
            ctx.lineTo(px + pw, gy)
            ctx.stroke()
        }

        ctx.restore()

        // Gain labels
        ctx.fillStyle = "rgba(185, 197, 214, 0.59)"
        ctx.font = "9px 'Roboto Condensed'"
        ctx.textAlign = "right"
        ctx.textBaseline = "middle"
        var gains = [-20, -10, 0, 10, 20]
        for (var li = 0; li < gains.length; li++) {
            var ly = zeroY - gains[li] * gainScale
            ctx.fillText(gains[li].toString(), px - 6, ly)
        }

        // Frequency labels along bottom — contiguous ranges matching ParaEQ bands
        ctx.font = "9px 'Roboto Condensed'"
        ctx.textBaseline = "top"
        var flY = py + ph + 4

        // Individual frequency markers
        ctx.fillStyle = "rgba(185, 197, 214, 0.59)"
        ctx.textAlign = "center"
        var freqLabels = [
            { pos: 0.00, text: "20" },
            { pos: 0.16, text: "100" },
            { pos: 0.36, text: "500" },
            { pos: 0.50, text: "1k" },
            { pos: 0.76, text: "5k" },
            { pos: 0.98, text: "20k" }
        ]
        for (var fli = 0; fli < freqLabels.length; fli++) {
            var flx = px + pw * freqLabels[fli].pos
            ctx.fillText(freqLabels[fli].text, flx, flY)
        }

        // Band range labels (brighter, centered in each band region)
        ctx.fillStyle = "rgba(185, 197, 214, 0.35)"
        ctx.font = "8px 'Roboto Condensed'"
        ctx.textAlign = "center"
        ctx.fillText("LOW", px + pw * 0.10, flY + 10)
        ctx.fillText("LOW MID", px + pw * 0.29, flY + 10)
        ctx.fillText("HIGH MID", px + pw * 0.67, flY + 10)
        ctx.fillText("HIGH", px + pw * 0.90, flY + 10)

        // Compute curve
        var lowCutAmount = Math.max(0, Math.min(1, lowCut / 30.0))
        var highCutAmount = Math.max(0, Math.min(1, highCut / 30.0))
        var lowCutX = px + pw * (0.02 + lowCutAmount * 0.16)
        var highCutX = px + pw * (0.82 + highCutAmount * 0.14)
        var lowMidCenter = px + pw * (0.20 + Math.max(0, Math.min(1, lowMidFreq / 30.0)) * 0.18)
        var highMidCenter = px + pw * (0.58 + Math.max(0, Math.min(1, highMidFreq / 30.0)) * 0.18)
        var lowMidWidth = pw * (0.14 - Math.max(0, Math.min(1, lowMidQ / 5.0)) * 0.08)
        var highMidWidth = pw * (0.14 - Math.max(0, Math.min(1, highMidQ / 5.0)) * 0.08)
        var lowShelfStart = px + pw * 0.16
        var highShelfStart = px + pw * 0.76

        var points = []
        var lowShelfPt, lowMidPt, highMidPt, highShelfPt

        for (var i = 0; i <= 160; i++) {
            var t = i / 160.0
            var x = px + pw * t
            var gain = 0.0
            gain += gainValue(lowGain) * (1.0 - smoothStep(lowShelfStart, lowShelfStart + pw * 0.18, x))
            gain += gainValue(highGain) * smoothStep(highShelfStart - pw * 0.18, highShelfStart, x)
            gain += gainValue(lowMidGain) * bellShape(x, lowMidCenter, lowMidWidth)
            gain += gainValue(highMidGain) * bellShape(x, highMidCenter, highMidWidth)
            if (lowCutAmount > 0) {
                gain -= 18.0 * lowCutAmount * (1.0 - smoothStep(lowCutX - pw * 0.08, lowCutX + pw * 0.02, x))
            }
            if (highCutAmount > 0) {
                gain -= 18.0 * highCutAmount * smoothStep(highCutX - pw * 0.02, highCutX + pw * 0.08, x)
            }
            var y = Math.max(py, Math.min(py + ph, zeroY - gain * gainScale))
            points.push({x: x, y: y})

            if (i === 26) lowShelfPt = {x: x, y: y}
            if (Math.abs(x - lowMidCenter) < pw / 160.0) lowMidPt = {x: x, y: y}
            if (Math.abs(x - highMidCenter) < pw / 160.0) highMidPt = {x: x, y: y}
            if (i === 134) highShelfPt = {x: x, y: y}
        }

        // Fill under curve
        ctx.beginPath()
        ctx.moveTo(points[0].x, points[0].y)
        for (var ci = 1; ci < points.length; ci++) {
            ctx.lineTo(points[ci].x, points[ci].y)
        }
        ctx.lineTo(px + pw, py + ph)
        ctx.lineTo(px, py + ph)
        ctx.closePath()
        ctx.fillStyle = "rgba(94, 206, 255, 0.14)"
        ctx.fill()

        // Curve shadow
        ctx.beginPath()
        ctx.moveTo(points[0].x, points[0].y)
        for (var si = 1; si < points.length; si++) {
            ctx.lineTo(points[si].x, points[si].y)
        }
        ctx.strokeStyle = "rgba(0, 0, 0, 0.33)"
        ctx.lineWidth = 4.5
        ctx.lineCap = "round"
        ctx.lineJoin = "round"
        ctx.stroke()

        // Curve line
        ctx.beginPath()
        ctx.moveTo(points[0].x, points[0].y)
        for (var li2 = 1; li2 < points.length; li2++) {
            ctx.lineTo(points[li2].x, points[li2].y)
        }
        ctx.strokeStyle = "#5ECEFF"
        ctx.lineWidth = 2.2
        ctx.stroke()

        // Nodes
        var nodePts = [lowShelfPt, lowMidPt, highMidPt, highShelfPt]
        var newPositions = []
        for (var ni = 0; ni < 4; ni++) {
            if (!nodePts[ni]) { newPositions.push(Qt.point(0,0)); continue }
            var np = nodePts[ni]
            // Outer ring
            ctx.beginPath()
            ctx.arc(np.x, np.y, 5, 0, Math.PI * 2)
            ctx.strokeStyle = "rgba(245, 248, 252, 0.82)"
            ctx.lineWidth = 1.2
            ctx.fillStyle = "rgba(17, 24, 31, 0.96)"
            ctx.fill()
            ctx.stroke()
            // Inner dot
            ctx.beginPath()
            ctx.arc(np.x, np.y, 2.2, 0, Math.PI * 2)
            ctx.fillStyle = "#5ECEFF"
            ctx.fill()
            newPositions.push(Qt.point(np.x, np.y))
        }
        nodePositions = newPositions
    }

    // Drag interaction
    MouseArea {
        anchors.fill: parent
        preventStealing: true
        property int dragNode: -1
        property real lastY: 0

        function hitTest(mx, my) {
            for (var i = 0; i < 4; i++) {
                var np = root.nodePositions[i]
                var dx = mx - np.x, dy = my - np.y
                if (Math.sqrt(dx*dx + dy*dy) <= 12) return i
            }
            return -1
        }

        onPressed: function(mouse) {
            dragNode = hitTest(mouse.x, mouse.y)
            lastY = mouse.y
        }

        onPositionChanged: function(mouse) {
            if (dragNode < 0) return
            var gainDb = (root.zeroY - mouse.y) / root.gainScale
            var rawGain = Math.max(12, Math.min(52, Math.round(gainDb + 32)))

            if (dragNode === 0) {
                root.lowGain = rawGain
                paramBridge.setValue(root.hex0, root.hex1, root.hex2, root.h3LowGain, rawGain)
            } else if (dragNode === 1) {
                var norm1 = Math.max(0, Math.min(1, (mouse.x - root.plotX) / root.plotW))
                var frac1 = Math.max(0, Math.min(1, (norm1 - 0.20) / 0.18))
                var freq1 = Math.round(frac1 * 30)
                root.lowMidFreq = freq1
                root.lowMidGain = rawGain
                paramBridge.setValue(root.hex0, root.hex1, root.hex2, root.h3LowMidFreq, freq1)
                paramBridge.setValue(root.hex0, root.hex1, root.hex2, root.h3LowMidGain, rawGain)
            } else if (dragNode === 2) {
                var norm2 = Math.max(0, Math.min(1, (mouse.x - root.plotX) / root.plotW))
                var frac2 = Math.max(0, Math.min(1, (norm2 - 0.58) / 0.18))
                var freq2 = Math.round(frac2 * 30)
                root.highMidFreq = freq2
                root.highMidGain = rawGain
                paramBridge.setValue(root.hex0, root.hex1, root.hex2, root.h3HighMidFreq, freq2)
                paramBridge.setValue(root.hex0, root.hex1, root.hex2, root.h3HighMidGain, rawGain)
            } else if (dragNode === 3) {
                root.highGain = rawGain
                paramBridge.setValue(root.hex0, root.hex1, root.hex2, root.h3HighGain, rawGain)
            }
        }

        onReleased: { dragNode = -1 }
    }
}
