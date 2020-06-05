import QtQuick 2.14
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0

import "../buttons"
import File_dialog_model_qml 1.0

Rectangle {
    id: root
    z: 0

    File_dialog_model {
        id: file_dialog_model
        onSelected: {
            avatar.source = path
            save_changes_btn.visible = true
            stack_view.pop()
        }
        onCurrent_path_changed: {
            current_path_text.text = path
        }
    }

    Text {
        id: current_path_text
        z: 1
        anchors {
            left: parent.left
            leftMargin: 3
            right: more_btn.left
            rightMargin: anchors.leftMargin
            top: parent.top
            topMargin: more_btn.anchors.topMargin
        }
        height: more_btn.height
        width: parent.width - anchors.leftMargin - anchors.rightMargin -
               more_btn.anchors.rightMargin - more_btn.width
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        fontSizeMode: Text.Fit
        minimumPointSize: 1
        font.pointSize: 12
        elide: Text.ElideRight
        wrapMode: Text.WordWrap
        text: file_dialog_model.get_current_path()
    }

    TextField {
        id: text_input_for_search
        z: 1
        anchors {
            left: parent.left
            leftMargin: 3
            right: parent.right
            rightMargin: 3
            top: parent.top
            topMargin: 3
        }
        Image {
            id: cancel_search_img
            anchors {
                right: parent.right
                rightMargin: 2
                verticalCenter: parent.verticalCenter
            }
            height: parent.height - 4
            width: height
            source: "qrc:/imgs/remove.png"
            fillMode: Image.PreserveAspectFit
            MouseArea {
                anchors.centerIn: parent
                width: parent.width + 3
                height: parent.height + 3
                onClicked: {
                    text_input_for_search.text = ""
                    file_dialog_model.cancel_search()
                    text_input_for_search.visible = false
                    current_path_text.visible = true
                    more_btn.visible = true
                }
            }
        }
        height: current_path_text.height
        width: current_path_text.width
        rightPadding: cancel_search_img.width + cancel_search_img.anchors.rightMargin * 2
        visible: false
        inputMethodHints: Qt.ImhNoPredictiveText
        placeholderText: "Search files in this directory"
        background: Rectangle {
            width: parent.width
            height: parent.height
            color: root.color
            border.width: 1
            border.color: "#000000"
        }
        onTextChanged: {
            file_dialog_model.search(text)
        }
    }

    More_btn {
        id: more_btn
        z: 1
        anchors {
            top: parent.top
            topMargin: 3
            right: parent.right
            rightMargin: 3
        }
        height: 30
        width: height
        color: mouse_area.pressed ? "#b22222" : root.color
        radius: 5
        mouse_area.onClicked: {
            popup_menu.open()
        }
        Menu {
            id: popup_menu
            property int count_of_items: 4
            property int item_height: 35
            width: 150
            height: count_of_items * item_height

            Action { text: qsTr("Search");
                onTriggered: {
                    file_dialog_model.prepare_search()
                    current_path_text.visible = false
                    more_btn.visible = false
                    text_input_for_search.visible = true
                }
            }
            Action { text: qsTr("Refresh"); onTriggered: { file_dialog_model.refresh() } }
            Action { text: qsTr("Sort by time"); onTriggered: { file_dialog_model.sort_by_time()} }
            Action { text: qsTr("Sort by name"); onTriggered: { file_dialog_model.sort_by_name()} }

            background: Rectangle {
                width: parent.width
                height: parent.height
                color: root.color
            }

            delegate: MenuItem {
                id: menu_item
                width: popup_menu.width
                height: popup_menu.item_height
                background: Rectangle {
                    width: parent.width
                    height: parent.height - border.height
                    color: menu_item.pressed ? "#b22222" : root.color
                    Rectangle {
                        id: border
                        anchors {
                            top: parent.bottom
                            horizontalCenter: parent.horizontalCenter
                        }
                        width: parent.width * 0.8
                        height: 1
                        color: "#000000"
                    }
                }
                contentItem: Text {
                    height: parent.height
                    width: parent.width
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    fontSizeMode: Text.Fit
                    minimumPointSize: 1
                    font.pointSize: 12
                    elide: Text.ElideRight
                    wrapMode: Text.WordWrap
                    text: menu_item.text
                }
            }
        }
    }

    ListView {
        id: files_list_view
        z: 1
        anchors {
            top: current_path_text.bottom
            topMargin: 10
            left: parent.left
            leftMargin: 5
            right: parent.right
            rightMargin: anchors.leftMargin
            bottom: return_back_btn.top
            bottomMargin: 5
        }
        orientation: ListView.Vertical
        clip: true
        spacing: 5
        model: file_dialog_model
        delegate: Rectangle {
            id: delegate
            height: 50
            width: parent.width
            color: mouse_area.pressed ? "#b22222" : root.color
            property int line_width: 1
            property string line_color: "#000000"

            Rectangle {
                id: top_line
                anchors {
                    top: parent.top
                }
                width: parent.width
                height: parent.line_width
                color: parent.line_color
            }
            Rectangle {
                id: bottom_line
                anchors {
                    bottom: parent.bottom
                }
                width: parent.width
                height: parent.line_width
                color: parent.line_color
            }
            Image {
                id: img
                anchors {
                    left: parent.left
                    leftMargin: 3
                    verticalCenter: parent.verticalCenter
                }
                height: parent.height - 2 * parent.line_width - 4
                width: height
                source: model.path_to_img
                fillMode: Image.PreserveAspectCrop
                layer.enabled: true
                layer.effect: OpacityMask {
                    maskSource: Rectangle {
                        width: img.width
                        height: img.height
                        radius: 5
                    }
                }
            }
            Column {
                id: texts_column
                anchors {
                    left: img.right
                    leftMargin: 5
                }
                width: parent.width - img.width - img.anchors.leftMargin -
                       anchors.leftMargin
                height: parent.height - 2 * parent.line_width
                Text {
                    id: file_name
                    height: parent.height / 2
                    width: parent.width
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    fontSizeMode: Text.Fit
                    minimumPointSize: 1
                    font.pointSize: 12
                    elide: Text.ElideRight
                    wrapMode: Text.WordWrap
                    text: String(model.name)
                }
                Text {
                    id: last_modified_time
                    height: parent.height / 2
                    width: parent.width
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    fontSizeMode: Text.Fit
                    minimumPointSize: 1
                    font.pointSize: 12
                    elide: Text.ElideRight
                    wrapMode: Text.WordWrap
                    text: String(model.last_modified_time)
                }
            }
            MouseArea {
                id: mouse_area
                anchors.fill: parent
                onClicked: {
                    if(!file_dialog_model.cd(file_name.text)) {
                        file_dialog_model.choose_avatar(file_name.text)
                    }
                }
            }
        }
    }

    Main_menu_btn {
        id: return_back_btn
        z: 1
        anchors {
            left: parent.left
            leftMargin: 5
            bottom: parent.bottom
            bottomMargin: 5
        }
        height: 35
        property int space: 5
        width: (parent.width - anchors.leftMargin - anchors.rightMargin - space) / 2
        color: mouse_area.pressed ? "#b22222" : root.color
        text.text: "Back"
        radius: 2
        mouse_area.onClicked: {
            text_input_for_search.text = ""
            file_dialog_model.cancel_search()
            text_input_for_search.visible = false
            current_path_text.visible = true
            more_btn.visible = true
            file_dialog_model.return_in_parent_directory()
        }
    }

    Main_menu_btn {
        id: cancel_btn
        z: 1
        anchors {
            right: parent.right
            rightMargin: return_back_btn.anchors.leftMargin
            bottom: parent.bottom
            bottomMargin: return_back_btn.anchors.bottomMargin
        }
        height: return_back_btn.height
        width: return_back_btn.width
        color: mouse_area.pressed ? "#b22222" : root.color
        text.text: "Cancel"
        radius: 2
        mouse_area.onClicked: {
            stack_view.pop()
        }
    }
}
