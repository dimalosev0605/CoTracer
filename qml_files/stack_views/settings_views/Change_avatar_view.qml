import QtQuick 2.14
import QtQuick.Controls 2.12

import "../../buttons"
import "../"

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

    Image {
        id: avatar
        z: 1
        anchors {
            top: parent.top
            topMargin: 5
            horizontalCenter: parent.horizontalCenter
        }
        width: parent.width * 0.9
        height: (parent.height / 2) - anchors.topMargin
        fillMode: Image.PreserveAspectFit
        source: client.get_avatar_path(true)
    }

    property int max_btns_width: 200
    property int max_btns_height: 40
    Main_menu_btn {
        id: select_avatar_btn
        z: 1
        anchors {
            top: avatar.bottom
            topMargin: 10
            horizontalCenter: parent.horizontalCenter
        }
        height: if((parent.height - avatar.height - avatar.anchors.topMargin - anchors.topMargin -
                save_changes_btn.anchors.topMargin * 2) / 2 > parent.max_btns_height) {
                    parent.max_btns_height
                }
                else {
                    (parent.height - avatar.height - avatar.anchors.topMargin - anchors.topMargin -
                                    save_changes_btn.anchors.topMargin * 2) / 2
                }
        width: if(parent.width < parent.max_btns_width) {
                   parent.width * 0.95 - back_btn.width * 2
               }
               else {
                   parent.max_btns_width
               }
        color: mouse_area.pressed ? "#b22222" : root.color
        text.text: "Select image"
        radius: 4
        border.width: 1
        border.color: "#000000"
        mouse_area.onClicked: {
            stack_view.push(file_dialog_comp)
        }
    }

    Main_menu_btn {
        id: save_changes_btn
        z: 1
        anchors {
            top: select_avatar_btn.bottom
            topMargin: 10
            horizontalCenter: parent.horizontalCenter
        }
        height: select_avatar_btn.height
        width: select_avatar_btn.width
        color: mouse_area.pressed ? "#b22222" : root.color
        text.text: "Save changes"
        text.font.bold: true
        radius: 4
        border.width: 2
        border.color: "#000000"
        visible: false
        mouse_area.onClicked: {
            client.change_avatar(avatar.source)
        }
    }

    Back_btn {
        id: back_btn
        z: 1
        anchors {
            bottom: parent.bottom
            bottomMargin: 5
            left: parent.left
            leftMargin: 5
        }
        color: mouse_area.pressed ? "#708090" : parent.color
    }
}
