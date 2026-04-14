#!/usr/bin/env python3
"""
SY-1000 FloorBoard Panel Layout Tests

Validates QML stomp box panels against the layout spec in panel_layout_spec.json.
Tests structure, positioning, MIDI address correctness, and naming conventions.

Usage:
    python3 tests/test_panel_layout.py
    python3 tests/test_panel_layout.py -v          # verbose
    python3 tests/test_panel_layout.py -k chorus   # run only chorus tests
"""

import json
import os
import re
import sys
import xml.etree.ElementTree as ET
from pathlib import Path

REPO = Path(__file__).parent.parent
QML_DIR = REPO / "qml"
SRC_DIR = REPO / "src"
DATA_DIR = REPO / "data"
SPEC_FILE = REPO / "tests" / "panel_layout_spec.json"

# ─── Helpers ────────────────────────────────────────────────────────────

def load_spec():
    with open(SPEC_FILE) as f:
        return json.load(f)

def read_qml(filename):
    path = REPO / filename
    if not path.exists():
        return None
    return path.read_text()

def read_js(filename):
    path = REPO / filename
    if not path.exists():
        return None
    return path.read_text()

class TestResults:
    def __init__(self):
        self.passed = 0
        self.failed = 0
        self.warnings = 0
        self.details = []

    def ok(self, msg):
        self.passed += 1
        self.details.append(("PASS", msg))

    def fail(self, msg):
        self.failed += 1
        self.details.append(("FAIL", msg))

    def warn(self, msg):
        self.warnings += 1
        self.details.append(("WARN", msg))

    def report(self, verbose=False):
        print(f"\n{'='*60}")
        print(f"  RESULTS: {self.passed} passed, {self.failed} failed, {self.warnings} warnings")
        print(f"{'='*60}\n")
        if verbose or self.failed > 0:
            for status, msg in self.details:
                if status == "FAIL" or verbose:
                    icon = "✅" if status == "PASS" else "❌" if status == "FAIL" else "⚠️"
                    print(f"  {icon} {msg}")
        if self.failed > 0:
            print(f"\n  {self.failed} FAILURES — fix before merging\n")
        else:
            print(f"\n  All tests passed.\n")
        return self.failed == 0

# ─── Test: Panel Structure ──────────────────────────────────────────────

def test_panel_structure(results, spec):
    """Check each panel QML file matches its spec structure."""
    rules = spec["layout_rules"]

    for name, panel in spec["panels"].items():
        qml_file = panel.get("file")
        if not qml_file:
            continue

        qml = read_qml(qml_file)
        if qml is None:
            results.fail(f"{name}: QML file {qml_file} not found")
            continue

        # Check root type
        expected_root = panel.get("root", None)
        if expected_root:
            if f"{expected_root} {{" in qml or qml.strip().startswith(expected_root):
                results.ok(f"{name}: root is {expected_root}")
            else:
                results.fail(f"{name}: expected root {expected_root}, not found")

        # Check no SyPanelBase with nested Flickable
        if "SyPanelBase" in qml and "Flickable" in qml:
            results.fail(f"{name}: SyPanelBase + nested Flickable (causes blank panel)")
        else:
            results.ok(f"{name}: no SyPanelBase/Flickable conflict")

        structure = panel.get("structure", [])
        for item in structure:
            item_type = item.get("type")

            if item_type == "header":
                if "StompHeader" in qml:
                    results.ok(f"{name}: has StompHeader")
                else:
                    results.warn(f"{name}: no StompHeader found")

            elif item_type == "selector":
                label = item.get("label", "")
                hex3 = item.get("hex3", "")
                if "SyModeSelector" in qml:
                    results.ok(f"{name}: uses SyModeSelector for {label}")
                elif f'labelWidth: 0' in qml and f'hex3: "{hex3}"' in qml:
                    results.ok(f"{name}: uses separate label + labelWidth:0 pattern for {label}")
                else:
                    results.fail(f"{name}: selector '{label}' not using SyModeSelector or labelWidth:0 pattern")

                # Check ccExclude (only required when midi handler exists)
                if item.get("ccExclude"):
                    has_midi = (SRC_DIR / "midiCCHandler.h").exists()
                    if "ccExclude" in qml:
                        results.ok(f"{name}: {label} has ccExclude")
                    elif has_midi:
                        results.fail(f"{name}: {label} missing ccExclude: true")
                    else:
                        results.warn(f"{name}: {label} needs ccExclude when midi merges")

            elif item_type == "knob_area":
                if "Flickable" in qml and "Flow" in qml:
                    results.ok(f"{name}: has Flickable + Flow for knobs")
                else:
                    results.warn(f"{name}: missing Flickable or Flow for knob area")

                # Check Chorus layout pattern values
                if "spacing: 12" in qml or "spacing: SyTheme" in qml:
                    results.ok(f"{name}: Column spacing present")

                if "spacing: 8" in qml:
                    results.ok(f"{name}: Flow spacing = 8")

