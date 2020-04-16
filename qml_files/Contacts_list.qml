import QtQuick 2.14
import QtQuick.Controls 2.12

Rectangle {

    id: root
    property string nickname

    Rectangle {
        width: 50
        height: 50
        color: "red"
        MouseArea {
            anchors.fill: parent
            onClicked: {
                stack_view.pop()
                client.destroy_model()
                root.destroy()
            }
        }
    }

    ListView {
        id: list_view
        anchors.centerIn: parent
        width: parent.width
        height: parent.height - 100

        orientation: ListView.Vertical
        clip: true
        spacing: 5

        model: client.create_model_based_on_nickname(nickname)
        delegate: Rectangle {
            color: model.is_registered ? "green" : "red"
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
                        list_view.currentIndex = index
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
        }
    }

}
