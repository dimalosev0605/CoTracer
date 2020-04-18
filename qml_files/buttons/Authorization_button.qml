import QtQuick 2.14
import QtQuick.Controls 2.12

Rectangle {

    property alias text: text
    property alias mouse_area: mouse_area
    radius: 1

    Text {
        id: text
        anchors.centerIn: parent
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        width: parent.width
        height: parent.height
        fontSizeMode: Text.Fit
        minimumPointSize: 5
        font.pointSize: 15
        elide: Text.ElideRight
        wrapMode: Text.WordWrap
    }
    MouseArea {
        id: mouse_area
        anchors.fill: parent
    }

}
