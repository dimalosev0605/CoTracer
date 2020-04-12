import QtQuick 2.14
import QtQuick.Controls 2.12

Rectangle {
    Rectangle {
        width: 50
        height: 50
        color: "red"
        MouseArea {
            anchors.fill: parent
            onClicked: {
                days_model.destroy_model()
                stack_view.pop()
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
        model: days_model.create_model()
        delegate: Rectangle {
            color: "red"
            width: parent.width
            height: 40
        }
    }
}
