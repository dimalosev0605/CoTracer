import QtQuick 2.14
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0

import "../buttons"
import ".."

Rectangle {
    id: root
    z: 0

    Component {
        id: add_contact_view_comp
        Add_contact_view {
            date: days_list_view.currentItem.date
        }
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
            // poped defined in Back_btn.qml
            client.cancel_operation()
            client.pop_model()
        }
    }

    Add_contact_btn {
        id: add_contact_btn
        z: 0

        width: back_btn.width
        height: width

        anchors {
            right: parent.right
            rightMargin: 10
            bottom: parent.bottom
            bottomMargin: 10
        }
        color: mouse_area.pressed ? "#00ff00" : parent.color
        mouse_area.onClicked: {
            stack_view.push(add_contact_view_comp)
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
        text: days_list_view.currentItem.date
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
        z: 0
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
        model: client.create_model_based_on_date(days_list_view.currentItem.date)
        delegate: Rectangle {
            id: delegate

            property alias nickname: nickname
            property alias time: time

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
                height: parent.height - delegate_bottom_line.height
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
                height: parent.height - delegate_bottom_line.height
                width: (parent.width - number.width - avatar.width - remove_contact_item.width) / 2
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
                height: parent.height - delegate_bottom_line.height
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
                anchors {
                    left: parent.left
                    right: remove_contact_item.left
                }
                height: parent.height - delegate_bottom_line.height
                onClicked: {
                    contacts_list_view.currentIndex = index
                    var comp = Qt.createComponent("Contacts_list.qml");
                    var obj = comp.createObject(root, { nickname: nickname.text, date: date.text } );
                    stack_view.push(obj);
                }
            }
            Item {
                id: remove_contact_item
                anchors {
                    right: parent.right
                }
                height: parent.height - delegate_bottom_line.height
                width: height
                Rectangle {
                    id: remove_contact_rect
                    anchors {
                        centerIn: parent
                    }
                    color: remove_contact_m_area.pressed ? "#000000" : delegate.color
                    height: remove_contact_img.height
                    width: remove_contact_img.width
                    radius: height / 2
                    Image {
                        id: remove_contact_img
                        anchors {
                            centerIn: parent
                        }
                        height: remove_contact_item.height * 0.75
                        width: height
                        fillMode: Image.PreserveAspectFit
                        source: "qrc:/imgs/remove.png"
                    }
                }
                MouseArea {
                    id: remove_contact_m_area
                    anchors.fill: parent
                    onClicked: {
                        contacts_list_view.currentIndex = index
                        client.remove_contact(contacts_list_view.currentItem.nickname.text,
                                              contacts_list_view.currentItem.time.text,
                                              days_list_view.currentItem.date,
                                              index)
                    }
                }
            }
        }
    }
}
