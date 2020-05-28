import QtQuick 2.14
import QtQuick.Controls 2.12

import "../buttons"
import Client_qml 1.0

Rectangle {
    id: root
    color: "#e00d0d"

    Component {
        id: account_view_comp
        Account_view {
            color: root.color
        }
    }

    Component {
        id: my_friends_view_comp
        My_friends_view {
            id: my_friends_view
            color: root.color
        }
    }

    Component {
        id: my_contacts_view_comp
        My_contacts_view {
            color: root.color
        }
    }

    Client {
        id: client
    }

    Text {
        id: app_label
        anchors {
            top: parent.top
            left: parent.left
        }
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        width: parent.width
        height: 40
        fontSizeMode: Text.Fit
        minimumPointSize: 1
        font.pointSize: 15
        elide: Text.ElideRight
        wrapMode: Text.WordWrap
        text: "CoTracer"
    }

    Image {
        id: bio_hazard_symbol_img
        anchors {
            top: app_label.bottom
            topMargin: 10
            horizontalCenter: parent.horizontalCenter
        }
        width: if(parent.height > parent.width) {
                   parent.width * 0.6
               }
               else {
                   parent.height * 0.4
               }
        height: width
        source: "qrc:/imgs/biohazard_symbol.png"
    }

    Column {
        id: btns_col

        anchors {
            top: bio_hazard_symbol_img.bottom
            topMargin: parent.height > parent.width ? 90 : 5
            bottom: parent.bottom
            bottomMargin: 5
            horizontalCenter: parent.horizontalCenter
        }
        width: parent.width * 0.9

        property int btns_count: 4
        property real btns_height:
            (height - spacing * (btns_count - 1)) / btns_count / 2
        property real btns_width: width
        property color btns_pressed_color: "#b22222"
        property real min_btns_height: 30
        property real max_btns_width: 250

        spacing: parent.height > parent.width ? 10 : 5

        Main_menu_btn {
            id: my_account_btn
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.btns_width > parent.max_btns_width ? parent.max_btns_width : parent.btns_width
            height: parent.btns_height < parent.min_btns_height ? parent.btns_height * 2 : parent.btns_height
            color: mouse_area.pressed ? parent.btns_pressed_color : root.color

            text.text: "My account"
            mouse_area.onClicked: {
                stack_view.push(account_view_comp)
            }
        }

        Main_menu_btn {
            id: my_friends_btn
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.btns_width > parent.max_btns_width ? parent.max_btns_width : parent.btns_width
            height: parent.btns_height < parent.min_btns_height ? parent.btns_height * 2 : parent.btns_height
            color: mouse_area.pressed ? parent.btns_pressed_color : root.color

            text.text: "My friends"
            mouse_area.onClicked: {
                stack_view.push(my_friends_view_comp)
            }
        }

        Main_menu_btn {
            id: my_contacts_btn
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.btns_width > parent.max_btns_width ? parent.max_btns_width : parent.btns_width
            height: parent.btns_height < parent.min_btns_height ? parent.btns_height * 2 : parent.btns_height
            color: mouse_area.pressed ? parent.btns_pressed_color : root.color

            text.text: "My contacts"
            mouse_area.onClicked: {
                stack_view.push(my_contacts_view_comp)
            }
        }

        Main_menu_btn {
            id: exit_btn
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.btns_width > parent.max_btns_width ? parent.max_btns_width : parent.btns_width
            height: parent.btns_height < parent.min_btns_height ? parent.btns_height * 2 : parent.btns_height
            color: mouse_area.pressed ? parent.btns_pressed_color : root.color

            text.text: "Exit"
            mouse_area.onClicked: {
                Qt.quit()
            }
        }
    }
}
