import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Controls 2.12

import "./stack_views"

Window {
    visible: true
    width: 392
    height: 761
    StackView {
        id: stack_view
        anchors.fill: parent
        initialItem: Main_menu_view {}
    }
}
