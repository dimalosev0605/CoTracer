import QtQuick 2.14
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0

import "../buttons"
import Found_friends_model_qml 1.0

Rectangle {
    id: root
    z: 0

    Found_friends_model {
        id: found_friends_model
    }

//    MouseArea {
//        id: root_m_area
//        z: 1
//        anchors.fill: parent
//        onClicked: {
//            Qt.inputMethod.hide()
//            nickname_field.focus = false
//            stack_view.focus = true
//        }
//    }

    Connections {
        id: keyboard_connections
        target: Qt.inputMethod
        onKeyboardRectangleChanged: {
            if(Qt.inputMethod.keyboardRectangle.height !== 0) {
//                back_btn.y = Qt.inputMethod.keyboardRectangle.y / Screen.devicePixelRatio
//                - back_btn.height - back_btn.bottom_margin
            }
            else {
//                back_btn.y = root.height - back_btn.height - back_btn.bottom_margin
            }
        }
    }

    Connections {
        id: found_friends_connection
        target: client
        onFriends_found: {
            found_friends_model.receive_found_friends(found_friends)
        }
    }

    Back_btn {
        id: back_btn
        z: 1
        y: root.height - height - bottom_margin
        property int bottom_margin: 10
        anchors {
            left: parent.left
            leftMargin: 5
        }
        color: mouse_area.pressed ? "#708090" : parent.color
    }

    TextField {
        id: nickname_field
        z: 1
        anchors {
            top: parent.top
            topMargin: 10
            horizontalCenter: parent.horizontalCenter
        }
        width: parent.width * 0.9
        height: 30
        placeholderText: "Enter a nickname"
        cursorVisible: activeFocus
        inputMethodHints: Qt.ImhNoPredictiveText
        font.pointSize: height * 0.4
        Keys.onPressed: {
            if(event.key === Qt.Key_Return) {
                event.accepted = true
                nickname_field.focus = false
                stack_view.focus = true
                client.find_friends(nickname_field.text)
            }
        }
    }

    ListView {
        id: found_friends_list_view
        z: 1
        anchors {
            left: parent.left
            right: parent.right
            top: nickname_field.bottom
            topMargin: 5
            bottom: back_btn.top
            bottomMargin: 5
        }
        orientation: ListView.Vertical
        clip: true
        model: found_friends_model
        delegate: Rectangle {
            id: delegate
            width: parent.width
            height: 50
            color: delegate_mouse_area.pressed ? "#b22222" : root.color
            Image {
                id: avatar
                anchors {
                    left: parent.left
                    leftMargin: 5
                    verticalCenter: parent.verticalCenter
                }
                height: parent.height - 5
                width: height
                source: model.avatar_path
                mipmap: true
                asynchronous: true
                fillMode: Image.PreserveAspectCrop
                layer.enabled: true
                layer.effect: OpacityMask {
                    maskSource: Rectangle {
                        width: avatar.width
                        height: avatar.height
                        radius: 5
                    }
                }
            }
            Text {
                id: nickname
                anchors {
                    left: avatar.right
                    top: parent.top
                }
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                height: parent.height
                width: parent.width - add_friend.width - add_friend.anchors.rightMargin - avatar.width
                - avatar.anchors.leftMargin
                fontSizeMode: Text.Fit
                minimumPointSize: 1
                font.pointSize: 12
                elide: Text.ElideRight
                wrapMode: Text.WordWrap
                text: String(model.nickname)
            }
            MouseArea {
                id: delegate_mouse_area
                anchors {
                    top: parent.top
                    bottom: parent.bottom
                    left: parent.left
                    right: add_friend.left
                }
            }
            Add_contact_btn {
                id: add_friend
                anchors {
                    right: parent.right
                    rightMargin: 5
                    verticalCenter: parent.verticalCenter
                }
                height: parent.height * 0.7
                width: height
                color: mouse_area.pressed ? "#00ff00" : root.color
                mouse_area.onClicked: {
                    client.add_in_my_friends(nickname.text)
                    console.log("CLICKED!")
                }
            }
        }
    }
}
