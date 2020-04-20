import QtQuick 2.14
import QtQuick.Controls 2.12

import "buttons"

Rectangle {
    id: root
    color: "#e00d0d"

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
                        client.remove_contact(7, contacts_list_view.currentItem.nickname.text,
                                                         contacts_list_view.currentItem.time.text)
                    } else {
                        client.remove_contact(6, contacts_list_view.currentItem.nickname.text,
                                                           contacts_list_view.currentItem.time.text)
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
                    }
                }
            }
        }

//        delegate: Rectangle {
//            color: "red"
//            width: parent.width
//            height: 40
//            property alias nickname: nickname
//            property alias time: time
//            Text {
//                id: nickname
//                anchors.centerIn: parent
//                text: String(model.nickname)
//                MouseArea {
//                    anchors.fill: parent
//                    onClicked: {
//                        if(model.is_registered) {
//                            var comp = Qt.createComponent("Contacts_list.qml")
//                            var obj = comp.createObject(root, {nickname: nickname.text})
//                            stack_view.push(obj)
//                        }
//                    }
//                }
//            }
//            Text {
//                id: time
//                anchors.left: nickname.right
//                text: String(model.time)
//            }
//            Rectangle {
//                anchors.right: parent.right
//                width: 40
//                height: 40
//                color: model.is_registered ? "green" : "blue"
//                MouseArea {
//                    anchors.fill: parent
//                    onClicked: {
//                        contacts_list_view.currentIndex = index
//                        if(model.is_registered) {
//                            console.log("remove registered qml")
//                            client.remove_contact(7, contacts_list_view.currentItem.nickname.text,
//                                                             contacts_list_view.currentItem.time.text)
//                        } else {
//                            console.log("remove unregistered qml")
//                            client.remove_contact(6, contacts_list_view.currentItem.nickname.text,
//                                                               contacts_list_view.currentItem.time.text)
//                        }
//                    }
//                }
//            }
//        }
    }
}
