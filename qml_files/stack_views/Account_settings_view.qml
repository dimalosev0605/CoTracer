import QtQuick 2.14
import QtQuick.Controls 2.12

import "../buttons"
import "../Create_dialog.js" as Create_dialog

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
        source: client.get_avatar_path(true)
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
        id: nickname_field
        z: 1
        anchors {
            top: select_avatar_btn.bottom
            topMargin: 10
            horizontalCenter: parent.horizontalCenter
        }
        height: 30
        width: select_avatar_btn.width
        text: client.get_nickname()
        inputMethodHints: Qt.ImhNoPredictiveText
    }

    TextField {
        id: password_field
        z: 1
        anchors {
            top: nickname_field.bottom
            topMargin: 10
            horizontalCenter: parent.horizontalCenter
        }
        height: nickname_field.height
        width: nickname_field.width
        text: client.get_password()
        echoMode: TextInput.Password
        inputMethodHints: Qt.ImhNoPredictiveText
        rightPadding: show_password_btn.width + show_password_btn.anchors.rightMargin * 2
        Show_password_btn {
            id: show_password_btn
            height: parent.height * 0.9
            width: height
            anchors {
                verticalCenter: parent.verticalCenter
                right: parent.right
                rightMargin: 2
            }
            mouse_area.onClicked: {
                if(is_visible) {
                    is_visible = false
                    password_field.echoMode = TextInput.Password
                }
                else {
                    password_field.echoMode = TextInput.Normal
                    is_visible = true
                }
            }
        }
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
        visible: if(nickname_field.text === client.get_nickname() && password_field.text === client.get_password()) {
                     false
                 }
                 else {
                     true
                 }
        mouse_area.onClicked: {
            client.change_avatar(avatar.source)
        }
    }
}
