import QtQuick 2.14
import QtQuick.Controls 2.12

Rectangle {

    Component {
        id: account_view_comp
        Account_view {}
    }
    Component {
        id: my_contacts_view_comp
        My_contacts_view {}
    }

    Column {
        anchors.centerIn: parent
        width: 150
        height: 30
        spacing: 10
        Rectangle {
            width: parent.width
            height: parent.height
            Text {
                anchors.centerIn: parent
                text: "My account"
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    stack_view.push(account_view_comp)
                }
            }
        }
        Rectangle {
            width: parent.width
            height: parent.height
            Text {
                anchors.centerIn: parent
                text: "My contacts"
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    stack_view.push(my_contacts_view_comp)
                }
            }
        }
        Rectangle {
            width: parent.width
            height: parent.height
            Text {
                anchors.centerIn: parent
                text: "Exit"
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                }
            }
        }
    }
}
