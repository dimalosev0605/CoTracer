import QtQuick 2.14
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0

import "buttons"

Rectangle {
    id: root
    color: "#e00d0d"

    property string nickname
    property string date

    Back_btn {
        id: back_btn
        anchors {
            bottom: parent.bottom
            left: parent.left
            bottomMargin: 10
            leftMargin: 10
        }
        color: mouse_area.pressed ? "#708090" : parent.color
        mouse_area.onClicked: {
            client.cancel_operation()
            client.pop_model()
            root.destroy()
        }
    }

    Text {
        id: date
        z: 0
        anchors {
            top: parent.top
            topMargin: 5
            horizontalCenter: parent.horizontalCenter

        }
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        width: parent.width - 10
        height: 30
        fontSizeMode: Text.Fit
        minimumPointSize: 5
        font.pointSize: 12
        elide: Text.ElideRight
        wrapMode: Text.WordWrap
        text: root.date
    }

    ListView {
        id: contacts_list_view
        anchors {
            top: date.bottom
            topMargin: 10
            left: parent.left
            leftMargin: 5
            right: parent.right
            rightMargin: 5
            bottom: back_btn.top
            bottomMargin: 10
        }

        orientation: ListView.Vertical
        clip: true
        spacing: 5

        model: client.create_model_based_on_nickname(nickname, root.date)
        delegate: Rectangle {
            id: delegate

            width: parent.width
            height: 70

            border.width: 1
            border.color: "#000000"
            color: model.is_registered ? "#ff0000" : "#d70707"

            Text {
                id: number
                anchors {
                    left: parent.left
                    top: parent.top
                }
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                height: parent.height
                width: height * 0.75
                fontSizeMode: Text.Fit
                minimumPointSize: 5
                font.pointSize: 12
                elide: Text.ElideRight
                wrapMode: Text.WordWrap
                text: index + 1
            }
            Image {
                id: avatar
                anchors {
                    left: number.right
                    top: number.top
                }
                height: parent.height
                width: height
                source: model.avatar_path
//                mipmap: true
                property bool rounded: true
                property bool adapt: true

                layer.enabled: rounded
                layer.effect: OpacityMask {
                    maskSource: Item {
                        width: avatar.width
                        height: avatar.height
                        Rectangle {
                            anchors.centerIn: parent
                            width: avatar.adapt ? avatar.width : Math.min(avatar.width, avatar.height)
                            height: avatar.adapt ? avatar.height : width
                            radius: Math.min(width, height)
                        }
                    }
                }
            }
            Text {
                id: nickname
                anchors {
                    left: avatar.right
                    verticalCenter: parent.verticalCenter
                }
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                height: parent.height
                width: (parent.width - avatar.width - number.width) / 2
                fontSizeMode: Text.Fit
                minimumPointSize: 5
                font.pointSize: 12
                elide: Text.ElideRight
                wrapMode: Text.WordWrap
                text: String(model.nickname)
            }
            Text {
                id: time
                anchors {
                    left: nickname.right
                    top: nickname.top
                }
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                height: parent.height
                width: nickname.width
                fontSizeMode: Text.Fit
                minimumPointSize: 5
                font.pointSize: 12
                elide: Text.ElideRight
                wrapMode: Text.WordWrap
                text: String(model.time)
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    contacts_list_view.currentIndex = index
                    if(model.is_registered) {
                        var comp = Qt.createComponent("Contacts_list.qml");
                        var obj = comp.createObject(root, { nickname: nickname.text, date: date.text } );
                        stack_view.push(obj);
                    }
                }
            }
        }
    }
}
