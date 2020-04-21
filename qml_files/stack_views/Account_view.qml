import QtQuick 2.14
import QtQuick.Controls 2.12

import Authorization_tcp_client_qml 1.0
import "../buttons"
import ".."

Rectangle {
    z: 0
    color: "#e00d0d"

    My_dialog {
        id: my_dialog
        z: 2
        anchors.centerIn: parent
        width: if(parent.height > parent.width) {
                   parent.width * 0.9
               }
               else {
                   parent.width * 0.5
               }

        height: if(parent.height > parent.width) {
                    parent.height * 0.2
                }
                else {
                    parent.height * 0.5
                }

        visible: true
        opacity: 0.7
    }

    Authorization_tcp_client {
        id: client
        onInfo: {
            my_dialog.busy_indicator.running = false
            my_dialog.text.text = info_message
            my_dialog.opacity_anim.start()
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
        id: microorganism_img
        z: 1
        anchors {
            horizontalCenter: parent.horizontalCenter
        }
        y: nickname_field.y / 2 - height / 2
        width: height
        height: if(parent.height > parent.width) {
                    nickname_field.y / 2
                }
                else {
                    nickname_field.y * 0.75
                }

        source: "qrc:/imgs/microorganism.png"
    }

    property real max_text_fields_width: 300
    property real text_fields_height: 30
    TextField {
        id: nickname_field
        z: 1
        anchors {
            horizontalCenter: parent.horizontalCenter
            bottom: password_field.top
            bottomMargin: 10
        }
        width: password_field.width
        height: parent.text_fields_height
        placeholderText: "Nickname"
        text: client.get_nickname()
    }
    TextField {
        // main item
        id: password_field
        z: 1
        anchors.centerIn: parent
        width: parent.width > parent.max_text_fields_width ? parent.max_text_fields_width * 0.8 : parent.width * 0.8
        height: parent.text_fields_height
        placeholderText: "Password"
        echoMode: TextInput.Password
        text: client.get_password()
    }

    property real max_btns_width: 250
    property int btns_height: 40

    Authorization_button {
        id: sign_up_btn
        z: 1
        anchors {
            bottom: parent.bottom
            bottomMargin: parent.height > parent.width ? 10 : 5
            horizontalCenter: parent.horizontalCenter
        }
        width: password_field.width
        height: parent.btns_height
        color: mouse_area.pressed ? "#ffffff" : parent.color

        visible: !client.is_authenticated
        text.text: "Sing up"
        mouse_area.onClicked: {
            if(nickname_field.text === "" || password_field.text === "") return
            if(client.is_connected) {
                if(client.sing_up(nickname_field.text, password_field.text)) {
                    my_dialog.show_dialog(true, "Please wait.")
                }
            }
        }
    }
    Authorization_button {
        id: sign_in_btn
        z: 1
        anchors {
            top: password_field.bottom
            topMargin: 15
            horizontalCenter: parent.horizontalCenter
        }
        width: password_field.width
        height: parent.btns_height
        color: mouse_area.pressed ? "#af1111" : "#b22222"

        visible: !client.is_authenticated
        text.text: "Sing in"
        mouse_area.onClicked: {
            if(nickname_field.text === "" || password_field.text === "") return
            if(client.is_connected) {
                if(client.sing_in(nickname_field.text, password_field.text)) {
                    my_dialog.show_dialog(true, "Please wait.")
                }
            }
        }
    }
    Authorization_button {
        id: exit_from_account_btn
        z: 1
        anchors {
            bottom: sign_up_btn.anchors.bottom
            bottomMargin: sign_up_btn.anchors.bottomMargin
            horizontalCenter: sign_up_btn.anchors.horizontalCenter
        }
        width: sign_up_btn.width
        height: sign_up_btn.height
        color: mouse_area.pressed ? "#af1111" : "#b22222"

        visible: client.is_authenticated
        text.text: "Exit"
        mouse_area.onClicked: {
            client.exit_from_account()
            nickname_field.text = ""
            password_field.text = ""
        }
    }
}
