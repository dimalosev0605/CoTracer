import QtQuick 2.14
import QtQuick.Controls 2.12
import QtQuick.Window 2.14

import "../buttons"
import Client_qml 1.0

Rectangle {
    id: root
    z: 0
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
        id: my_stat_view_comp
        My_stat_view {
            color: root.color
        }
    }

    Client {
        id: client
    }

    Text {
        id: app_label
        z: 1
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
        z: 1
        anchors {
            top: app_label.bottom
            topMargin: 10
            horizontalCenter: parent.horizontalCenter
        }
        width:  parent.width * 0.5
        height: width
        source: "qrc:/imgs/biohazard_symbol.png"
    }

    Column {
        id: btns_col
        z: 1
        y: root.height / 2
        anchors {
            horizontalCenter: parent.horizontalCenter
        }
        width: parent.width * 0.5
        spacing: 10
        property color btns_pressed_color: "#b22222"
        property int btns_height: 40
        property int btns_radius: 5
        Main_menu_btn {
            id: my_account_btn
            width: btns_col.width
            height: btns_col.btns_height
            color: mouse_area.pressed ? parent.btns_pressed_color : root.color
            radius: btns_col.btns_radius
            text.text: "My account"
            mouse_area.onClicked: {
                stack_view.push(account_view_comp)
            }
        }

        Main_menu_btn {
            id: my_friends_btn
            width: btns_col.width
            height: btns_col.btns_height
            color: mouse_area.pressed ? parent.btns_pressed_color : root.color
            radius: btns_col.btns_radius
            text.text: "My friends"
            mouse_area.onClicked: {
                stack_view.push(my_friends_view_comp)
            }
        }

        Main_menu_btn {
            id: my_contacts_btn
            width: btns_col.width
            height: btns_col.btns_height
            color: mouse_area.pressed ? parent.btns_pressed_color : root.color
            radius: btns_col.btns_radius
            text.text: "My statistic"
            mouse_area.onClicked: {
                stack_view.push(my_stat_view_comp)
            }
        }

        Main_menu_btn {
            id: exit_btn
            width: btns_col.width
            height: btns_col.btns_height
            color: mouse_area.pressed ? parent.btns_pressed_color : root.color
            radius: btns_col.btns_radius
            text.text: "Exit"
            mouse_area.onClicked: {
                Qt.quit()
            }
        }
    }
}
