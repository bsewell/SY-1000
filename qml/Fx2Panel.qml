import QtQuick
import "Fx1TypeData.js" as Fx1Data

Rectangle {
    id: root
    color: SyTheme.bgPanel
    implicitWidth: SyTheme.panelWidth
    implicitHeight: SyTheme.panelHeight

    property string hex1: "00"
    property string hex2: "66"   // base hex2: guitar="66", bass="63"
    property color accentColor: Qt.rgba(0.8, 0.2, 0.8, 1)  // magenta
    property string fxTitle: "FX2"

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

    Connections {
        target: fxTypeCombo
        function onValueChanged() {
            root.fxTypeIndex = fxTypeCombo.value
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

        Rectangle {
            width: parent.width
            height: 32
            color: SyTheme.bgControl

            Row {
                anchors.fill: parent
                anchors.leftMargin: 12
                spacing: 8

                Text {
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
                    showLabel: false
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }

        Rectangle { width: parent.width; height: 1; color: SyTheme.divider }

        // Total fixed rows above: StompHeader (headerHeight) + FX TYPE row (modeSelectorH) + divider (1)
        Item {
            width: parent.width
            height: parent.height - SyTheme.headerHeight - SyTheme.modeSelectorH - 1

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

    Component {
        id: genericComponent

        Item {
            id: genericRoot

            property var controls: Fx1Data.getControls(root.fxTypeIndex)
            property string currentHex1: root.typeEffectiveHex1(root.fxTypeIndex)
            property string currentHex2: root.typeEffectiveHex2(root.fxTypeIndex)

            Flickable {
                anchors.fill: parent
                contentWidth: controlFlow.width
                contentHeight: controlFlow.height
                clip: true

                Flow {
                    id: controlFlow
                    width: genericRoot.width - 24
                    x: 12
                    y: 16
                    spacing: 16

                    Repeater {
                        model: genericRoot.controls ? genericRoot.controls.length : 0

                        Loader {
                            property var ctrl: genericRoot.controls[index]
                            property string ctrlHex1: genericRoot.currentHex1
                            property string ctrlHex2: genericRoot.currentHex2

                            sourceComponent: {
                                if (!ctrl) return null
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
