import QtQuick

Rectangle {
    id: root
    color: "#1e2228"

    // Segmented toggle: PRESET / USER — matching Boss Tone Studio tab style
    Row {
        id: modeRow
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 34
        spacing: 0

        Rectangle {
            width: parent.width / 2
            height: parent.height
            color: patchListModel.presetMode ? "#2a3040" : "transparent"

            Rectangle {
                anchors.bottom: parent.bottom
                width: parent.width; height: 2
                color: SyTheme.accent
                visible: patchListModel.presetMode
            }

            Text {
                anchors.centerIn: parent
                text: "PRESET"
                color: patchListModel.presetMode ? SyTheme.accent : SyTheme.textDimmed
                font.pixelSize: 12
                font.family: SyTheme.fontFamily
                font.bold: true
            }

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: patchListModel.presetMode = true
            }
        }

        Rectangle {
            width: parent.width / 2
            height: parent.height
            color: !patchListModel.presetMode ? "#2a3040" : "transparent"

            Rectangle {
                anchors.bottom: parent.bottom
                width: parent.width; height: 2
                color: SyTheme.accent
                visible: !patchListModel.presetMode
            }

            Text {
                anchors.centerIn: parent
                text: "USER"
                color: !patchListModel.presetMode ? SyTheme.accent : SyTheme.textDimmed
                font.pixelSize: 12
                font.family: SyTheme.fontFamily
                font.bold: true
            }

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: patchListModel.presetMode = false
            }
        }
    }

    // Patch list — two-line per entry matching Boss Tone Studio
    ListView {
        id: patchList
        anchors.top: modeRow.bottom
        anchors.left: parent.left
        anchors.right: scrollTrack.left
        anchors.bottom: parent.bottom
        clip: true
        model: patchListModel

        currentIndex: patchListModel.selectedIndex
        onCurrentIndexChanged: {
            if (currentIndex >= 0)
                positionViewAtIndex(currentIndex, ListView.Contain)
        }

        delegate: Rectangle {
            id: delegateRoot
            width: patchList.width
            height: 48
            color: index === patchListModel.selectedIndex ? "#2a3040" : "transparent"

            // Left highlight bar for selected item
            Rectangle {
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: 3
                color: SyTheme.accent
                visible: index === patchListModel.selectedIndex
            }

            // Two-line layout: bank-patch ID + patch name
            Column {
                anchors.left: parent.left
                anchors.leftMargin: 12
                anchors.verticalCenter: parent.verticalCenter
                spacing: 1

                Text {
                    text: bankLabel
                    color: index === patchListModel.selectedIndex ? SyTheme.textPrimary : SyTheme.textLabel
                    font.pixelSize: SyTheme.fontLabel
                    font.family: SyTheme.fontFamily
                }

                Text {
                    text: patchName
                    color: index === patchListModel.selectedIndex ? SyTheme.textPrimary : "#dddddd"
                    font.pixelSize: 13
                    font.family: SyTheme.fontFamily
                    font.bold: true
                    elide: Text.ElideRight
                    width: delegateRoot.width - 24
                }
            }

            // Bottom separator
            Rectangle {
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 8
                anchors.rightMargin: 8
                height: 1
                color: SyTheme.bgControl
            }

            MouseArea {
                id: delegateMouse
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {
                    patchListModel.selectedIndex = index
                    patchListModel.itemClicked(index)
                }
            }

            // Hover highlight
            Rectangle {
                anchors.fill: parent
                color: "#ffffff"
                opacity: 0.04
                visible: delegateMouse.containsMouse && index !== patchListModel.selectedIndex
            }
        }
    }

    // Scrollbar — sibling of ListView, not child (fixes scroll-with-content bug)
    Rectangle {
        id: scrollTrack
        anchors.right: parent.right
        anchors.top: modeRow.bottom
        anchors.bottom: parent.bottom
        width: 4
        color: "transparent"
        visible: patchList.contentHeight > patchList.height

        Rectangle {
            width: parent.width
            radius: 2
            color: "#555"
            y: {
                if (patchList.contentHeight <= patchList.height) return 0
                var frac = patchList.contentY / (patchList.contentHeight - patchList.height)
                frac = Math.max(0, Math.min(1, frac))
                return frac * (parent.height - height)
            }
            height: patchList.contentHeight > 0
                    ? Math.max(20, patchList.height / patchList.contentHeight * parent.height) : 0
        }
    }
}
