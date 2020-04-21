import QtQuick 2.14
import QtQuick.Controls 2.12

Rectangle {
    id: root

    property alias text: text
    property alias busy_indicator: busy_indicator
    property alias opacity_anim: opacity_anim

    function show_dialog(is_busy, message) {
        my_dialog.busy_indicator.running = is_busy
        my_dialog.text.text = message
        my_dialog.visible = true
    }

    color: "#c0c0c0"
    border.width: 1
    border.color: "#000000"
    radius: 5

    BusyIndicator {
        id: busy_indicator
        anchors {
            top: parent.top
            topMargin: 2
            horizontalCenter: parent.horizontalCenter
        }
        height: parent.height * 0.5
        width: height
    }

    Text {
        id: text
        anchors {
            top: busy_indicator.bottom
            topMargin: 3
        }
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        width: parent.width
        height: parent.height - busy_indicator.height - busy_indicator.anchors.topMargin
                - anchors.topMargin
        fontSizeMode: Text.Fit
        minimumPointSize: 5
        font.pointSize: 15
        elide: Text.ElideRight
        wrapMode: Text.WordWrap
        text: "Connecting to server..."
    }
    OpacityAnimator {
        id: opacity_anim
        target: root
        from: root.opacity
        to: 0
        duration: 2500
        running: false
        onFinished: {
            root.opacity = 0.7
            root.visible = false
        }
    }
}
