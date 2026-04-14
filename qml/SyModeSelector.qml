import QtQuick

Column {
    id: root

    property string label: "MODE"
    property alias combo: modeCombo
    property alias value: modeCombo.value

    // Forward hex addressing to the combo
    property string hex0: "10"
    property string hex1: ""
    property string hex2: ""
    property string hex3: ""

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

            Text {
                width: SyTheme.selectorLabelW
                text: root.label
                color: SyTheme.textDimmed
                font.pixelSize: SyTheme.fontLabel
                font.family: SyTheme.fontFamily
                font.capitalization: Font.AllUppercase
                anchors.verticalCenter: parent.verticalCenter
            }

            SyComboBox {
                id: modeCombo
                hex0: root.hex0; hex1: root.hex1; hex2: root.hex2; hex3: root.hex3
                labelWidth: 0
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }

    Rectangle { width: parent.width; height: 1; color: SyTheme.divider }
}
