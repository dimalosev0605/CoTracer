import QtQuick 2.14
import QtQuick.Controls 2.12

import "buttons"

Rectangle {
    id: root
    color: "#e00d0d"
    z: 0

    property string date

    Connections {
        target: client
    }

    Back_btn {
        id: back_btn
        z: 0
        anchors {
            bottom: parent.bottom
            left: parent.left
            bottomMargin: 10
            leftMargin: 10
        }
        color: mouse_area.pressed ? "#708090" : parent.color
        mouse_area.onClicked: {
            // poped define in Back_btn.qml
        }
    }

    TextField {
        id: nickname_field
        z: 0
        y: parent.height * 0.25
        anchors {
            horizontalCenter: parent.horizontalCenter
        }
        width: parent.width > 300 ? 240 : parent.width * 0.8
        height: 30
        placeholderText: "Nickname"
    }

    Rectangle {
        id: time_rect
        z: 0
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
                horizontalAlignment: Text.AlignHCenter
                width: parent.width * 0.7
                height: parent.height
                fontSizeMode: Text.Fit
                minimumPointSize: 5
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
    Rectangle {
        id: is_reg_rect
        z: 0
        color: root.color
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: time_rect.bottom
            topMargin: 10
        }
        width: nickname_field.width
        height: 30
        Row {
            id: row
            anchors.fill: parent
            Text {
                id: qstn
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                width: parent.width * 0.5
                height: parent.height
                fontSizeMode: Text.Fit
                minimumPointSize: 5
                font.pointSize: 15
                elide: Text.ElideRight
                wrapMode: Text.NoWrap
                text: "Is registered?"
                property bool is_reg: false
            }
            Rectangle {
                id: yes_rect
                height: parent.height
                width: (parent.width - qstn.width) / 2
                radius: 3
                color: qstn.is_reg ? "#00ff00" : root.color
                Text {
                    anchors.centerIn: parent
                    text: "Yes"
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        qstn.is_reg = true
                    }
                }
            }
            Rectangle {
                id: no_rect
                height: parent.height
                width: yes_rect.width
                radius: 3
                color: !qstn.is_reg ? "#00ff00" : root.color
                Text {
                    anchors.centerIn: parent
                    text: "No"
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        qstn.is_reg = false
                    }
                }
            }
        }
    }

    Authorization_button {
        id: add_contact
        z: 0
        anchors {
            top: is_reg_rect.bottom
            topMargin: 20
            horizontalCenter: parent.horizontalCenter
        }
        width: nickname_field.width * 0.8
        height: 30
        radius: 5
        color: mouse_area.pressed ? "#af1111" : "#b22222"

        text.text: "Add contact"
        mouse_area.onClicked: {
            if(nickname_field.text === "") return;
            if(qstn.is_reg) {
                client.add_contact(2, nickname_field.text, hours.currentItem.text + ":" + minutes.currentItem.text, root.date)
            }
            else {
                client.add_contact(3, nickname_field.text, hours.currentItem.text + ":" + minutes.currentItem.text, root.date)
            }
        }
    }
}
