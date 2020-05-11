import QtQuick 2.14
import QtQuick.Controls 2.12

import "../buttons"
import File_dialog_model_qml 1.0

Rectangle {
    id: root
    z: 0

    Return_back_btn {
        id: return_back_btn
        z: 1
        width: 35
        height: width
        color: "#000000"
        anchors {
            left: parent.left
            leftMargin: 5
            bottom: parent.bottom
            bottomMargin: 5
        }
        mouse_area.onClicked: {
            file_dialog_model.return_back()
        }
    }

    Main_menu_btn {
        id: cancel_btn
        z: 1
        anchors {
            bottom: parent.bottom
            bottomMargin: return_back_btn.anchors.bottomMargin
            left: return_back_btn.right
            leftMargin: return_back_btn.anchors.leftMargin
            right: parent.right
            rightMargin: anchors.leftMargin
        }
        width: parent.width * 0.8
        height: return_back_btn.height
        color: mouse_area.pressed ? "#b22222" : root.color
        text.text: "Cancel"
        mouse_area.onClicked: {
            stack_view.pop()
        }
    }

    File_dialog_model {
        id: file_dialog_model
        onPhoto: {
            console.log("path: " + path)
            avatar.source = path
            stack_view.pop()
        }
    }

    ListView {
        id: files_list_view
        z: 1
        anchors {
            top: parent.top
            topMargin: 10
            left: parent.left
            leftMargin: 5
            right: parent.right
            rightMargin: anchors.leftMargin
            bottom: cancel_btn.top
            bottomMargin: 5
        }
        orientation: ListView.Vertical
        clip: true
        spacing: 5
        model: file_dialog_model
        delegate: Rectangle {
            width: parent.width
            height: 40
            Text {
                anchors.centerIn: parent
                text: String(model.entry_name)
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    var lol = entry_name
                    if(!file_dialog_model.try_cd(entry_name)) {
                        file_dialog_model.try_choose_photo(lol)
                    }
                }
            }
        }
    }

}
