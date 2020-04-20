import QtQuick 2.14
import QtQuick.Controls 2.12

import ".."
import "../buttons"
import Days_model_qml 1.0
import Communication_tcp_client_qml 1.0

Rectangle {
    z: 0
    color: "#e00d0d"

    Days_model {
        id: days_model
    }

    Component {
        id: my_contacts_list_comp
        My_contacts_list {
            id: my_contacts_list
        }
    }

    Communication_tcp_client {
        id: client
        onSuccess_connection: {
            my_dialog.visible = false
        }
        onConnection_error: {
            my_dialog.busy_indicator.running = false
            my_dialog.text.text = "Connection error."
            my_dialog.visible = true
        }
        onUndefined_error: {
            my_dialog.busy_indicator.running = false
            my_dialog.text.text = "Error occured. Try later."
            sign_in_btn.enabled = false
            sign_up_btn.enabled = false
            my_dialog.visible = true
        }
        onSuccess_unregister_contact_deletion: {
            my_contacts_list.my_dialog.text.text = "Success deletion!"
            my_contacts_list.my_dialog.busy_indicator.running = false
            my_contacts_list.my_dialog.visible = true
            my_contacts_list.my_dialog.opacity_anim.start()
        }
        onSuccess_register_contact_deletion: {
            my_contacts_list.my_dialog.text.text = "Success deletion!"
            my_contacts_list.my_dialog.busy_indicator.running = false
            my_contacts_list.my_dialog.visible = true
            my_contacts_list.my_dialog.opacity_anim.start()
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

        visible: true
        opacity: 0.7
    }

    Back_btn {
        id: back_btn
        z: 1
        anchors {
            bottom: parent.bottom
            left: parent.left
            bottomMargin: 10
            leftMargin: 10
        }
        color: mouse_area.pressed ? "#708090" : parent.color
    }

    ListView {
        id: days_list_view
        z: 1

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
        model: days_model

        delegate: Rectangle {
            id: delegate
            z: 2

            width: parent.width
            height: 50

            border.width: 1
            border.color: "#000000"
            color: "#d70707"

            property string date: text.text
            Text {
                id: text
                anchors.centerIn: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                width: parent.width
                height: parent.height
                fontSizeMode: Text.Fit
                minimumPointSize: 5
                font.pointSize: 12
                elide: Text.ElideRight
                wrapMode: Text.WordWrap
                text: String(model.date)
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    days_list_view.currentIndex = index
                    stack_view.push(my_contacts_list_comp)
                }
            }
        }
    }
}
