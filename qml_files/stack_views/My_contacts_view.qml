import QtQuick 2.14
import QtQuick.Controls 2.12

import ".."
import "../buttons"
import Days_model_qml 1.0
import Communication_tcp_client_qml 1.0

Rectangle {
    id: root
    z: 0
    color: "#e00d0d"

    Days_model {
        id: days_model
    }

    Component {
        id: my_contacts_list_comp
        My_contacts_list {
            id: my_contacts_list
        }
    }
    Component {
        id: chart_view_comp
        Chart_view {
            id: chart_view
        }
    }

    My_dialog {
        id: my_dialog
        z: 3
        anchors.centerIn: parent
        width: if(parent.height > parent.width) {
                   parent.width * 0.9
               }
               else {
                   parent.width * 0.5
               }

        height: if(parent.height > parent.width) {
                    parent.height * 0.2
                }
                else {
                    parent.height * 0.5
                }

        visible: true
        opacity: 0.7
    }

    Communication_tcp_client {
        id: client
        onInfo: {
            my_dialog.busy_indicator.running = false
            my_dialog.text.text = info_message
            my_dialog.opacity_anim.start()
        }
        onStatistics_received: {
            days_model.receive_stats(m_stats)
            my_dialog.busy_indicator.running = false
            my_dialog.text.text = "Statistics was received."
            my_dialog.opacity_anim.start()
        }
        onFetching_statistics: {
            my_dialog.busy_indicator.running = true
            my_dialog.text.text = message
            my_dialog.visible = true
        }
    }

    Back_btn {
        id: back_btn
        z: 1
        anchors {
            bottom: parent.bottom
            left: parent.left
            bottomMargin: 10
            leftMargin: 10
        }
        color: mouse_area.pressed ? "#708090" : parent.color
    }
    Chart_btn {
        id: chart_btn
        z: 1
        anchors {
            right: parent.right
            bottomMargin: back_btn.anchors.bottomMargin
            bottom: parent.bottom
            rightMargin: back_btn.anchors.leftMargin
        }
        height: back_btn.height
        width: height * 1.2
        color: mouse_area.pressed ? "#708090" : parent.color
    }

    ListView {
        id: days_list_view
        z: 1

        anchors {
            top: parent.top
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
        model: days_model

        headerPositioning: ListView.OverlayHeader
        header: Rectangle {
            z: 3
            width: parent.width
            height: 50
            border.width: 1
            color: root.color
            Text {
                id: sort_date
                anchors {
                    left: parent.left
                    top: parent.top
                }
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                width: parent.width * 0.4
                height: parent.height
                fontSizeMode: Text.Fit
                minimumPointSize: 1
                font.pointSize: 12
                elide: Text.ElideRight
                wrapMode: Text.WordWrap
                text: "Date"
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        days_model.sort_by_date()
                    }
                }
            }
            Text {
                id: sort_reg_count
                anchors {
                    left: sort_date.right
                    top: sort_date.top
                }
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                width: (parent.width - sort_date.width) / 3
                height: parent.height
                fontSizeMode: Text.Fit
                minimumPointSize: 1
                font.pointSize: 12
                elide: Text.ElideRight
                wrapMode: Text.WordWrap
                text: "Reg."
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        days_model.sort_by_reg_count()
                    }
                }
            }
            Text {
                id: sort_unreg_count
                anchors {
                    left: sort_reg_count.right
                    top: sort_reg_count.top
                }
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                width: sort_reg_count.width
                height: parent.height
                fontSizeMode: Text.Fit
                minimumPointSize: 1
                font.pointSize: 12
                elide: Text.ElideRight
                wrapMode: Text.WordWrap
                text: "Unreg."
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        days_model.sort_by_unreg_count()
                    }
                }
            }
            Text {
                id: sort_sum_contacts
                anchors {
                    left: sort_unreg_count.right
                    top: sort_unreg_count.top
                }
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                width: sort_reg_count.width
                height: parent.height
                fontSizeMode: Text.Fit
                minimumPointSize: 1
                font.pointSize: 12
                elide: Text.ElideRight
                wrapMode: Text.WordWrap
                text: "Sum"
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        days_model.sort_by_sum()
                    }
                }
            }
        }

        delegate: Rectangle {
            id: delegate
            z: 2

            width: parent.width
            height: 50

            border.width: 1
            border.color: "#000000"
            color: "#d70707"

            property string date: date.text
            Text {
                id: date
                anchors {
                    left: parent.left
                    top: parent.top
                }
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                width: parent.width * 0.4
                height: parent.height
                fontSizeMode: Text.Fit
                minimumPointSize: 5
                font.pointSize: 12
                elide: Text.ElideRight
                wrapMode: Text.WordWrap
                text: String(model.date)
            }
            Text {
                id: reg_count
                anchors {
                    left: date.right
                    top: date.top
                }
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                width: (parent.width - date.width) / 3
                height: parent.height
                fontSizeMode: Text.Fit
                minimumPointSize: 5
                font.pointSize: 12
                elide: Text.ElideRight
                wrapMode: Text.WordWrap
                text: String(model.reg_count)
            }
            Text {
                id: unreg_count
                anchors {
                    left: reg_count.right
                    top: reg_count.top
                }
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                width: reg_count.width
                height: parent.height
                fontSizeMode: Text.Fit
                minimumPointSize: 5
                font.pointSize: 12
                elide: Text.ElideRight
                wrapMode: Text.WordWrap
                text: String(model.unreg_count)
            }
            Text {
                id: sum_contacts
                anchors {
                    left: unreg_count.right
                    top: unreg_count.top
                }
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                width: reg_count.width
                height: parent.height
                fontSizeMode: Text.Fit
                minimumPointSize: 5
                font.pointSize: 12
                elide: Text.ElideRight
                wrapMode: Text.WordWrap
                text: parseInt(reg_count.text) + parseInt(unreg_count.text)
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    days_list_view.currentIndex = index
                    stack_view.push(my_contacts_list_comp)
                }
            }
        }
    }
}
