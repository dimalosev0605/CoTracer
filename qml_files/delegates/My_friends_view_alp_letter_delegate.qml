import QtQuick 2.14
import QtQuick.Controls 2.12

Rectangle {
    id: root

    property string alphabet_letter

    height: 20

    Text {
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignLeft
        leftPadding: 5
        height: parent.height
        width: parent.width - leftPadding
        fontSizeMode: Text.Fit
        minimumPointSize: 1
        font.pointSize: 12
        elide: Text.ElideRight
        wrapMode: Text.WordWrap
        text: root.alphabet_letter
    }
}
