import QtQuick 2.14
import QtQuick.Controls 2.12

Rectangle {

    property alias mouse_area: mouse_area

    Image {
        anchors.fill: parent
//        source: ""
    }
    MouseArea {
        id: mouse_area
        anchors.centerIn: parent
        width: parent.width + 3
        height: parent.height + 3
    }
}
