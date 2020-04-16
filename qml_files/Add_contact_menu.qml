import QtQuick 2.14
import QtQuick.Controls 2.12

Item {
    Rectangle {
        width: 50
        height: 50
        color: "red"
        MouseArea {
            anchors.fill: parent
            onClicked: {
                stack_view.pop()
            }
        }
    }

    TextField {
        id: nickname_field
        anchors.centerIn: parent
        width: parent.width - 100
        height: 30
    }
    TextField {
        id: time
        anchors.top: nickname_field.bottom
        anchors.left: nickname_field.left
        width: nickname_field.width
        height: 30
    }

    Rectangle {
        id: registered
        anchors.top: time.bottom
        anchors.left: time.left
        color: "red"
        width: 50
        height: 50
        Text {
            anchors.centerIn: parent
            text:  "Add reg contact"
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                client.add_contact(2, nickname_field.text, time.text)
            }
        }
    }
    Rectangle {
        id: not_registered
        anchors.top: time.bottom
        anchors.right: time.right
        color: "green"
        width: 50
        height: 50
        Text {
            anchors.centerIn: parent
            text:  "Add unreg contact"
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                client.add_contact(3, nickname_field.text, time.text)
            }
        }
    }
}
