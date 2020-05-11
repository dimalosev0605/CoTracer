import QtQuick 2.14
import QtQuick.Controls 2.12

import "../buttons"

Rectangle {
    id: root
    z: 0

    Component {
        id: file_dialog_comp
        File_dialog {
            id: file_dialog
            color: root.color
        }
    }

    Back_btn {
        id: back_btn
        z: 1
        anchors {
            left: parent.left
            leftMargin: 5
            top: parent.top
            topMargin: 5
        }
        color: mouse_area.pressed ? "#708090" : parent.color
    }

    Image {
        id: avatar
        z: 1
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: back_btn.bottom
            topMargin: 10
        }
        width: parent.width * 0.8
        height: width
    }

    Main_menu_btn {
        id: select_avatar_btn
        z: 1
        anchors {
            top: avatar.bottom
            topMargin: 10
            horizontalCenter: parent.horizontalCenter
        }
        height: 30
        width: parent.width * 0.7
        color: mouse_area.pressed ? "#b22222" : root.color
        text.text: "Select photo"
        mouse_area.onClicked: {
            stack_view.push(file_dialog_comp)
        }
    }

    TextField {
        id: user_nickname_field
        z: 1
        anchors {
            top: select_avatar_btn.bottom
            topMargin: 10
            horizontalCenter: parent.horizontalCenter
        }
        height: 30
        width: select_avatar_btn.width
        text: client.get_nickname()
    }

    Main_menu_btn {
        id: save_changes_btn
        z: 1
        anchors {
            bottom: parent.bottom
            bottomMargin: 10
            horizontalCenter: parent.horizontalCenter
        }
        height: 30
        width: parent.width * 0.7
        color: mouse_area.pressed ? "#b22222" : root.color
        text.text: "Save Changes"
        mouse_area.onClicked: {
            client.change_avatar(avatar.source)
        }
    }
}
