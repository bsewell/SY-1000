// Fx1TypeData.js — Control definitions for all 37 FX1 effect types
// Each type maps to a guitar-mode hex2 offset from 0x41, bass-mode from 0x3E

.pragma library

// typeIndex → guitar hex2 offset from 0x41
// The FX TYPE combo at base hex2 ("40"/"3D"), hex3="01" selects the type index.
// Each type's controls live at hex2 = baseHex2 + 1 + typeIndex (guitar)
// or hex2 = baseHex2 + 1 + typeIndex (bass)

// Control types: "knob", "combo", "switch", "dataknob"
// For simple (non-nested) types, controls are laid out in a single row.

function getControls(typeIndex) {
    switch (typeIndex) {
    case 0: // AC Resonance
        return [
            { type: "combo", hex3: "00" },
            { type: "knob",  hex3: "01" },
            { type: "knob",  hex3: "02" },
            { type: "knob",  hex3: "03" }
        ];
    case 1: // Auto Wah
        return [
            { type: "combo", hex3: "00" },
            { type: "knob",  hex3: "01" },
            { type: "knob",  hex3: "02" },
            { type: "knob",  hex3: "04" },
            { type: "knob",  hex3: "05" },
            { type: "combo", hex3: "06" },
            { type: "knob",  hex3: "03" },
            { type: "knob",  hex3: "07" }
        ];
    case 2: // Chorus (complex — handled by ChorusView.qml)
        return null;
    case 3: // Classic Vibe
        return [
            { type: "knob", hex3: "00" },
            { type: "knob", hex3: "01" },
            { type: "knob", hex3: "02" }
        ];
    case 4: // Compressor
        return [
            { type: "combo", hex3: "01" },
            { type: "knob",  hex3: "02" },
            { type: "knob",  hex3: "03" },
            { type: "knob",  hex3: "06" },
            { type: "knob",  hex3: "04" },
            { type: "knob",  hex3: "05" }
        ];
    case 5: // Defretter
        return [
            { type: "knob", hex3: "00" },
            { type: "knob", hex3: "01" },
            { type: "knob", hex3: "04" },
            { type: "knob", hex3: "05" },
            { type: "knob", hex3: "02" },
            { type: "knob", hex3: "03" },
            { type: "knob", hex3: "06" }
        ];
    case 6: // Defretter Bass
        return [
            { type: "knob", hex3: "00" },
            { type: "knob", hex3: "01" },
            { type: "knob", hex3: "02" },
            { type: "knob", hex3: "03" },
            { type: "knob", hex3: "04" }
        ];
    case 7: // Delay (complex — handled by DelayView.qml)
        return null;
    case 8: // Flanger
        return [
            { type: "knob", hex3: "00" },
            { type: "knob", hex3: "01" },
            { type: "knob", hex3: "02" },
            { type: "knob", hex3: "03" },
            { type: "knob", hex3: "04" },
            { type: "knob", hex3: "05" },
            { type: "knob", hex3: "06" },
            { type: "knob", hex3: "07" },
            { type: "knob", hex3: "08" }
        ];
    case 9: // Flanger Bass
        return [
            { type: "knob", hex3: "00" },
            { type: "knob", hex3: "01" },
            { type: "knob", hex3: "02" },
            { type: "knob", hex3: "03" },
            { type: "knob", hex3: "04" },
            { type: "knob", hex3: "05" },
            { type: "knob", hex3: "06" },
            { type: "knob", hex3: "07" },
            { type: "knob", hex3: "08" }
        ];
    case 10: // Foot Volume
        return [
            { type: "dataknob", hex3: "00" },
            { type: "dataknob", hex3: "04" },
            { type: "dataknob", hex3: "08" },
            { type: "knob",     hex3: "0C" }
        ];
    case 11: // Graphic EQ
        return [
            { type: "knob", hex3: "00" },
            { type: "knob", hex3: "01" },
            { type: "knob", hex3: "02" },
            { type: "knob", hex3: "03" },
            { type: "knob", hex3: "04" },
            { type: "knob", hex3: "05" },
            { type: "knob", hex3: "06" },
            { type: "knob", hex3: "07" }
        ];
    case 12: // Harmonizer (complex — handled by HarmonizerView.qml)
        return null;
    case 13: // Humanizer
        return [
            { type: "combo", hex3: "00" },
            { type: "knob",  hex3: "01" },
            { type: "knob",  hex3: "02" },
            { type: "knob",  hex3: "03" },
            { type: "knob",  hex3: "04" },
            { type: "knob",  hex3: "05" },
            { type: "knob",  hex3: "06" },
            { type: "knob",  hex3: "07" },
            { type: "knob",  hex3: "08" }
        ];
    case 14: // Isolator — Band, Rate, Depth, Level (BPM is shared Master param)
        return [
            { type: "combo", hex3: "00" },
            { type: "knob",  hex3: "01" },
            { type: "knob",  hex3: "02" },
            { type: "knob",  hex3: "03" }
        ];
    case 15: // Limiter
        return [
            { type: "combo", hex3: "00" },
            { type: "knob",  hex3: "01" },
            { type: "knob",  hex3: "02" },
            { type: "knob",  hex3: "04" },
            { type: "knob",  hex3: "05" },
            { type: "knob",  hex3: "03" }
        ];
    case 16: // LoFi
        return [
            { type: "knob", hex3: "00" },
            { type: "knob", hex3: "01" },
            { type: "knob", hex3: "02" }
        ];
    case 17: // Octave
        return [
            { type: "knob", hex3: "00" },
            { type: "knob", hex3: "01" },
            { type: "knob", hex3: "02" }
        ];
    case 18: // Octave Bass
        return [
            { type: "knob", hex3: "00" },
            { type: "knob", hex3: "01" },
            { type: "knob", hex3: "02" }
        ];
    case 19: // Panner
        return [
            { type: "combo", hex3: "00" },
            { type: "knob",  hex3: "01" },
            { type: "knob",  hex3: "02" },
            { type: "knob",  hex3: "03" },
            { type: "knob",  hex3: "05" },
            { type: "knob",  hex3: "04" },
            { type: "knob",  hex3: "06" }
        ];
    case 20: // Para EQ — Boss order: Low Cut, Low Gain, LM Freq/Q/Gain, HM Freq/Q/Gain, High Gain, High Cut, Level
        return [
            { type: "knob", hex3: "09" },
            { type: "knob", hex3: "01" },
            { type: "knob", hex3: "03" },
            { type: "knob", hex3: "04" },
            { type: "knob", hex3: "05" },
            { type: "knob", hex3: "06" },
            { type: "knob", hex3: "07" },
            { type: "knob", hex3: "08" },
            { type: "knob", hex3: "02" },
            { type: "knob", hex3: "0A" },
            { type: "knob", hex3: "0B" }
        ];
    case 21: // Pedal Bend
        return [
            { type: "knob",     hex3: "00" },
            { type: "dataknob", hex3: "02" },
            { type: "knob",     hex3: "01" },
            { type: "knob",     hex3: "06" }
        ];
    case 22: // Phaser
        return [
            { type: "combo", hex3: "00" },
            { type: "knob",  hex3: "01" },
            { type: "knob",  hex3: "02" },
            { type: "knob",  hex3: "03" },
            { type: "knob",  hex3: "04" },
            { type: "knob",  hex3: "05" },
            { type: "knob",  hex3: "06" },
            { type: "knob",  hex3: "07" }
        ];
    case 23: // Pitch Shifter (complex — handled by PitchShifterView.qml)
        return null;
    case 24: // Reverb
        return [
            { type: "combo",    hex3: "01" },
            { type: "combo",    hex3: "03" },
            { type: "dataknob", hex3: "04" },
            { type: "knob",     hex3: "02" },
            { type: "knob",     hex3: "0B" },
            { type: "knob",     hex3: "0D" },
            { type: "knob",     hex3: "09" },
            { type: "knob",     hex3: "0A" },
            { type: "knob",     hex3: "08" },
            { type: "knob",     hex3: "0C" }
        ];
    case 25: // Ring Modulator
        return [
            { type: "combo", hex3: "00" },
            { type: "knob",  hex3: "01" },
            { type: "knob",  hex3: "02" },
            { type: "knob",  hex3: "03" },
            { type: "knob",  hex3: "04" },
            { type: "knob",  hex3: "05" }
        ];
    case 26: // Rotary
        return [
            { type: "combo", hex3: "00" },
            { type: "knob",  hex3: "01" },
            { type: "knob",  hex3: "02" },
            { type: "knob",  hex3: "03" },
            { type: "knob",  hex3: "04" },
            { type: "knob",  hex3: "05" },
            { type: "knob",  hex3: "06" },
            { type: "knob",  hex3: "07" }
        ];
    case 27: // Sitar Sim
        return [
            { type: "knob", hex3: "00" },
            { type: "knob", hex3: "01" },
            { type: "knob", hex3: "04" },
            { type: "knob", hex3: "05" },
            { type: "knob", hex3: "02" },
            { type: "knob", hex3: "03" },
            { type: "knob", hex3: "06" }
        ];
    case 28: // Slicer
        return [
            { type: "knob", hex3: "00" },
            { type: "knob", hex3: "01" },
            { type: "knob", hex3: "02" },
            { type: "knob", hex3: "05" },
            { type: "knob", hex3: "04" },
            { type: "knob", hex3: "03" },
            { type: "knob", hex3: "06" }
        ];
    case 29: // Slow Gear
        return [
            { type: "knob", hex3: "00" },
            { type: "knob", hex3: "01" },
            { type: "knob", hex3: "02" }
        ];
    case 30: // Slow Gear Bass
        return [
            { type: "knob", hex3: "00" },
            { type: "knob", hex3: "01" },
            { type: "knob", hex3: "02" }
        ];
    case 31: // Sound Hold
        return [
            { type: "knob", hex3: "00" },
            { type: "knob", hex3: "01" },
            { type: "knob", hex3: "02" }
        ];
    case 32: // T-Wah
        return [
            { type: "combo", hex3: "00" },
            { type: "combo", hex3: "01" },
            { type: "knob",  hex3: "02" },
            { type: "knob",  hex3: "03" },
            { type: "knob",  hex3: "04" },
            { type: "knob",  hex3: "05" },
            { type: "knob",  hex3: "06" },
            { type: "knob",  hex3: "07" }
        ];
    case 33: // T-Wah Bass
        return [
            { type: "combo", hex3: "00" },
            { type: "combo", hex3: "01" },
            { type: "knob",  hex3: "02" },
            { type: "knob",  hex3: "03" },
            { type: "knob",  hex3: "04" },
            { type: "knob",  hex3: "05" },
            { type: "knob",  hex3: "06" },
            { type: "knob",  hex3: "07" }
        ];
    case 34: // Tremolo
        return [
            { type: "knob", hex3: "00" },
            { type: "knob", hex3: "01" },
            { type: "knob", hex3: "02" },
            { type: "knob", hex3: "03" },
            { type: "knob", hex3: "04" }
        ];
    case 35: // Vibrato
        return [
            { type: "knob",  hex3: "00" },
            { type: "knob",  hex3: "01" },
            { type: "combo", hex3: "02" },
            { type: "knob",  hex3: "03" },
            { type: "knob",  hex3: "04" },
            { type: "knob",  hex3: "05" }
        ];
    case 36: // Wah
        return [
            { type: "combo",    hex3: "00" },
            { type: "dataknob", hex3: "02" },
            { type: "dataknob", hex3: "06" },
            { type: "dataknob", hex3: "0A" },
            { type: "knob",     hex3: "01" },
            { type: "knob",     hex3: "0E" }
        ];
    default:
        return [];
    }
}

// Returns true if the type uses a dedicated sub-component
function isComplexType(typeIndex) {
    return typeIndex === 2 || typeIndex === 7 || typeIndex === 12 || typeIndex === 23;
}

// Get the sub-component QML source for complex types
function getComplexSource(typeIndex) {
    switch (typeIndex) {
    case 2:  return "fx1/ChorusView.qml";
    case 7:  return "fx1/DelayView.qml";
    case 12: return "fx1/HarmonizerView.qml";
    case 23: return "fx1/PitchShifterView.qml";
    default: return "";
    }
}
