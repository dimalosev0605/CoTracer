import QtQuick 2.14
import QtQuick.Controls 2.12

import "../buttons"

Rectangle {
    id: root
    z: 0

    Component {
        id: add_friend_comp
        Add_friend_view {
            color: root.color
        }
    }

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

    Text {
        id: lbl
        z: 1
        anchors {
            top: parent.top
            topMargin: add_friend_btn.anchors.topMargin
        }
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        width: parent.width - add_friend_btn.width - add_friend_btn.anchors.rightMargin
        height: back_btn.height
        fontSizeMode: Text.Fit
        font.bold: true
        minimumPointSize: 1
        font.pointSize: 15
        elide: Text.ElideRight
        wrapMode: Text.NoWrap
        text: "Your friends"
    }

    Add_contact_btn {
        id: add_friend_btn
        z: 1
        anchors {
            top: parent.top
            topMargin: 5
            right: parent.right
            rightMargin: 5
        }
        height: 35
        width: height
        color: mouse_area.pressed ? "#00ff00" : parent.color
        mouse_area.onClicked: {
            stack_view.push(add_friend_comp)
        }
    }

    ListView {
        id: my_friends_list_view
        z: 1
        anchors {
            top: add_friend_btn.bottom
            topMargin: 3
            bottom: back_btn.top
            bottomMargin: 3
            left: parent.left
            right: parent.right
        }
        orientation: ListView.Vertical
        clip: true
//        model:
    }


}