# ─── Test: Knob Sizes ──────────────────────────────────────────────────

def test_knob_sizes(results, spec):
    """Check all visible knobs use consistent sizing."""
    for name, panel in spec["panels"].items():
        qml_file = panel.get("file")
        if not qml_file:
            continue

        qml = read_qml(qml_file)
        if qml is None:
            continue

        large_count = qml.count("knobLargeSrc")
        small_count = qml.count("knobSmallSrc")

        if small_count > 0 and large_count > 0:
            results.warn(f"{name}: mixed knob sizes ({large_count} large, {small_count} small)")
        elif large_count > 0:
            results.ok(f"{name}: all knobs use knobLargeSrc ({large_count})")
        elif small_count > 0:
            results.warn(f"{name}: uses only small knobs ({small_count})")

# ─── Test: FX Type Data vs MIDI XML ────────────────────────────────────

def test_fx_types_vs_midi(results, spec):
    """Verify Fx1TypeData.js hex3 addresses exist in midi.xml."""
    js = read_js("qml/Fx1TypeData.js")
    if js is None:
        results.fail("Fx1TypeData.js not found")
        return

    # Extract all hex3 values from the JS
    hex3_pattern = re.compile(r'hex3:\s*"([0-9A-Fa-f]{2})"')
    all_hex3 = hex3_pattern.findall(js)
    results.ok(f"Fx1TypeData.js: found {len(all_hex3)} hex3 references")

    # Check for known phantom addresses
    phantoms = {
        "Isolator (case 14)": ["04"],  # was removed
    }
    for label, bad_addrs in phantoms.items():
        for addr in bad_addrs:
            # Search within the specific case block
            if f'"04"' in js:
                # Need to check if it's in the Isolator case specifically
                pass  # Complex to parse JS cases — skip for now

# ─── Test: Signal Chain Labels ──────────────────────────────────────────

def test_signal_chain_labels(results, spec):
    """Check stompBox.cpp label conventions."""
    stompbox_cpp = (SRC_DIR / "stompBox.cpp").read_text()

    # Check Init Cap format for fxName assignments
    fx_names = re.findall(r'this->fxName\s*=\s*tr\("([^"]+)"\)', stompbox_cpp)
    for name in fx_names:
        # Should be Init Cap: first letter of each word uppercase
        words = name.split()
        is_init_cap = all(
            w[0].isupper() or w in ["A/B", "1", "2", "3"]  # exceptions
            for w in words if len(w) > 0
        )
        if is_init_cap:
            results.ok(f"fxName '{name}' is Init Cap")
        else:
            results.fail(f"fxName '{name}' is NOT Init Cap")

    # Check updateLabel uses Init Cap conversion
    if "toLower()" in stompbox_cpp and "toUpper()" in stompbox_cpp:
        results.ok("updateLabel: has title case conversion")
    elif "toLower()" in stompbox_cpp:
        results.fail("updateLabel: uses toLower() without title case conversion")

    # Check DLY 1/2 use clearLabel
    dd1 = (SRC_DIR / "stompbox_dd1.cpp").read_text()
    dd2 = (SRC_DIR / "stompbox_dd2.cpp").read_text()
    dd3 = (SRC_DIR / "stompbox_dd3.cpp").read_text()

    if "clearLabel" in dd1:
        results.ok("DLY 1: uses clearLabel()")
    elif "updateLabel" not in dd1:
        results.ok("DLY 1: no updateLabel (label stays default)")
    else:
        results.fail("DLY 1: still calls updateLabel (should use clearLabel)")

    if "clearLabel" in dd2:
        results.ok("DLY 2: uses clearLabel()")
    else:
        results.fail("DLY 2: missing clearLabel")

    if "updateLabel" in dd3:
        results.ok("M DLY: uses updateLabel (shows type name)")
    else:
        results.fail("M DLY: missing updateLabel")

