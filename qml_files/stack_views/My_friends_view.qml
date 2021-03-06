import QtQuick 2.14
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0

import "../buttons"
import My_friends_model_qml 1.0

Rectangle {
    id: root
    z: 0

    My_friends_model {
        id: my_friends_model
    }

    Connections {
        id: success_friend_adding_connection
        target: client
        ignoreUnknownSignals: true
        onSuccess_friend_adding: {
            my_friends_model.add_friend(new_friend)
        }
        onSuccess_friend_removing: {
            my_friends_model.remove_friend(removed_friend)
        }
    }

    Component {
        id: add_friend_comp
        Add_friend_view {
            color: root.color
        }
    }

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

    Text {
        id: lbl
        z: 1
        anchors {
            top: parent.top
            topMargin: add_friend_btn.anchors.topMargin
        }
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        width: parent.width - add_friend_btn.width - add_friend_btn.anchors.rightMargin
        height: back_btn.height
        fontSizeMode: Text.Fit
        font.bold: true
        minimumPointSize: 1
        font.pointSize: 15
        elide: Text.ElideRight
        wrapMode: Text.NoWrap
        text: "Your friends"
    }

    Add_contact_btn {
        id: add_friend_btn
        z: 1
        anchors {
            top: parent.top
            topMargin: 5
            right: parent.right
            rightMargin: 5
        }
        height: 35
        width: height
        color: mouse_area.pressed ? "#00ff00" : parent.color
        mouse_area.onClicked: {
            stack_view.push(add_friend_comp)
        }
    }

    ListView {
        id: my_friends_list_view
        z: 1
        anchors {
            top: add_friend_btn.bottom
            topMargin: 3
            bottom: back_btn.top
            bottomMargin: 3
            left: parent.left
            right: parent.right
        }
        orientation: ListView.Vertical
        clip: true
        model: my_friends_model
        delegate: Component {
            Loader {
                id: loader

                property bool delegate_type: model.delegate_type
                property string nick: String(model.friend_nickname)
                property string src: model.friend_avatar_path

                Component.onCompleted: {
                    if(loader.delegate_type) {
                        loader.setSource("qrc:/qml_files/delegates/My_friends_view_user_delegate.qml",
                                         {
                                             "width": my_friends_list_view.width,
                                             "nick": loader.nick,
                                             "src": loader.src,
                                             "default_color": root.color,
                                             "type": true
                                         });
                    }
                    else {
                        loader.setSource("qrc:/qml_files/delegates/My_friends_view_alp_letter_delegate.qml",
                                         {
                                             "width": my_friends_list_view.width,
                                             "alphabet_letter": loader.nick,
                                             "color": root.color
                                         });
                    }
                }
            }
        }
    }
}
