import QtQuick
import "DdTypeData.js" as DdData

Rectangle {
    id: root
    color: "#1a1a1a"
    implicitWidth: 800
    implicitHeight: 480

    property string hex1: "00"
    property string hex2: "3E"
    property color accentColor: Qt.rgba(0.85, 0.85, 0.85, 1)
    property string ddTitle: "DELAY 3"

    property int ddTypeIndex: 0

    Component.onCompleted: {
        ddTypeIndex = paramBridge.getValue("10", hex1, hex2, "01")
    }

    Connections {
        target: typeCombo
        function onValueChanged() {
            root.ddTypeIndex = typeCombo.value
        }
    }

    Column {
        anchors.fill: parent
        spacing: 0

        StompHeader {
            width: parent.width
            accentColor: root.accentColor
            title: root.ddTitle
            powerHex0: "10"; powerHex1: root.hex1; powerHex2: root.hex2; powerHex3: "00"
        }

        Rectangle {
            width: parent.width
            height: 32
            color: "#2a2a2a"

            Row {
                anchors.fill: parent
                anchors.leftMargin: 12
                spacing: 8

                Text {
                    text: "TYPE"
                    color: "#888888"
                    font.pixelSize: 11
                    font.family: "Roboto Condensed"
                    font.capitalization: Font.AllUppercase
                    anchors.verticalCenter: parent.verticalCenter
                }

                SyComboBox {
                    id: typeCombo
                    hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "01"
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }

        Rectangle { width: parent.width; height: 1; color: "#333333" }

        Flickable {
            width: parent.width
            height: parent.height - 70
            contentHeight: contentCol.height + 24
            clip: true

            Column {
                id: contentCol
                width: parent.width
                spacing: 8
                topPadding: 12

                Flow {
                    width: parent.width - 24
                    x: 12
                    spacing: 12
                    visible: DdData.getCategory(root.ddTypeIndex) !== "dual"

                    Repeater {
                        model: {
                            var cat = DdData.getCategory(root.ddTypeIndex)
                            if (cat === "pan") return DdData.getPanControls()
                            if (cat === "modulate") return DdData.getModulateControls()
                            if (cat === "twist") return DdData.getTwistControls()
                            return DdData.getSimpleControls()
                        }

                        Loader {
                            property var ctrl: modelData
                            sourceComponent: {
                                if (!ctrl) return null
                                if (ctrl.type === "combo") return ddComboComp
                                return ddKnobComp
                            }
                        }
                    }
                }

                Column {
                    width: parent.width
                    spacing: 8
                    visible: DdData.getCategory(root.ddTypeIndex) === "dual"

                    Text { x: 12; text: "DELAY A"; color: "#666666"; font.pixelSize: 10; font.family: "Roboto Condensed" }

                    Flow {
                        width: parent.width - 24; x: 12; spacing: 12
                        Repeater {
                            model: DdData.getDualControls1()
                            Loader { property var ctrl: modelData; sourceComponent: ddKnobComp }
                        }
                    }

                    Rectangle { width: parent.width - 24; height: 1; color: "#333333"; x: 12 }

                    Text { x: 12; text: "DELAY B"; color: "#666666"; font.pixelSize: 10; font.family: "Roboto Condensed" }

                    Flow {
                        width: parent.width - 24; x: 12; spacing: 12
                        Repeater {
                            model: DdData.getDualControls2()
                            Loader { property var ctrl: modelData; sourceComponent: ddKnobComp }
                        }
                    }

                    Rectangle { width: parent.width - 24; height: 1; color: "#333333"; x: 12 }

                    Text { x: 12; text: "OUTPUT"; color: "#666666"; font.pixelSize: 10; font.family: "Roboto Condensed" }

                    Flow {
                        width: parent.width - 24; x: 12; spacing: 12
                        Repeater {
                            model: DdData.getDualCommon()
                            Loader { property var ctrl: modelData; sourceComponent: ddKnobComp }
                        }
                    }
                }
            }
        }
    }

    Component {
        id: ddKnobComp
        FilmstripKnob {
            hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: ctrl.hex3
            filmstrip: "knobs/knob_56.png"; frameSize: 56
        }
    }

    Component {
        id: ddComboComp
        SyComboBox {
            hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: ctrl.hex3
        }
    }
}
