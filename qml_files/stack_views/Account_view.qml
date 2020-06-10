import QtQuick 2.14
import QtQuick.Controls 2.12
import QtQuick.Window 2.14

import "../buttons"
import ".."

Rectangle {
    id: root
    z: 0

    MouseArea {
        id: root_m_area
        z: 0
        anchors.fill: parent
        onClicked: {
            nickname_field.focus = false
            password_field.focus = false
            stack_view.focus = true
            Qt.inputMethod.hide()
        }
    }

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

    Connections {
        id: keyboard_connections
        target: Qt.inputMethod
        onKeyboardRectangleChanged: {
            if(Qt.inputMethod.keyboardRectangle.height !== 0) {
                nickname_field.y = 10
                exit_from_account_btn.y = Qt.inputMethod.keyboardRectangle.y / Screen.devicePixelRatio
                - exit_from_account_btn.height - exit_from_account_btn.bottom_margin
            }
            else {
                nickname_field.focus = false
                password_field.focus = false
                stack_view.focus = true
                nickname_field.y = root.height * 0.35
                exit_from_account_btn.y = root.height - exit_from_account_btn.height
                - exit_from_account_btn.bottom_margin
            }
        }
    }



    property int auth_btn_h: 40 // sign in, sign up, exit from account

    Authorization_button {
        id: exit_from_account_btn
        z: 1
        property int bottom_margin: 5
        y: parent.height - height - bottom_margin
        anchors {
            horizontalCenter: parent.horizontalCenter
        }
        width: (parent.width - back_btn.width - settings_btn.width
        - back_btn.anchors.leftMargin - settings_btn.anchors.rightMargin) * 0.7
        height: parent.auth_btn_h
        color: mouse_area.pressed ? "#af1111" : "#b22222"
        radius: 4
        visible: client.is_authorized
        text.text: "Exit"
        mouse_area.onClicked: {
            if(client.exit_from_account()) {
                nickname_field.clear()
                password_field.clear()
            }
        }
    }
    Back_btn {
        id: back_btn
        z: 1
        focus: true
        anchors {
            left: parent.left
            leftMargin: 4
            verticalCenter: exit_from_account_btn.verticalCenter
        }
        color: mouse_area.pressed ? "#af1111" : parent.color
    }
    Settings_btn {
        id: settings_btn
        z: 1
        anchors {
            right: parent.right
            rightMargin: back_btn.anchors.leftMargin
            verticalCenter: exit_from_account_btn.verticalCenter
        }
        width: back_btn.width
        height: back_btn.height
        radius: back_btn.radius
        visible: client.is_authorized
        color: mouse_area.pressed ? "#af1111" : parent.color
        mouse_area.onClicked: {
            stack_view.push(account_settings_view_comp)
        }
    }
    TextField {
        id: nickname_field
        z: 1
        y: parent.height * 0.35
        anchors {
            horizontalCenter: parent.horizontalCenter
        }
        width: parent.width * 0.7
        height: 30
        placeholderText: "Nickname"
        cursorVisible: activeFocus
        inputMethodHints: Qt.ImhNoPredictiveText
        font.pointSize: height * 0.4
        enabled: !client.is_authorized
        text: client.get_nickname()
        Keys.onPressed: {
            if(event.key === Qt.Key_Return) {
                event.accepted = true
                nickname_field.focus = false
                stack_view.focus = true
            }
        }
    }
    TextField {
        id: password_field
        z: 1
        anchors {
            top: nickname_field.bottom
            topMargin: 10
            horizontalCenter: parent.horizontalCenter
        }
        width: nickname_field.width
        height: nickname_field.height
        placeholderText: "Password"
        cursorVisible: activeFocus
        inputMethodHints: Qt.ImhNoPredictiveText
        font.pointSize: height * 0.4
        echoMode: TextInput.Password
        enabled: !client.is_authorized
        text: client.get_password()
        Keys.onPressed: {
            if(event.key === Qt.Key_Return) {
                event.accepted = true
                password_field.focus = false
                stack_view.focus = true
            }
        }
    }
    Authorization_button {
        id: sign_in_btn
        z: 1
        anchors {
            top: password_field.bottom
            topMargin: 10
            horizontalCenter: parent.horizontalCenter
        }
        width: nickname_field.width
        height: parent.auth_btn_h
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
        id: sign_up_btn
        z: 1
        y: exit_from_account_btn.y
        anchors {
            horizontalCenter: exit_from_account_btn.anchors.horizontalCenter
        }
        width: exit_from_account_btn.width
        height: parent.auth_btn_h
        color: mouse_area.pressed ? "#ffffff" : parent.color
        radius: 4

        visible: !client.is_authorized
        text.text: "Sing up"
        mouse_area.onClicked: {
            if(nickname_field.text === "" || password_field.text === "") return
            client.sign_up(nickname_field.text, password_field.text)
        }
    }
}
