import QtQuick 2.14
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0

Rectangle {
    id: root

    property string nick
    property string src
    property string default_color
    property bool type // if true -> show delete button else -> hide + on root_m_area action

    height: 50
    color: root_mouse_area.pressed ? "#b22222" : root.default_color

    Image {
        id: avatar
        anchors {
            left: parent.left
            leftMargin: 5
            verticalCenter: parent.verticalCenter
        }
        height: parent.height - 5
        width: height
        source: root.src
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
            top: parent.top
        }
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        height: parent.height
        width: parent.width - avatar.width - avatar.anchors.leftMargin - remove_friend_img.width
        fontSizeMode: Text.Fit
        minimumPointSize: 1
        font.pointSize: 12
        elide: Text.ElideRight
        wrapMode: Text.WordWrap
        text: root.nick
    }
    MouseArea {
        id: root_mouse_area
        anchors {
            top: parent.top
            bottom: parent.bottom
            left: parent.left
            right: remove_friend_item.left
        }
        onClicked: {
            if(!root.type) {
                nickname_field.text = root.nick
                stack_view.pop()
            }
        }
    }
    Item {
        id: remove_friend_item
        anchors {
            right: parent.right
        }
        height: parent.height
        width: height
        visible: root.type
        Rectangle {
            id: remove_friend_rect
            anchors {
                centerIn: parent
            }
            color: remove_friend_m_area.pressed ? "#000000" : root.color
            height: remove_friend_img.height
            width: remove_friend_img.width
            radius: height / 2
            Image {
                id: remove_friend_img
                anchors {
                    centerIn: parent
                }
                height: remove_friend_item.height * 0.75
                width: height
                fillMode: Image.PreserveAspectFit
                source: "qrc:/imgs/remove.png"
            }
        }
        MouseArea {
            id: remove_friend_m_area
            anchors.fill: parent
            onClicked: {
                my_friends_list_view.currentIndex = index
                client.remove_from_my_friends(root.nick)
            }
        }
    }
}
