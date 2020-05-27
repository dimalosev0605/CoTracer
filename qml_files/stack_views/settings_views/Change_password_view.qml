import QtQuick 2.14
import QtQuick.Controls 2.12

import "../../buttons"
import "../../buttons"

Rectangle {
    id: root
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

    Column {
        id: column
        z: 1
        anchors {
            top: parent.top
            topMargin: 10
            horizontalCenter: parent.horizontalCenter
        }
        width: parent.width * 0.7
        spacing: 10

        TextField {
            id: old_password_field
            width: parent.width
            height: 30
            placeholderText: "Input old password"
            echoMode: TextInput.Password
            inputMethodHints: Qt.ImhNoPredictiveText
            onTextChanged: if(old_password_field.text !== client.get_password()) {
                               info_text.text = "Incorrect password"
                               new_password_field.enabled = false
                               repeat_password_field.enabled = false
                           }
                           else {
                               new_password_field.enabled = true
                               repeat_password_field.enabled = true
                               info_text.text = ""
                               if(new_password_field.text !== repeat_password_field.text) {
                                   info_text.text = "Passwords don't match!"
                               }
                               else {
                                   info_text.text = ""
                               }
                           }
        }
        TextField {
            id: new_password_field
            width: parent.width
            height: old_password_field.height
            placeholderText: "Input new password"
            enabled: false
            echoMode: TextInput.Password
            inputMethodHints: Qt.ImhNoPredictiveText
            onTextChanged: {
                if(new_password_field.text !== repeat_password_field.text) {
                    info_text.text = "Passwords don't match!"
                }
                else {
                    info_text.text = ""
                }
            }
        }
        TextField {
            id: repeat_password_field
            width: parent.width
            height: old_password_field.height
            placeholderText: "Repeat new password"
            echoMode: TextInput.Password
            inputMethodHints: Qt.ImhNoPredictiveText
            enabled: false
            onTextChanged: if(new_password_field.text !== repeat_password_field.text) {
                               info_text.text = "Passwords don't match!"
                           }
                           else {
                               info_text.text = ""
                           }
        }
        Text {
            id: info_text
            width: parent.width
            height: 40
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            fontSizeMode: Text.Fit
            minimumPointSize: 5
            font.pointSize: 12
            elide: Text.ElideRight
            wrapMode: Text.WordWrap
        }
        Main_menu_btn {
            id: change_password_btn
            width: parent.width
            height: 50
            color: mouse_area.pressed ? "#b22222" : root.color
            text.text: "Change password"
            mouse_area.onClicked: {
                client.change_password(new_password_field.text)
            }
        }
    }
}
