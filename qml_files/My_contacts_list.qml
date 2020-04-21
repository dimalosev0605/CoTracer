import QtQuick 2.14
import QtQuick.Controls 2.12

import "buttons"

Rectangle {
    id: root
    color: "#e00d0d"

    property alias my_dialog: my_dialog

    Back_btn {
        id: back_btn
//        z:
        anchors {
            bottom: parent.bottom
            left: parent.left
            bottomMargin: 10
            leftMargin: 10
        }
        color: mouse_area.pressed ? "#708090" : parent.color
        mouse_area.onClicked: {
            // poped define in Back_btn.qml
            client.destroy_model()
        }
    }
    Add_contact_btn {
        id: add_contact_btn

        width: back_btn.width
        height: width

        anchors {
            right: parent.right
            rightMargin: 10
            bottom: parent.bottom
            bottomMargin: 10
        }
        color: mouse_area.pressed ? "#00ff00" : parent.color
        mouse_area.onClicked: {
            stack_view.push(add_contact_menu_comp)
        }
    }
    My_dialog {
        id: my_dialog
        z: 3
        anchors.centerIn: parent
        width: if(parent.height > parent.width) {
                   parent.width * 0.9
               }
               else {
                   parent.width * 0.5
               }

        height: if(parent.height > parent.width) {
                    parent.height * 0.2
                }
                else {
                    parent.height * 0.5
                }

        visible: false
        opacity: 0.7
    }
    Connections {
        target: client
        onUndefined_error: {
            my_dialog.busy_indicator.running = false
            my_dialog.text.text = "Error occured. Try later."
            my_dialog.visible = true
        }
        onSuccess_unregister_contact_deletion: {
            my_dialog.text.text = "Success deletion!"
            my_dialog.busy_indicator.running = false
            my_dialog.visible = true
            my_dialog.opacity_anim.start()
        }
        onSuccess_register_contact_deletion: {
            my_dialog.text.text = "Success deletion!"
            my_dialog.busy_indicator.running = false
            my_dialog.visible = true
            my_dialog.opacity_anim.start()
        }
        onInternal_server_error: {
            my_dialog.text.text = "Internal server errror. Try later."
            my_dialog.busy_indicator.running = false
            my_dialog.visible = true
        }
    }

    Component {
        id: add_contact_menu_comp
        Add_contact_menu {}
    }

    ListView {
        id: contacts_list_view
        anchors {
            top: parent.top
            topMargin: 10
            left: parent.left
            leftMargin: 5
            right: parent.right
            rightMargin: 5
            bottom: back_btn.top
            bottomMargin: 10
        }

        orientation: ListView.Vertical
        clip: true
        spacing: 5
        model: client.create_model_based_on_date(days_list_view.currentItem.date)
        delegate: Rectangle {
            id: delegate

            property alias nickname: nickname
            property alias time: time

            width: parent.width
            height: 40

            border.width: 1
            border.color: "#000000"
            color: model.is_registered ? "#ff0000" : "#d70707"

            Text {
                id: number
                anchors {
                    left: parent.left
                    top: parent.top
                }
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                height: parent.height
                width: height * 0.75
                fontSizeMode: Text.Fit
                minimumPointSize: 5
                font.pointSize: 12
                elide: Text.ElideRight
                wrapMode: Text.WordWrap
                text: index + 1
            }
            Text {
                id: nickname
                anchors {
                    left: number.right
                    verticalCenter: parent.verticalCenter
                }
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                height: parent.height
                width: (parent.width - number.width - remove_contact_rect.width) / 2
                fontSizeMode: Text.Fit
                minimumPointSize: 5
                font.pointSize: 12
                elide: Text.ElideRight
                wrapMode: Text.WordWrap
                text: String(model.nickname)
            }
            Text {
                id: time
                anchors {
                    left: nickname.right
                    top: nickname.top
                }
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                height: parent.height
                width: nickname.width
                fontSizeMode: Text.Fit
                minimumPointSize: 5
                font.pointSize: 12
                elide: Text.ElideRight
                wrapMode: Text.WordWrap
                text: String(model.time)
            }
            MouseArea {
                id: mouse_area
                anchors {
                    left: parent.left
                    right: remove_contact_rect.left
                }
                height: parent.height
                onClicked: {
                }
            }
            Rectangle {
                id: remove_contact_rect
                anchors {
                    verticalCenter: parent.verticalCenter
                    right: parent.right
                    rightMargin: delegate.border.width
                }
                height: parent.height - parent.border.width * 4
                width: height
                color: delegate.color
//                radius: height / 2
                Image {
                    id: img
                    width: parent.width
                    height: parent.height
                    source: "qrc:/imgs/remove.png"
                }
                MouseArea {
                    id: m_area
                    anchors.fill: parent
                    onClicked: {
                        contacts_list_view.currentIndex = index
                        if(model.is_registered) {
                            if(client.is_connected) {
                                if(client.remove_contact(7, contacts_list_view.currentItem.nickname.text,
                                                         contacts_list_view.currentItem.time.text, index))
                                {
                                    my_dialog.busy_indicator.running = true
                                    my_dialog.text.text = "Please wait"
                                    my_dialog.visible = true
                                }
                            }
                        } else {
                            if(client.is_connected) {
                                if(client.remove_contact(6, contacts_list_view.currentItem.nickname.text,
                                                                   contacts_list_view.currentItem.time.text, index))
                                {
                                    my_dialog.busy_indicator.running = true
                                    my_dialog.text.text = "Please wait"
                                    my_dialog.visible = true
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
