import QtQuick 2.14
import QtQuick.Controls 2.12

Rectangle {
    id: root

    Rectangle {
        width: 50
        height: 50
        color: "red"
        MouseArea {
            anchors.fill: parent
            onClicked: {
                stack_view.pop()
                client.destroy_model()
            }
        }
    }

    Component {
        id: add_contact_menu_comp
        Add_contact_menu {}
    }

    Rectangle {
        width: 50
        height: 50
        color: "green"
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        MouseArea {
            anchors.fill: parent
            onClicked: {
                stack_view.push(add_contact_menu_comp)
            }
        }
    }

    ListView {
        id: contacts_list_view
        anchors.centerIn: parent
        width: parent.width
        height: parent.height - 100

        orientation: ListView.Vertical
        clip: true
        spacing: 5
        model: client.create_main_model(days_list_view.currentItem.text)
        delegate: Rectangle {
            color: "red"
            width: parent.width
            height: 40
            property alias nickname: nickname
            property alias time: time
            Text {
                id: nickname
                anchors.centerIn: parent
                text: String(model.nickname)
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if(model.is_registered) {
                            var comp = Qt.createComponent("Contacts_list.qml")
                            var obj = comp.createObject(root, {nickname: nickname.text})
                            stack_view.push(obj)
                        }
                    }
                }
            }
            Text {
                id: time
                anchors.left: nickname.right
                text: String(model.time)
            }
            Rectangle {
                anchors.right: parent.right
                width: 40
                height: 40
                color: model.is_registered ? "green" : "blue"
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        contacts_list_view.currentIndex = index
                        if(model.is_registered) {
                            console.log("remove registered qml")
                            client.remove_registered_contact(contacts_list_view.currentItem.nickname.text,
                                                             contacts_list_view.currentItem.time.text)
                        } else {
                            console.log("remove unregistered qml")
                            client.remove_unregistered_contact(contacts_list_view.currentItem.nickname.text,
                                                               contacts_list_view.currentItem.time.text)
                        }
                    }
                }
            }
        }
    }
}
