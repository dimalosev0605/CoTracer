import QtQuick 2.14
import QtQuick.Controls 2.12

import Authorization_tcp_client_qml 1.0

Rectangle {

    Authorization_tcp_client {
        id: client
        onSuccess_connection: {
            info_text.text = "Connection completed"
        }
        onConnection_error: {
            info_text.text = "Connection error"
        }
        onSuccess_sign_in: {
            info_text.text = "Success sing in";
        }
        onSign_in_failure: {
            info_text.text = "Sing in fail";
        }
        onSuccess_sign_up: {
            info_text.text = "Success sign up!"
        }
        onSign_up_failure: {
            info_text.text = "Sign up fail"
        }
        onInternal_server_error: {
            info_text.text = "Server error"
        }
    }
    Text {
        id: info_text
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        text: "Connecting to server..."
    }


    Rectangle {
        width: 50
        height: 50
        color: "red"
        MouseArea {
            anchors.fill: parent
            onClicked: stack_view.pop()
        }
    }
    TextField {
        id: nickname_field
        anchors.centerIn: parent
        width: 200
        height: 30
        text: client.get_nickname()
    }
    TextField {
        id: password_field
        anchors.top: nickname_field.bottom
        anchors.left: nickname_field.left
        width: 200
        height: 30
        text: client.get_password()
    }
    Row {
        anchors.top: password_field.bottom
        anchors.left: password_field.left
        spacing: 10
        Text {
            id: sing_up_t
            text: "Sing up"
            MouseArea {
                anchors.fill: parent
                onClicked: client.sing_up(nickname_field.text, password_field.text)
            }
        }
        Text {
            id: sing_in_t
            text: "Sing in"
            MouseArea {
                anchors.fill: parent
                onClicked: client.sing_in(nickname_field.text, password_field.text)
            }
        }
        Text {
            id: exit_t
            text: "Exit"
            MouseArea {
                anchors.fill: parent
                onClicked: client.exit_from_account()
            }
        }
    }
}
