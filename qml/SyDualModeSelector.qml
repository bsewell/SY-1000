import QtQuick

Column {
    id: root

    // TYPE selector (left side)
    property string typeLabel: "TYPE"
    property alias typeCombo: typeComboBox
    property alias typeValue: typeComboBox.value
    property string typeHex0: "10"
    property string typeHex1: ""
    property string typeHex2: ""
    property string typeHex3: ""

    // MODE selector (right side, conditionally visible)
    property string modeLabel: "MODE"
    property alias modeCombo: modeComboBox
    property alias modeValue: modeComboBox.value
    property string modeHex0: "10"
    property string modeHex1: ""
    property string modeHex2: ""
    property string modeHex3: ""
    property bool modeVisible: false

    width: parent ? parent.width : 400
    spacing: 0

    Rectangle {
        width: parent.width
        height: SyTheme.modeSelectorH
        color: SyTheme.bgControl

        Row {
            anchors.fill: parent
            anchors.leftMargin: SyTheme.panelPadding
            spacing: 8

            // TYPE label + combo
            Text {
                text: root.typeLabel
                color: SyTheme.textDimmed
                font.pixelSize: SyTheme.fontLabel
                font.family: SyTheme.fontFamily
                font.capitalization: Font.AllUppercase
                anchors.verticalCenter: parent.verticalCenter
            }

            SyComboBox {
                id: typeComboBox
                hex0: root.typeHex0; hex1: root.typeHex1
                hex2: root.typeHex2; hex3: root.typeHex3
                labelWidth: 0
                implicitWidth: 200
                anchors.verticalCenter: parent.verticalCenter
            }

            // Spacer
            Item { width: 24; height: 1; visible: root.modeVisible }

            // MODE label + combo (only visible for certain types)
            Text {
                visible: root.modeVisible
                text: root.modeLabel
                color: SyTheme.textDimmed
                font.pixelSize: SyTheme.fontLabel
                font.family: SyTheme.fontFamily
                font.capitalization: Font.AllUppercase
                anchors.verticalCenter: parent.verticalCenter
            }

            SyComboBox {
                id: modeComboBox
                visible: root.modeVisible
                hex0: root.modeHex0; hex1: root.modeHex1
                hex2: root.modeHex2; hex3: root.modeHex3
                labelWidth: 0
                implicitWidth: 200
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }

    Rectangle { width: parent.width; height: 1; color: SyTheme.divider }
}
