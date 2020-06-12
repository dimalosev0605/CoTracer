import QtQuick 2.14
import QtQuick.Controls 2.12
import QtQml 2.12

import "../../buttons"
import "../"
import Avatar_selector_qml 1.0

Rectangle {
    id: root
    z: 0

    Connections {
        target: client
        ignoreUnknownSignals: true
        onSuccess_setting_default_avatar: {
            avatar.source = client.get_avatar_path(true)
        }
    }

    Avatar_selector {
        id: avatar_selector
        onImage_selected: {
            save_changes_btn.visible = true
            avatar.source = path
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
        mipmap: true
        asynchronous: true
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

    Main_menu_btn {
        id: set_default_avatar_btn
        z: 1
        anchors {
            bottom: parent.bottom
            bottomMargin: back_btn.anchors.bottomMargin
            horizontalCenter: parent.horizontalCenter
        }
        height: root.btns_height
        width: root.btns_width
        color: mouse_area.pressed ? "#b22222" : root.color
        text.text: "Set default avatar"
        radius: 4
        visible: avatar.source.toString() === "qrc:/imgs/default_avatar.png" ? false : save_changes_btn.visible ? false : true
        border.width: 1
        border.color: "#000000"
        mouse_area.onClicked: {
            client.set_default_avatar()
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
