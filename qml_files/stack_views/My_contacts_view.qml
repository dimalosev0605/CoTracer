import QtQuick 2.14
import QtQuick.Controls 2.12

import ".."
import Days_model_qml 1.0

Rectangle {
    Rectangle {
        width: 50
        height: 50
        color: "red"
        MouseArea {
            anchors.fill: parent
            onClicked: stack_view.pop()
        }
    }

    Days_model {
        id: days_model
    }

    Component {
        id: contacts_list_comp
        Contacts_list {}
    }

    ListView {
        id: days_list_view
        anchors.centerIn: parent
        width: parent.width
        height: parent.height - 100

        orientation: ListView.Vertical
        clip: true
        spacing: 5
        model: days_model
        delegate: Rectangle {
            color: "red"
            width: parent.width
            height: 40
            Text {
                anchors.centerIn: parent
                text: String(model.date)
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    stack_view.push(contacts_list_comp)
                }
            }
        }
    }
}
