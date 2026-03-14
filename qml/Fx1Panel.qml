import QtQuick
import "Fx1TypeData.js" as Fx1Data

Rectangle {
    id: root
    color: "#1a1a1a"
    implicitWidth: 800
    implicitHeight: 480

    property string hex1: "00"
    property string hex2: "40"   // base hex2: guitar="40", bass="3D"
    property color accentColor: Qt.rgba(0.8, 0.2, 0.8, 1)  // magenta

    // Current FX type selection
    property int fxTypeIndex: 0

    // Compute the hex2 for the selected effect type's parameters
    // Each type lives at baseHex2 + 1 + typeIndex
    function typeHex2(typeIdx) {
        var base = parseInt(hex2, 16)
        var val = base + 1 + typeIdx
        var h = val.toString(16).toUpperCase()
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
        }
    }

    Column {
        anchors.fill: parent
        spacing: 0

        StompHeader {
            width: parent.width
            accentColor: root.accentColor
            title: "FX1"
            powerHex0: "10"; powerHex1: root.hex1; powerHex2: root.hex2; powerHex3: "00"
        }

        // FX TYPE row
        Rectangle {
            width: parent.width
            height: 32
            color: "#2a2a2a"

            Row {
                anchors.fill: parent
                anchors.leftMargin: 12
                spacing: 8

                Text {
                    text: "FX TYPE"
                    color: "#888888"
                    font.pixelSize: 11
                    font.family: "Roboto Condensed"
                    font.capitalization: Font.AllUppercase
                    anchors.verticalCenter: parent.verticalCenter
                }

                SyComboBox {
                    id: fxTypeCombo
                    hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "01"
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }

        Rectangle { width: parent.width; height: 1; color: "#333333" }

        // Dynamic content area
        Item {
            width: parent.width
            height: parent.height - 70

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
    Component {
        id: genericComponent

        Item {
            id: genericRoot

            property var controls: Fx1Data.getControls(root.fxTypeIndex)
            property string currentHex2: root.typeHex2(root.fxTypeIndex)

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
            hex0: "10"; hex1: root.hex1; hex2: ctrlHex2; hex3: ctrl.hex3
            filmstrip: "knobs/knob_56.png"; frameSize: 56
        }
    }

    Component {
        id: comboComponent
        SyComboBox {
            hex0: "10"; hex1: root.hex1; hex2: ctrlHex2; hex3: ctrl.hex3
        }
    }

    Component {
        id: switchComponent
        SySwitch {
            hex0: "10"; hex1: root.hex1; hex2: ctrlHex2; hex3: ctrl.hex3
        }
    }

    // Complex component: loads dedicated sub-panel
    Component {
        id: complexComponent

        Loader {
            source: Fx1Data.getComplexSource(root.fxTypeIndex)

            onLoaded: {
                item.hex1 = root.hex1
                item.hex2 = root.typeHex2(root.fxTypeIndex)
            }
        }
    }
}
