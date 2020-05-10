import QtQuick 2.14
import QtQuick.Controls 2.12

import "buttons"
import "./Create_dialog.js" as Create_dialog

Rectangle {
    id: root
    z: 0

    Component.onCompleted: {
        Create_dialog.create_dialog(root, 1, "Please wait", Animation.Infinite, true, false)
    }

    Component {
        id: add_contact_menu_comp
        Add_contact_menu {}
    }

    Connections {
        target: client
        ignoreUnknownSignals: true
        onInfo: {
            Create_dialog.create_dialog(root, 1, info_message, 2000, false, is_static)
        }
        onSuccess_contact_deletion: {
            Create_dialog.create_dialog(root, 1, "Contact was deleted.", 2000, false, false)
        }
    }

    Back_btn {
        id: back_btn
        z: 0
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
        z: 0

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

    ListView {
        id: contacts_list_view
        z: 0
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
                    contacts_list_view.currentIndex = index
                    if(model.is_registered) {
                        var comp = Qt.createComponent("Contacts_list.qml");
                        var obj = comp.createObject(root, { nickname: nickname.text } );
                        stack_view.push(obj);
                    }
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
                                if(client.remove_contact(5, contacts_list_view.currentItem.nickname.text,
                                                         contacts_list_view.currentItem.time.text, index))
                                {
                                    Create_dialog.create_dialog(root, 1, "Please wait.", Animation.Infinite, true, false)
                                }
                            }
                        } else {
                            if(client.is_connected) {
                                if(client.remove_contact(4, contacts_list_view.currentItem.nickname.text,
                                                                   contacts_list_view.currentItem.time.text, index))
                                {
                                    Create_dialog.create_dialog(root, 1, "Please wait.", Animation.Infinite, true, false)
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
