.pragma library

// Delay type categories:
// 0=STEREO1, 1=STEREO2, 2=PAN, 3=DUAL_SERIES, 4=DUAL_PARALLEL, 5=DUAL_LR,
// 6=REVERSE, 7=ANALOG, 8=TAPE, 9=MODULATE, 10=WARP, 11=TWIST

function getCategory(typeIndex) {
    switch (typeIndex) {
    case 0: case 1: case 6: case 7: case 8: case 10:
        return "simple"
    case 2:
        return "pan"
    case 3: case 4: case 5:
        return "dual"
    case 9:
        return "modulate"
    case 11:
        return "twist"
    default:
        return "simple"
    }
}

// Simple: time, feedback, highcut, effect, direct, carryover
function getSimpleControls() {
    return [
        { type: "dataknob", hex3: "02" },
        { type: "knob",     hex3: "06" },
        { type: "knob",     hex3: "07" },
        { type: "knob",     hex3: "08" },
        { type: "knob",     hex3: "09" },
        { type: "knob",     hex3: "20" }
    ]
}

// Pan: adds tap time %
function getPanControls() {
    return [
        { type: "dataknob", hex3: "02" },
        { type: "knob",     hex3: "06" },
        { type: "knob",     hex3: "1F" },
        { type: "knob",     hex3: "07" },
        { type: "knob",     hex3: "08" },
        { type: "knob",     hex3: "09" },
        { type: "knob",     hex3: "20" }
    ]
}

// Dual: two delay sections
function getDualControls1() {
    return [
        { type: "dataknob", hex3: "0A" },
        { type: "knob",     hex3: "0E" },
        { type: "knob",     hex3: "0F" },
        { type: "knob",     hex3: "10" }
    ]
}

function getDualControls2() {
    return [
        { type: "dataknob", hex3: "11" },
        { type: "knob",     hex3: "15" },
        { type: "knob",     hex3: "16" },
        { type: "knob",     hex3: "17" }
    ]
}

function getDualCommon() {
    return [
        { type: "knob", hex3: "09" },
        { type: "knob", hex3: "20" }
    ]
}

// Modulate: adds mod rate/depth/phase
function getModulateControls() {
    return [
        { type: "dataknob", hex3: "02" },
        { type: "knob",     hex3: "06" },
        { type: "knob",     hex3: "18" },
        { type: "knob",     hex3: "19" },
        { type: "knob",     hex3: "1A" },
        { type: "knob",     hex3: "07" },
        { type: "knob",     hex3: "08" },
        { type: "knob",     hex3: "09" },
        { type: "knob",     hex3: "20" }
    ]
}

// Twist: adds twist type combo + rise/fall
function getTwistControls() {
    return [
        { type: "dataknob", hex3: "02" },
        { type: "knob",     hex3: "06" },
        { type: "combo",    hex3: "1B" },
        { type: "knob",     hex3: "1C" },
        { type: "knob",     hex3: "1D" },
        { type: "knob",     hex3: "1E" },
        { type: "knob",     hex3: "07" },
        { type: "knob",     hex3: "08" },
        { type: "knob",     hex3: "09" },
        { type: "knob",     hex3: "20" }
    ]
}
