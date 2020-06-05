import QtQuick 2.14
import QtQuick.Controls 2.12

import "../buttons"
import "./settings_views"
import Settings_model_qml 1.0

Rectangle {
    id: root
    z: 0

    Component {
        id: change_avatar_comp
        Change_avatar_view {
            id: change_avatar
            color: root.color
        }
    }

    Component {
        id: change_password_comp
        Change_password_view {
            id: change_password_view
            color: root.color
        }
    }

    Back_btn {
        id: back_btn
        z: 1
        anchors {
            left: parent.left
            leftMargin: 5
            bottom: parent.bottom
            bottomMargin: 5
        }
        color: mouse_area.pressed ? "#708090" : parent.color
    }

    Settings_model {
        id: settings_model
    }

    ListView {
        id: settings_list_view
        z: 1
        anchors {
            top: parent.top
            topMargin: 5
            left: parent.left
            right: parent.right
            bottom: back_btn.top
            bottomMargin: anchors.topMargin
        }
        orientation: ListView.Vertical
        clip: true
        model: settings_model

        delegate: Rectangle {
            id: delegate
            height: 50
            width: parent.width
            color: mouse_area.pressed ? "#b22222" : root.color

            property int line_width: 1
            property string line_color: "#000000"
            Rectangle {
                id: bottom_line
                anchors {
                    bottom: parent.bottom
                }
                width: parent.width
                height: parent.line_width
                color: parent.line_color
            }
            Text {
                anchors.centerIn: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                width: parent.width
                height: parent.height - parent.line_width
                fontSizeMode: Text.Fit
                minimumPointSize: 5
                font.pointSize: 12
                elide: Text.ElideRight
                wrapMode: Text.WordWrap
                text: model.settings_name
                property int settings_number: model.settings_number
            }
            MouseArea {
                id: mouse_area
                anchors.fill: parent
                onClicked: {
                    switch(settings_number) {
                    case 0: {
                        break;
                    }
                    case 1: {
                        stack_view.push(change_password_comp)
                        break;
                    }
                    case 2: {
                        stack_view.push(change_avatar_comp)
                        break;
                    }
                    }
                }
            }
        }
    }
}
