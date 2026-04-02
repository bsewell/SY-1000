import QtQuick
import "DdTypeData.js" as DdData

SyPanelBase {
    id: panel
    hex1: "00"; hex2: "3D"
    accentColor: Qt.rgba(0.85, 0.85, 0.85, 1)
    title: "DELAY 2"
    powerHex0: "10"; powerHex1: hex1; powerHex2: hex2; powerHex3: "00"
    showHeader: false

    property int ddTypeIndex: 0
    property var typeCombo: null

    Component.onCompleted: {
        ddTypeIndex = paramBridge.getValue("10", hex1, hex2, "01")
    }

    Connections {
        target: typeCombo
        function onValueChanged() {
            panel.ddTypeIndex = typeCombo.value
        }
    }

    Column {
        width: parent.width
        spacing: 0

        StompHeader {
            width: parent.width
            accentColor: panel.accentColor
            title: "DELAY 2"
            powerHex0: "10"; powerHex1: panel.hex1; powerHex2: panel.hex2; powerHex3: "00"
        }

        Rectangle { width: parent.width; height: 1; color: SyTheme.divider }

        SyModeSelector {
            label: "TYPE"
            hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: "01"
            Component.onCompleted: typeCombo = combo
        }

        Flickable {
            width: parent.width
            height: panel.height - SyTheme.headerHeight - 1 - SyTheme.modeSelectorH - 1
            contentHeight: contentCol.height + 2 * SyTheme.panelPadding
            clip: true
            interactive: contentHeight > height

            Column {
                id: contentCol
                width: parent.width
                spacing: SyTheme.flowSpacingSm
                topPadding: SyTheme.panelPadding

                Flow {
                    width: parent.width - 2 * SyTheme.panelPadding
                    x: SyTheme.panelPadding
                    spacing: SyTheme.flowSpacingSm
                    visible: DdData.getCategory(panel.ddTypeIndex) !== "dual"

                    Repeater {
                        model: {
                            var cat = DdData.getCategory(panel.ddTypeIndex)
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
                    spacing: SyTheme.flowSpacingSm
                    visible: DdData.getCategory(panel.ddTypeIndex) === "dual"

                    Text { x: SyTheme.panelPadding; text: "DELAY A"; color: SyTheme.textSection; font.pixelSize: SyTheme.fontSmall; font.family: SyTheme.fontFamily }

                    Flow {
                        width: parent.width - 2 * SyTheme.panelPadding; x: SyTheme.panelPadding; spacing: SyTheme.flowSpacingSm
                        Repeater {
                            model: DdData.getDualControls1()
                            Loader { property var ctrl: modelData; sourceComponent: ddKnobComp }
                        }
                    }

                    Rectangle { width: parent.width - 2 * SyTheme.panelPadding; height: 1; color: SyTheme.divider; x: SyTheme.panelPadding }

                    Text { x: SyTheme.panelPadding; text: "DELAY B"; color: SyTheme.textSection; font.pixelSize: SyTheme.fontSmall; font.family: SyTheme.fontFamily }

                    Flow {
                        width: parent.width - 2 * SyTheme.panelPadding; x: SyTheme.panelPadding; spacing: SyTheme.flowSpacingSm
                        Repeater {
                            model: DdData.getDualControls2()
                            Loader { property var ctrl: modelData; sourceComponent: ddKnobComp }
                        }
                    }

                    Rectangle { width: parent.width - 2 * SyTheme.panelPadding; height: 1; color: SyTheme.divider; x: SyTheme.panelPadding }

                    Text { x: SyTheme.panelPadding; text: "OUTPUT"; color: SyTheme.textSection; font.pixelSize: SyTheme.fontSmall; font.family: SyTheme.fontFamily }

                    Flow {
                        width: parent.width - 2 * SyTheme.panelPadding; x: SyTheme.panelPadding; spacing: SyTheme.flowSpacingSm
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
            hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: ctrl.hex3
            filmstrip: SyTheme.knobLargeSrc; frameSize: SyTheme.knobLarge
        }
    }

    Component {
        id: ddComboComp
        SyComboBox {
            hex0: "10"; hex1: panel.hex1; hex2: panel.hex2; hex3: ctrl.hex3
        }
    }
}
