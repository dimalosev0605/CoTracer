import QtQuick 2.14
import QtQuick.Controls 2.12

Rectangle {

    property alias mouse_area: mouse_area

    Image {
        id: img
        anchors.fill: parent
        height: parent.height
        width: height
        source: "qrc:/imgs/add.png"
    }
    MouseArea {
        id: mouse_area
        anchors.centerIn: parent
        width: parent.width + 3
        height: parent.height + 3
    }
}
