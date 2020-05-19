import QtQuick 2.14
import QtQuick.Controls 2.12

import "../buttons"
import File_dialog_model_qml 1.0

Rectangle {
    id: root
    z: 0

    Row {
        id: btns
        anchors {
            left: parent.left
            leftMargin: 5
            right: parent.right
            rightMargin: 5
            bottom: parent.bottom
            bottomMargin: 5
        }
        height: 35
        spacing: 5
        leftPadding: 5
        rightPadding: leftPadding

        Return_back_btn {
            id: return_back_btn
            height: parent.height
            width: height
            color: mouse_area.pressed ? "#b22222" : root.color
            mouse_area.onClicked: {
                file_dialog_model.return_back()
            }
        }

        Main_menu_btn {
            id: cancel_btn
            height: parent.height
            width: (parent.width - return_back_btn.width - parent.spacing * 2 - parent.leftPadding - parent.rightPadding) / 2
            color: mouse_area.pressed ? "#b22222" : root.color
            text.text: "Cancel"
            mouse_area.onClicked: {
                stack_view.pop()
            }
        }

        Main_menu_btn {
            id: update_btn
            height: parent.height
            width: cancel_btn.width
            color: mouse_area.pressed ? "#b22222" : root.color
            text.text: "Refresh"
            mouse_area.onClicked: {
                file_dialog_model.refresh()
            }
        }
    }

    File_dialog_model {
        id: file_dialog_model
        onSelected: {
            console.log("path: " + path)
            avatar.source = path
            save_changes_btn.visible = true
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
            bottom: btns.top
            bottomMargin: 5
        }
        orientation: ListView.Vertical
        clip: true
        spacing: 5
        model: file_dialog_model
        delegate: Rectangle {
            width: parent.width
            height: 40
            color: mouse_area.pressed ? "#b22222" : root.color
            border.width: 1
            border.color: "#000000"
            radius: 1
            Text {
                id: text
                anchors.centerIn: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                width: parent.width - img.width - img.anchors.rightMargin
                height: parent.height
                fontSizeMode: Text.Fit
                minimumPointSize: 5
                font.pointSize: 12
                elide: Text.ElideRight
                wrapMode: Text.WordWrap
                text: String(model.entry_name)
            }
            Image {
                id: img
                anchors {
                    right: parent.right
                    rightMargin: 5
                    verticalCenter: parent.verticalCenter
                }
                height: parent.height * 0.7
                width: height
                source: model.is_dir_or_file ? "qrc:/imgs/folder.png" : "qrc:/imgs/up_arrow_in_circle.png"
            }
            MouseArea {
                id: mouse_area
                anchors.fill: parent
                onClicked: {
                    var lol = entry_name
                    if(!file_dialog_model.try_cd(entry_name)) {
                        file_dialog_model.try_choose_avatar(lol)
                    }
                }
            }
        }
    }

}
