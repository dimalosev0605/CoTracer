import QtQuick 2.14
import QtQuick.Controls 2.12

import "../../buttons"
import "../"
import Avatar_selector_qml 1.0

Rectangle {
    id: root
    z: 0
//    Component {
//        id: file_dialog_comp
//        File_dialog {
//            id: file_dialog
//            color: root.color
//        }
//    }

    Avatar_selector {
        id: avatar_selector
        onImage_selected: {
            avatar.source = path
            save_changes_btn.visible = true
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
        cache: false
        width: parent.width * 0.9
        height: (parent.height / 2) - anchors.topMargin
        fillMode: Image.PreserveAspectFit
        source: client.get_avatar_path(true)
    }

    property int btns_width: 200
    property int btns_height: 40
    Main_menu_btn {
        id: select_avatar_btn
        z: 1
        anchors {
            top: avatar.bottom
            topMargin: 10
            horizontalCenter: parent.horizontalCenter
        }
        height: root.btns_height
        width: root.btns_width
        color: mouse_area.pressed ? "#b22222" : root.color
        text.text: "Select image"
        radius: 4
        border.width: 1
        border.color: "#000000"
        mouse_area.onClicked: {
//            stack_view.push(file_dialog_comp)
            avatar_selector.select_avatar()
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
        height: root.btns_height
        width: root.btns_width
        color: mouse_area.pressed ? "#b22222" : root.color
        text.text: "Save"
        text.font.bold: true
        radius: 4
        border.width: 2
        border.color: "#000000"
        visible: false
        mouse_area.onClicked: {
            client.change_avatar(avatar_selector.get_selected_image_path())
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
