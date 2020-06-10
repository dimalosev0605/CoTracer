import QtQuick 2.14
import QtQuick.Controls 2.12

Rectangle {

    width: 30
    height: width
    radius: 5
    property alias mouse_area: mouse_area

    Image {
        anchors.fill: parent
        source: "qrc:/imgs/back.png"
    }
    MouseArea {
        id: mouse_area
        anchors.centerIn: parent
        width: parent.width + 3
        height: parent.height + 3
        onClicked: stack_view.pop()
    }

}
