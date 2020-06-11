import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Controls 2.12

import "./stack_views"

Window {
    id: window
    visible: true
    width: 392
    height: 761

    Screen.orientationUpdateMask: Qt.PrimaryOrientation | Qt.LandscapeOrientation |
                                  Qt.PortraitOrientation | Qt.InvertedLandscapeOrientation |
                                  Qt.InvertedPortraitOrientation

    Component {
        id: rotation_message_view_comp
        Rotation_message_view {}
    }

    Connections {
        target: window
        property int counter: 0
        Screen.onPrimaryOrientationChanged: {
            if(Screen.orientation !== Qt.PortraitOrientation) {
                if(stack_view.currentItem.objectName === "Chart_view") {
                    return
                }
                stack_view.push(rotation_message_view_comp);
                ++counter;
            }
            else {
                while(counter !== 0) {
                    stack_view.pop();
                    --counter;
                }
            }
        }
    }

    StackView {
        id: stack_view
        anchors.fill: parent
        initialItem: Main_menu_view {}

        focus: true
        Keys.onPressed: {
            if(event.key === Qt.Key_Back) {
                event.accepted = true
                if(Screen.orientation !== Qt.PortraitOrientation) return
                if(stack_view.depth > 1) {
                    stack_view.pop()
                }
            }
        }
    }
}
