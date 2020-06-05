import QtQuick 2.14
import QtQuick.Controls 2.12

import "../buttons"
import ".."

Rectangle {
    id: root
    z: 0

    Component {
        id: account_settings_view_comp
        Account_settings_view {
            id: account_settings_view
            color: root.color
        }
    }

    Connections {
        target: client
        ignoreUnknownSignals: true
        onUpdate_password_field: {
            password_field.text = client.get_password()
        }
    }

    Back_btn {
        id: back_btn
        z: 1
        height: exit_from_account_btn.height
        width: height
        anchors {
            left: parent.left
            leftMargin: 5
            bottom: parent.bottom
            bottomMargin: exit_from_account_btn.anchors.bottomMargin
        }
        color: mouse_area.pressed ? "#708090" : parent.color
    }

    Settings_btn {
        id: settings_btn
        z: 1
        width: back_btn.width
        height: back_btn.height
        radius: back_btn.radius
        anchors {
            right: parent.right
            rightMargin: back_btn.anchors.leftMargin
            bottom: parent.bottom
            bottomMargin: back_btn.anchors.bottomMargin
        }
        color: mouse_area.pressed ? "#708090" : parent.color
        visible: client.is_authorized
        mouse_area.onClicked: {
            stack_view.push(account_settings_view_comp)
        }
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
        enabled: !client.is_authorized
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
        enabled: !client.is_authorized
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
        radius: 4

        visible: !client.is_authorized
        text.text: "Sing up"
        mouse_area.onClicked: {
            if(nickname_field.text === "" || password_field.text === "") return
            client.sign_up(nickname_field.text, password_field.text)
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
        radius: 4

        visible: !client.is_authorized
        text.text: "Sing in"
        mouse_area.onClicked: {
            if(nickname_field.text === "" || password_field.text === "") return
            client.sign_in(nickname_field.text, password_field.text)
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
        width: if(sign_up_btn.width  > parent.width - back_btn.width - settings_btn.width - back_btn.anchors.leftMargin -
                  settings_btn.anchors.rightMargin - 2 * back_btn.anchors.leftMargin)
               {
                   parent.width - back_btn.width - settings_btn.width - back_btn.anchors.leftMargin -
                   settings_btn.anchors.rightMargin - 2 * back_btn.anchors.leftMargin
               }
               else
               {
                   sign_up_btn.width
               }
        height: sign_up_btn.height
        color: mouse_area.pressed ? "#af1111" : "#b22222"
        radius: 4

        visible: client.is_authorized
        text.text: "Exit"
        mouse_area.onClicked: {
            if(client.exit_from_account()) {
                nickname_field.text = ""
                password_field.text = ""
            }
        }
    }
}
