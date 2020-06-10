import QtQuick 2.14

Rectangle {
    color: "#000000"
    Text {
        id: sorry_message
        anchors.centerIn: parent
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        width: parent.width
        height: parent.height
        fontSizeMode: Text.Fit
        minimumPointSize: 1
        font.pointSize: 15
        elide: Text.ElideRight
        wrapMode: Text.WordWrap
        color: "#ffffff"
        text: "Sorry:((("
    }
}
