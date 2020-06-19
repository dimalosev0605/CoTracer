import QtQuick 2.14
import QtQuick.Controls 2.12

import "../buttons"
import My_friends_model_qml 1.0

Rectangle {
    id: root
    z: 0

    My_friends_model {
        id: my_friends_model
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
    ListView {
        id: my_friends_list_view
        z: 1
        anchors {
            top: parent.top
            topMargin: 5
            left: parent.left
            right: parent.right
            bottom: back_btn.top
            bottomMargin: 5
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
                                             "type": false
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
