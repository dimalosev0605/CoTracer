import QtQuick 2.14
import QtQuick.Controls 2.12
import QtQuick.Window 2.14

import "../buttons"

Rectangle {
    id: root
    color: "#e00d0d"
    z: 0

    property string date

    MouseArea {
        id: root_m_area
        z: 1
        anchors.fill: parent
        onClicked: {
            Qt.inputMethod.hide()
            nickname_field.focus = false
            stack_view.focus = true
        }
    }

    Connections {
        id: keyboard_connections
        target: Qt.inputMethod
        onKeyboardRectangleChanged: {
            if(Qt.inputMethod.keyboardRectangle.height !== 0) {
                nickname_field.y = root.height * 0.2
                back_btn.y = Qt.inputMethod.keyboardRectangle.y / Screen.devicePixelRatio
                - back_btn.height - back_btn.bottom_margin
            }
            else {
                nickname_field.y = root.height * 0.35
                back_btn.y = root.height - back_btn.height - back_btn.bottom_margin
            }
        }
    }

    Back_btn {
        id: back_btn
        z: 1
        y: root.height - height - bottom_margin
        property int bottom_margin: 10
        anchors {
            left: parent.left
            leftMargin: 10
        }
        color: mouse_area.pressed ? "#708090" : parent.color
    }

    TextField {
        id: nickname_field
        z: 1
        y: parent.height * 0.35
        anchors {
            horizontalCenter: parent.horizontalCenter
        }
        width: parent.width * 0.7
        height: 30
        placeholderText: "Enter a nickname"
        cursorVisible: activeFocus
        inputMethodHints: Qt.ImhNoPredictiveText
        font.pointSize: height * 0.4
        Keys.onPressed: {
            if(event.key === Qt.Key_Return) {
                event.accepted = true
                nickname_field.focus = false
                stack_view.focus = true
            }
        }
    }

    Rectangle {
        id: time_rect
        z: 1
        color: root.color
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: nickname_field.bottom
            topMargin: 10
        }
        height: 90
        width: nickname_field.width
        Row {
            anchors.fill: parent
            Text {
                id: time
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                width: parent.width * 0.6
                height: parent.height
                fontSizeMode: Text.Fit
                minimumPointSize: 1
                font.pointSize: 15
                elide: Text.ElideRight
                wrapMode: Text.NoWrap
                text: "Contact time:"
            }
            Tumbler {
                id: hours
                function create_hour_arr() {
                    var array = ["00","01","02","03","04","05","06","07","08", "09"];
                    for(var i = 10; i < 24; ++i) {
                        array.push(i.toString());
                    }
                    return array;
                }
                model: create_hour_arr()
                width: (parent.width - time.width) / 2
                height: parent.height
                function get_curr_hour() {
                    var curr_hour = Qt.formatTime(new Date(), "hh");
                    return curr_hour;
                }
                visibleItemCount: 3
                currentIndex: get_curr_hour()
                delegate: Text {
                    opacity: 1.0 - Math.abs(Tumbler.displacement) / (Tumbler.tumbler.visibleItemCount / 2)
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: 10
                    text: modelData
                }
            }
            Tumbler {
                id: minutes
                function create_min_arr() {
                    var array = ["00","01","02","03","04","05","06","07","08", "09"];
                    for(var i = 10; i < 60; ++i) {
                        array.push(i.toString());
                    }
                    return array;
                }
                model: create_min_arr()
                width: hours.width
                height: hours.height
                function get_curr_min() {
                    var curr_min = Qt.formatTime(new Date(), "mm");
                    return curr_min;
                }
                currentIndex: get_curr_min()
                visibleItemCount: 3
                delegate: Text {
                    opacity: 1.0 - Math.abs(Tumbler.displacement) / (Tumbler.tumbler.visibleItemCount / 2)
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: 10
                    text: modelData
                }
            }
        }
    }
    Authorization_button {
        id: add_contact
        z: 1
        anchors {
            top: time_rect.bottom
            topMargin: 10
            horizontalCenter: parent.horizontalCenter
        }
        width: nickname_field.width * 0.8
        height: 40
        radius: 5
        color: mouse_area.pressed ? "#af1111" : "#b22222"

        text.text: "Add contact"
        mouse_area.onClicked: {
            nickname_field.focus = false
            if(nickname_field.text === "") return;
            client.add_contact(nickname_field.text, hours.currentItem.text + ":" + minutes.currentItem.text, root.date)
            stack_view.focus = true
            Qt.inputMethod.hide()
        }
    }
}
