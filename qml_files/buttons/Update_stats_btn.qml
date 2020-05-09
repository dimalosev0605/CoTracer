import QtQuick 2.14
import QtQuick.Controls 2.12

Rectangle {
    id: root
    radius: 5
    border.width: 1
    border.color: "#000000"

    property alias mouse_area: mouse_area

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
        text: "Update"
    }
    MouseArea {
        id: mouse_area
        anchors.centerIn: parent
        width: parent.width + 3
        height: parent.height + 3
    }
}
