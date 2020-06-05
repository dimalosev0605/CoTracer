import QtQuick 2.14
import QtQuick.Controls 2.12

Rectangle {
    id: root

    property alias mouse_area: mouse_area
    property alias rotation_anim: rotation_anim

    Image {
        id: img
        anchors {
            verticalCenter: parent.verticalCenter
        }
        source: "qrc:/imgs/update.png"
        height: parent.height
        width: height
        fillMode: Image.PreserveAspectFit
    }
    RotationAnimation {
        id: rotation_anim
        target: img
        duration: 750
        loops: Animation.Infinite
        running: false
        alwaysRunToEnd: true
        from: 0
        to: 360
    }
    MouseArea {
        id: mouse_area
        anchors.centerIn: parent
        width: parent.width + 3
        height: parent.height + 3
    }
}