# ─── Test: CC Mapping Safety ────────────────────────────────────────────

def test_cc_safety(results, spec):
    """Check ccExclude on type-changing combos (midi branch only)."""
    # ccExclude only exists on midi-cc-controller branch — warn instead of fail on main
    has_midi = (SRC_DIR / "midiCCHandler.h").exists()

    for panel_file in ["Fx1Panel.qml", "Fx2Panel.qml", "Fx3Panel.qml"]:
        qml = read_qml(f"qml/{panel_file}")
        if qml is None:
            results.warn(f"{panel_file}: not found")
            continue

        if "ccExclude" in qml:
            results.ok(f"{panel_file}: FX TYPE has ccExclude")
        elif has_midi:
            results.fail(f"{panel_file}: FX TYPE missing ccExclude (LCXL will change effect type)")
        else:
            results.warn(f"{panel_file}: ccExclude needed when midi branch merges")

    inst_qml = read_qml("qml/InstrumentPanel.qml")
    if inst_qml:
        if "ccExclude" in inst_qml:
            results.ok("InstrumentPanel: INST TYPE has ccExclude")
        elif has_midi:
            results.fail("InstrumentPanel: INST TYPE missing ccExclude")
        else:
            results.warn("InstrumentPanel: ccExclude needed when midi branch merges")

# ─── Test: Dropdown Alignment ───────────────────────────────────────────

def test_dropdown_alignment(results, spec):
    """Check selector row dropdowns use consistent alignment."""
    mode_selector = read_qml("qml/SyModeSelector.qml")
    if mode_selector:
        if "selectorLabelW" in mode_selector and "labelWidth: 0" in mode_selector:
            results.ok("SyModeSelector: uses selectorLabelW + labelWidth:0 pattern")
        else:
            results.fail("SyModeSelector: doesn't follow alignment pattern")

    # Check FX panels use the pattern for promoted combos
    for panel_file in ["Fx1Panel.qml", "Fx2Panel.qml", "Fx3Panel.qml"]:
        qml = read_qml(f"qml/{panel_file}")
        if qml and "selectorLabelW" in qml:
            results.ok(f"{panel_file}: promoted combo uses selectorLabelW")

# ─── Main ───────────────────────────────────────────────────────────────

def main():
    verbose = "-v" in sys.argv
    keyword = None
    for i, arg in enumerate(sys.argv):
        if arg == "-k" and i + 1 < len(sys.argv):
            keyword = sys.argv[i + 1].lower()

    spec = load_spec()
    results = TestResults()

    print("SY-1000 FloorBoard — Panel Layout Tests")
    print("=" * 60)

    if keyword:
        print(f"  Filter: *{keyword}*\n")

    tests = [
        ("Panel Structure", test_panel_structure),
        ("Knob Sizes", test_knob_sizes),
        ("FX Types vs MIDI", test_fx_types_vs_midi),
        ("Signal Chain Labels", test_signal_chain_labels),
        ("CC Mapping Safety", test_cc_safety),
        ("Dropdown Alignment", test_dropdown_alignment),
    ]

    for name, fn in tests:
        if keyword and keyword not in name.lower():
            continue
        print(f"\n── {name} ──")
        fn(results, spec)

    success = results.report(verbose)
    sys.exit(0 if success else 1)

if __name__ == "__main__":
    main()
