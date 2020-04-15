import QtQuick 2.14
import QtQuick.Controls 2.12

import ".."
import Days_model_qml 1.0
import Communication_tcp_client_qml 1.0

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
    Communication_tcp_client {
        id: client
        onConnection_error: {

        }
        onSuccess_connection: {

        }
    }

    Component {
        id: my_contacts_list_comp
        My_contacts_list {}
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
            id: m_delegate
            color: "red"
            width: parent.width
            height: 40
            property string text: text.text
            Text {
                id: text
                anchors.centerIn: parent
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
