import QtQuick 2.14
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0

import "../buttons"

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
            horizontalCenter: parent.horizontalCenter

        }
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        width: parent.width
        height: 30
        fontSizeMode: Text.Fit
        minimumPointSize: 1
        font.pointSize: 12
        elide: Text.ElideRight
        wrapMode: Text.WordWrap
        text: root.date
        Rectangle {
            id: under_date_line
            anchors {
                top: date.bottom
            }
            height: 1
            width: parent.width
            color: "#000000"
        }
    }

    ListView {
        id: contacts_list_view
        anchors {
            top: date.bottom
            topMargin: under_date_line.height
            left: parent.left
            right: parent.right
            bottom: back_btn.top
            bottomMargin: 10
        }
        orientation: ListView.Vertical
        clip: true
        model: client.create_model_based_on_date_and_nickname(nickname, root.date)
        delegate: Rectangle {
            id: delegate
            width: parent.width
            height: 50
            color: mouse_area.pressed ? "#b22222" : "#d70707"
            Rectangle {
                id: delegate_bottom_line
                anchors {
                    bottom: parent.bottom
                }
                height: 1
                width: parent.width
                color: "#000000"
            }
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
                minimumPointSize: 1
                font.pointSize: 12
                elide: Text.ElideRight
                wrapMode: Text.WordWrap
                text: index + 1
            }
            Image {
                id: avatar
                anchors {
                    left: number.right
                    verticalCenter: parent.verticalCenter
                }
                height: parent.height - delegate_bottom_line.height - 4
                width: height
                source: model.contact_avatar_path
                mipmap: true
                asynchronous: true
                fillMode: Image.PreserveAspectCrop
                layer.enabled: true
                layer.effect: OpacityMask {
                    maskSource: Rectangle {
                        width: avatar.width
                        height: avatar.height
                        radius: 5
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
                width: (parent.width - number.width - avatar.width) / 2
                fontSizeMode: Text.Fit
                minimumPointSize: 1
                font.pointSize: 12
                elide: Text.ElideRight
                wrapMode: Text.WordWrap
                text: String(model.contact_nickname)
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
                minimumPointSize: 1
                font.pointSize: 12
                elide: Text.ElideRight
                wrapMode: Text.WordWrap
                text: String(model.contact_time)
            }
            MouseArea {
                id: mouse_area
                anchors.fill: parent
                onClicked: {
                    contacts_list_view.currentIndex = index
                    var comp = Qt.createComponent("Contacts_list.qml");
                    var obj = comp.createObject(root, { nickname: nickname.text, date: date.text } );
                    stack_view.push(obj);
                }
            }
        }
    }
}
