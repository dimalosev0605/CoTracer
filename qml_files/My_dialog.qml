import QtQuick 2.14
import QtQuick.Controls 2.12

Rectangle {
    id: root

    property string message
    property int opacity_anim_duration
    property bool is_busy_indicator_running
    property bool is_opacity_anim_running
    property bool is_destroy

    anchors.centerIn: parent
    color: "#c0c0c0"
    border.width: 1
    border.color: "#000000"
    radius: 5
    opacity: 0.7

    width: if(parent.width < parent.height) {
               parent.width * 0.70
           }
           else {
               parent.width * 0.4
           }
    height: if(parent.height > parent.width) {
                parent.height * 0.25
            }
            else {
                parent.height * 0.4
            }

    BusyIndicator {
        id: busy_indicator
        anchors {
            top: parent.top
            topMargin: 2
            horizontalCenter: parent.horizontalCenter
        }
        height: parent.height * 0.5
        width: height
        running: is_busy_indicator_running
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
        text: message
    }
    OpacityAnimator {
        id: opacity_anim
        target: root
        from: root.opacity
        to: 0
        duration: opacity_anim_duration
        running: !is_opacity_anim_running
        onFinished: {
            if(is_destroy) {
                root.destroy()
            }
        }
    }
    Connections {
        target: client
        ignoreUnknownSignals: true
        onChange_dialog: {
            message = m_message;
            opacity_anim_duration = m_opacity_anim_duration;
            is_busy_indicator_running = m_is_busy_indicator_running;
            is_opacity_anim_running = m_is_opacity_anim_running;
            is_destroy = m_is_destroy;
            opacity_anim.restart()
        }
    }
}
