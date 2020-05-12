import QtQuick 2.14
import QtQuick.Controls 2.12

Image {
    property alias mouse_area: mouse_area
    property bool is_visible: false
    source: !is_visible ? "qrc:/imgs/opened_eye.png" : "qrc:/imgs/closed_eye.png"

    MouseArea {
        id: mouse_area
        anchors.centerIn: parent
        width: parent.width + 4
        height: parent.height + 4
    }

}

