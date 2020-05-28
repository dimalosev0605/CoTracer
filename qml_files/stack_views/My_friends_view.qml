import QtQuick 2.14
import QtQuick.Controls 2.12

import "../buttons"

Rectangle {
    z: 0

    Back_btn {
        id: back_btn
        z: 1
        anchors {
            left: parent.left
            leftMargin: 5
            bottom: parent.bottom
            bottomMargin: 5
        }
        color: mouse_area.pressed ? "#708090" : parent.color
    }


}
