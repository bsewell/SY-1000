.pragma library

// Master Delay type indices:
// 0=STEREO1, 1=STEREO2, 2=PAN, 3=DUAL_SERIES, 4=DUAL_PARALLEL, 5=DUAL_LR,
// 6=REVERSE, 7=ANALOG, 8=TAPE, 9=MOD, 10=WARP, 11=TWIST

// Twist mode indices:
// 0=RISE->FALL, 1=RISE->FADE

// --- Row 1: Simple/shared controls ---
// Boss layout: TIME, FEEDBACK, HIGH CUT, TAP TIME, MOD RATE, MOD DEPTH
function getRow1() {
    return [
        { hex3: "02", type: "dataknob" },   // TIME
        { hex3: "06", type: "knob" },        // FEEDBACK
        { hex3: "07", type: "knob" },        // HIGH CUT
        { hex3: "1F", type: "knob" },        // TAP TIME
        { hex3: "18", type: "knob" },        // MOD RATE
        { hex3: "19", type: "knob" }         // MOD DEPTH
    ]
}

// --- Row 2: Dual controls ---
// Boss layout: D1 TIME, D1 FEEDBACK, D1 HIGH CUT, D1 EFFECT LEVEL,
//              D2 TIME, D2 FEEDBACK, D2 HIGH CUT, D2 EFFECT LEVEL
function getRow2() {
    return [
        { hex3: "0A", type: "dataknob" },   // D1 TIME
        { hex3: "0E", type: "knob" },        // D1 FEEDBACK
        { hex3: "0F", type: "knob" },        // D1 HIGH CUT
        { hex3: "10", type: "knob" },        // D1 EFFECT LEVEL
        { hex3: "11", type: "dataknob" },    // D2 TIME
        { hex3: "15", type: "knob" },        // D2 FEEDBACK
        { hex3: "16", type: "knob" },        // D2 HIGH CUT
        { hex3: "17", type: "knob" }         // D2 EFFECT LEVEL
    ]
}

// --- Row 3: Twist/common controls ---
// Boss layout: TRIGGER, RISE TIME, FALL TIME, FADE TIME,
//              EFFECT LEVEL, DIRECT LEVEL, CARRY OVER, BPM
function getRow3() {
    return [
        { hex3: "1A", type: "knob" },        // TRIGGER (WARP)
        { hex3: "1C", type: "knob" },        // RISE TIME
        { hex3: "1D", type: "knob" },        // FALL TIME
        { hex3: "1E", type: "knob" },        // FADE TIME
        { hex3: "08", type: "knob" },        // EFFECT LEVEL
        { hex3: "09", type: "knob" },        // DIRECT LEVEL
        { hex3: "20", type: "knob" },        // CARRY OVER
        { hex3: "bpm", type: "knob" }        // BPM (patch-level tempo)
    ]
}

// --- Dimming logic per control ---
// Returns true if the control is ACTIVE (not dimmed) for the given type/mode
function isActive(hex3, typeIndex, modeIndex) {
    var isDual = (typeIndex >= 3 && typeIndex <= 5)  // DUAL-S, DUAL-P, DUAL L/R
    var isPan = (typeIndex === 2)
    var isMod = (typeIndex === 9)
    var isWarp = (typeIndex === 10)
    var isTwist = (typeIndex === 11)

    switch (hex3) {
    // Row 1: simple shared controls — active when NOT dual and NOT twist
    case "02": case "06": case "07":
        return !isDual && !isTwist
    // TAP TIME — only PAN
    case "1F":
        return isPan
    // MOD RATE, MOD DEPTH — only MOD
    case "18": case "19":
        return isMod
    // Row 2: dual controls — only DUAL types
    case "0A": case "0E": case "0F": case "10":
    case "11": case "15": case "16": case "17":
        return isDual
    // TRIGGER — only WARP
    case "1A":
        return isWarp
    // RISE TIME — only TWIST
    case "1C":
        return isTwist
    // FALL TIME — only TWIST + RISE->FALL mode
    case "1D":
        return isTwist && modeIndex === 0
    // FADE TIME — only TWIST + RISE->FADE mode
    case "1E":
        return isTwist && modeIndex === 1
    // EFFECT LEVEL, DIRECT LEVEL, CARRY OVER, BPM — always active
    case "08": case "09": case "20": case "bpm":
        return true
    default:
        return true
    }
}
