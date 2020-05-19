import QtQuick 2.14
import QtQuick.Controls 2.12

import "../../buttons"
import "../"

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

    Image {
        id: avatar
        z: 0
        anchors {
            top: parent.top
            topMargin: 5
            horizontalCenter: parent.horizontalCenter
        }
        width: parent.width * 0.8
        height: width
        fillMode: Image.PreserveAspectFit
        source: client.get_avatar_path(true)
    }

    Main_menu_btn {
        id: select_avatar_btn
        z: 0
        anchors {
            top: avatar.bottom
            topMargin: 10
            horizontalCenter: parent.horizontalCenter
        }
        height: 50
        width: parent.width * 0.7
        color: mouse_area.pressed ? "#b22222" : root.color
        text.text: "Select image"
        mouse_area.onClicked: {
            stack_view.push(file_dialog_comp)
        }
    }

    Main_menu_btn {
        id: save_changes_btn
        z: 0
        anchors {
            top: select_avatar_btn.bottom
            topMargin: 10
            horizontalCenter: parent.horizontalCenter
        }
        height: select_avatar_btn.height
        width: parent.width * 0.7
        color: mouse_area.pressed ? "#b22222" : root.color
        text.text: "Save changes"
        visible: false
        mouse_area.onClicked: {
            client.change_avatar(avatar.source)
        }
    }

    Back_btn {
        id: back_btn
        z: 0
        anchors {
            bottom: parent.bottom
            bottomMargin: 5
            left: parent.left
            leftMargin: 5
        }
        color: mouse_area.pressed ? "#708090" : parent.color
    }
}
