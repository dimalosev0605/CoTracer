import QtQuick 2.14
import QtQuick.Controls 2.12

Rectangle {
    id: root

    property string message
    property int opacity_anim_duration
    property bool is_busy_running
    property bool is_static

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
        running: is_busy_running
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
        running: !is_static
        onFinished: {
            root.destroy()
        }
    }
    Connections {
        target: client
        ignoreUnknownSignals: true
        onInfo: {
            if(opacity_anim_duration === Animation.Infinite) {
                root.destroy()
            }
        }
        onContacts_received: {
            if(opacity_anim_duration === Animation.Infinite) {
                root.destroy()
            }
        }
        onSuccess_contact_deletion: {
            if(opacity_anim_duration === Animation.Infinite) {
                root.destroy()
            }
        }
        onSuccess_adding: {
            if(opacity_anim_duration === Animation.Infinite) {
                root.destroy()
            }
        }
        onStatistics_received: {
            if(opacity_anim_duration === Animation.Infinite) {
                root.destroy()
            }
        }
    }
}
