import QtQuick
import "Fx1TypeData.js" as Fx1Data

Rectangle {
    id: root
    color: SyTheme.bgPanel
    implicitWidth: SyTheme.panelWidth
    implicitHeight: SyTheme.panelHeight

    property string hex1: "00"
    property string hex2: "40"   // base hex2: guitar="40", bass="3D"
    property color accentColor: Qt.rgba(0.8, 0.2, 0.8, 1)  // magenta
    property string fxTitle: "FX1"

    // Current FX type selection
    property int fxTypeIndex: 0

    // Compute effective hex1 and hex2 for a given type index
    // Roland SysEx uses 7-bit addressing: when hex2 exceeds 0x7F, hex1 increments
    function typeEffectiveHex1(typeIdx) {
        var base = parseInt(hex2, 16)
        var raw = base + 1 + typeIdx
        if (raw > 0x7F) {
            var h1 = parseInt(hex1, 16) + 1
            var h = h1.toString(16).toUpperCase()
            return h.length < 2 ? "0" + h : h
        }
        return hex1
    }

    function typeEffectiveHex2(typeIdx) {
        var base = parseInt(hex2, 16)
        var raw = base + 1 + typeIdx
        if (raw > 0x7F) raw -= 0x80
        var h = raw.toString(16).toUpperCase()
        return h.length < 2 ? "0" + h : h
    }

    Component.onCompleted: {
        fxTypeIndex = paramBridge.getValue("10", hex1, hex2, "01")
    }

    // Listen for type combo changes
    Connections {
        target: fxTypeCombo
        function onValueChanged() {
            root.fxTypeIndex = fxTypeCombo.value
            paramBridge.requestRescan()  // re-scan knobs for CC controller mapping
        }
    }

    Column {
        anchors.fill: parent
        spacing: 0

        StompHeader {
            width: parent.width
            accentColor: root.accentColor
            title: root.fxTitle
            powerHex0: "10"; powerHex1: root.hex1; powerHex2: root.hex2; powerHex3: "00"
        }

        // FX TYPE row
        Rectangle {
            width: parent.width
            height: 32
            color: SyTheme.bgControl

            Row {
                anchors.fill: parent
                anchors.leftMargin: 12
                spacing: 8

                Text {
                    width: SyTheme.selectorLabelW
                    text: "FX TYPE"
                    color: SyTheme.textDimmed
                    font.pixelSize: SyTheme.fontLabel
                    font.family: SyTheme.fontFamily
                    font.capitalization: Font.AllUppercase
                    anchors.verticalCenter: parent.verticalCenter
                }

                SyComboBox {
                    id: fxTypeCombo
                    hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "01"
                    property bool ccExclude: true  // don't map to LCXL — changing FX TYPE destroys panel
                    labelWidth: 0
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }

        Rectangle { width: parent.width; height: 1; color: SyTheme.divider }

        // Dynamic content area
        // Total fixed rows above: StompHeader (headerHeight) + FX TYPE row (modeSelectorH) + divider (1)
        Item {
            width: parent.width
            height: parent.height - SyTheme.headerHeight - SyTheme.modeSelectorH - 1

            // Generic view for simple types
            Loader {
                id: contentLoader
                anchors.fill: parent
                active: true

                sourceComponent: {
                    if (Fx1Data.isComplexType(root.fxTypeIndex))
                        return complexComponent
                    return genericComponent
                }
            }
        }
    }

    // Generic component: renders controls from Fx1TypeData
    // First combo becomes a selector bar; remaining controls render as knobs in a flow
    Component {
        id: genericComponent

        Item {
            id: genericRoot

            property var controls: Fx1Data.getControls(root.fxTypeIndex)
            property string currentHex1: root.typeEffectiveHex1(root.fxTypeIndex)
            property string currentHex2: root.typeEffectiveHex2(root.fxTypeIndex)

            // Find index of first combo control (for selector bar)
            property int firstComboIdx: {
                if (!controls) return -1
                for (var i = 0; i < controls.length; i++) {
                    if (controls[i].type === "combo") return i
                }
                return -1
            }

            Column {
                anchors.fill: parent
                spacing: 0

                // Selector bar for the first combo control (if any)
                Rectangle {
                    width: parent.width
                    height: genericRoot.firstComboIdx >= 0 ? SyTheme.modeSelectorH : 0
                    visible: genericRoot.firstComboIdx >= 0
                    color: SyTheme.bgControl

                    Row {
                        anchors.fill: parent
                        anchors.leftMargin: SyTheme.panelPadding
                        spacing: 8

                        Text {
                            id: promotedLabel1
                            width: SyTheme.selectorLabelW
                            text: promotedCombo1.label
                            color: SyTheme.textDimmed
                            font.pixelSize: SyTheme.fontLabel
                            font.family: SyTheme.fontFamily
                            font.capitalization: Font.AllUppercase
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        SyComboBox {
                            id: promotedCombo1
                            hex0: "10"
                            hex1: genericRoot.currentHex1
                            hex2: genericRoot.currentHex2
                            hex3: genericRoot.firstComboIdx >= 0 ? genericRoot.controls[genericRoot.firstComboIdx].hex3 : "00"
                            labelWidth: 0
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
                }

                Rectangle {
                    width: parent.width; height: 1; color: SyTheme.divider
                    visible: genericRoot.firstComboIdx >= 0
                }

                // Remaining controls as knobs in a flow
                Flickable {
                    width: parent.width
                    height: parent.height - (genericRoot.firstComboIdx >= 0 ? SyTheme.modeSelectorH + 1 : 0)
                    contentHeight: controlFlow.height + 2 * SyTheme.panelPadding
                    clip: true
                    interactive: contentHeight > height

                    Flow {
                        id: controlFlow
                        width: parent.width - 2 * SyTheme.panelPadding
                        x: SyTheme.panelPadding
                        y: SyTheme.panelPadding
                        spacing: SyTheme.flowSpacingSm

                        Repeater {
                            model: genericRoot.controls ? genericRoot.controls.length : 0

                            Loader {
                                // Skip the first combo (already in selector bar)
                                visible: !(index === genericRoot.firstComboIdx)
                                width: visible ? implicitWidth : 0
                                height: visible ? implicitHeight : 0

                                property var ctrl: genericRoot.controls[index]
                                property string ctrlHex1: genericRoot.currentHex1
                                property string ctrlHex2: genericRoot.currentHex2

                                sourceComponent: {
                                    if (!ctrl || index === genericRoot.firstComboIdx) return null
                                    switch (ctrl.type) {
                                    case "knob":
                                    case "dataknob":
                                        return knobComponent
                                    case "combo":
                                        return comboComponent
                                    case "switch":
                                        return switchComponent
                                    default:
                                        return null
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    Component {
        id: knobComponent
        FilmstripKnob {
            hex0: "10"; hex1: ctrlHex1; hex2: ctrlHex2; hex3: ctrl.hex3
            filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge
        }
    }

    Component {
        id: comboComponent
        SyComboBox {
            hex0: "10"; hex1: ctrlHex1; hex2: ctrlHex2; hex3: ctrl.hex3
        }
    }

    Component {
        id: switchComponent
        SySwitch {
            hex0: "10"; hex1: ctrlHex1; hex2: ctrlHex2; hex3: ctrl.hex3
        }
    }

    // Complex component: loads dedicated sub-panel with initial properties
    Component {
        id: complexComponent

        Loader {
            id: complexLoader
            anchors.fill: parent
            Component.onCompleted: {
                complexLoader.setSource(
                    Fx1Data.getComplexSource(root.fxTypeIndex),
                    {
                        "hex1": root.typeEffectiveHex1(root.fxTypeIndex),
                        "hex2": root.typeEffectiveHex2(root.fxTypeIndex)
                    }
                )
            }
        }
    }
}
