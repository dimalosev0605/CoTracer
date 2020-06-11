import QtQuick 2.14
import QtQuick.Controls 2.12
import QtQuick.Window 2.14

import "../../buttons"
import "../../buttons"

Rectangle {
    id: root
    z: 0

    MouseArea {
        id: root_m_area
        z: 1
        anchors.fill: parent
        onClicked: {
            Qt.inputMethod.hide()
            nickname_field.focus = false
            stack_view.focus = true
        }
    }

    Connections {
        id: keyboard_connections
        target: Qt.inputMethod
        onKeyboardRectangleChanged: {
            if(Qt.inputMethod.keyboardRectangle.height !== 0) {
                column.y = parent.height * 0.2
                back_btn.y = Qt.inputMethod.keyboardRectangle.y / Screen.devicePixelRatio
                - back_btn.height - back_btn.bottom_margin
            }
            else {
                column.y = parent.height * 0.35
                back_btn.y = root.height - back_btn.height - back_btn.bottom_margin
            }
        }
    }

    Back_btn {
        id: back_btn
        z: 1
        y: root.height - height - bottom_margin
        property int bottom_margin: 5
        anchors {
            left: parent.left
            leftMargin: 5
        }
        color: mouse_area.pressed ? "#708090" : parent.color
    }

    Column {
        id: column
        z: 1
        y: parent.height * 0.35
        anchors {
            horizontalCenter: parent.horizontalCenter
        }
        width: parent.width * 0.7
        spacing: 10

        TextField {
            id: old_password_field
            width: parent.width
            height: 30
            placeholderText: "Enter current password"
            echoMode: TextInput.Password
            inputMethodHints: Qt.ImhNoPredictiveText
            font.pointSize: height * 0.4
            cursorVisible: activeFocus
            onTextChanged: if(old_password_field.text !== client.get_password()) {
                               info_text.text = "Wrong password!"
                               info_text.visible = true
                               new_password_field.visible = false
                               repeat_password_field.visible = false
                           }
                           else {
                               new_password_field.visible = true
                               repeat_password_field.visible = true
                               info_text.text = ""
                               if(new_password_field.text !== repeat_password_field.text) {
                                   info_text.text = "Passwords don't match!"
                                   info_text.visible = true
                               }
                               else {
                                   info_text.visible = false
                               }
                           }
            Keys.onPressed: {
                if(event.key === Qt.Key_Return) {
                    event.accepted = true
                    focus = false
                    stack_view.focus = true
                }
            }
        }
        TextField {
            id: new_password_field
            width: parent.width
            height: old_password_field.height
            placeholderText: "Enter new password"
            visible: false
            echoMode: TextInput.Password
            inputMethodHints: Qt.ImhNoPredictiveText
            font.pointSize: height * 0.4
            cursorVisible: activeFocus
            onTextChanged: {
                if(new_password_field.text !== repeat_password_field.text) {
                    info_text.text = "Passwords don't match!"
                    info_text.visible = true
                }
                else {
                    info_text.visible = false
                }
            }
            Keys.onPressed: {
                if(event.key === Qt.Key_Return) {
                    event.accepted = true
                    focus = false
                    stack_view.focus = true
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
            font.pointSize: height * 0.4
            cursorVisible: activeFocus
            visible: false
            onTextChanged: if(new_password_field.text !== repeat_password_field.text) {
                               info_text.text = "Passwords don't match!"
                               info_text.visible = true
                           }
                           else {
                               info_text.visible = false
                           }
            Keys.onPressed: {
                if(event.key === Qt.Key_Return) {
                    event.accepted = true
                    focus = false
                    stack_view.focus = true
                }
            }
        }
        Text {
            id: info_text
            width: parent.width
            height: 40
            visible: false
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            fontSizeMode: Text.Fit
            minimumPointSize: 5
            font.pointSize: 15
            elide: Text.ElideRight
            wrapMode: Text.WordWrap
        }
        Main_menu_btn {
            id: change_password_btn
            width: parent.width
            height: 50
            color: mouse_area.pressed ? "#b22222" : root.color
            radius: 5
            border.width: 1
            border.color: "#000000"
            visible: if(old_password_field.text === client.get_password() &&
                        new_password_field.text === repeat_password_field.text &&
                        new_password_field.text !== "" &&
                        repeat_password_field.text !== "")
                     {
                         true
                     }
                     else {
                         false
                     }
            text.text: "Change password"
            mouse_area.onClicked: {
                client.change_password(new_password_field.text)
                stack_view.focus = true
                Qt.inputMethod.hide()
            }
        }
    }
}
