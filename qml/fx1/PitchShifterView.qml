import QtQuick
import ".."

Item {
    id: root
    property string hex1: "00"
    property string hex2: "58"  // guitar="58", bass="55"

    property int psMode: 0

    Component.onCompleted: {
        psMode = paramBridge.getValue("10", hex1, hex2, "00")
    }

    Connections {
        target: psModeCombo
        function onValueChanged() { root.psMode = psModeCombo.value }
    }

    Column {
        anchors.fill: parent
        spacing: 8
        topPadding: 12

        // Mode selector bar
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
                    text: "VOICE"
                    color: SyTheme.textDimmed
                    font.pixelSize: SyTheme.fontLabel
                    font.family: SyTheme.fontFamily
                    font.capitalization: Font.AllUppercase
                    anchors.verticalCenter: parent.verticalCenter
                }

                SyComboBox {
                    id: psModeCombo
                    hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "00"
                    labelWidth: 0
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }

        Rectangle { width: parent.width; height: 1; color: SyTheme.divider }

        Item {
            width: parent.width
            height: parent.height - 60

            // Mode 0: 1 voice
            Column {
                anchors.centerIn: parent
                spacing: 8
                visible: root.psMode === 0

                Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 12
                    SyComboBox { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "04" }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "06"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "01"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "05"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0B"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0A"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "03"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                }
            }

            // Mode 1 or 2: 2 voices
            Column {
                anchors.centerIn: parent
                spacing: 8
                visible: root.psMode === 1 || root.psMode === 2

                // Voice 1
                Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 12
                    SyComboBox { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "04" }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "06"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "01"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "05"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0B"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                }

                Rectangle { width: parent.width - 48; height: 1; color: SyTheme.divider; anchors.horizontalCenter: parent.horizontalCenter }

                // Voice 2
                Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 12
                    SyComboBox { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0C" }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0E"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "02"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0D"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                }

                // Level
                Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 12
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "0A"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "12"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                    FilmstripKnob { hex0: "10"; hex1: root.hex1; hex2: root.hex2; hex3: "03"; filmstrip: SyTheme.knobSmallSrc; frameSize: SyTheme.knobSmall }
                }
            }
        }
    }
}
