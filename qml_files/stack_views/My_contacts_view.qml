import QtQuick 2.14
import QtQuick.Controls 2.12

import ".."
import "../buttons"
import Stat_model_qml 1.0

Rectangle {
    id: root
    z: 0

    Component.onCompleted: {
        client.fetch_stat_for_14_days()
    }

    Stat_model {
        id: stat_model
    }

    Component {
        id: my_contacts_list_comp
        My_contacts_list {
            id: my_contacts_list
            color: root.color
        }
    }

    Component {
        id: chart_view_comp
        Chart_view {
            id: chart_view
            color: root.color
        }
    }

    Connections {
        target: client
        ignoreUnknownSignals: true
        onStatistic_received: {
            stat_model.receive_stats(statistic)
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
        mouse_area.onClicked: {
            client.cancel_operation()
        }
    }

    Update_stats_btn {
        id: update_stats_btn
        z: 1
        anchors {
            bottom: parent.bottom
            bottomMargin: back_btn.anchors.bottomMargin
            horizontalCenter: parent.horizontalCenter
        }
        height: back_btn.height
        width: height * 1.6
        color: mouse_area.pressed ? "#708090" : parent.color
        mouse_area.onClicked: {
            client.request_for_14_days_stats()
        }
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
        model: stat_model

        headerPositioning: ListView.OverlayHeader
        header: Rectangle {
            id: my_header
            z: 3
            width: parent.width
            height: 50
            border.width: 1
            color: root.color
            Text {
                id: date_header
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
                    }
                }
            }
            Text {
                id: count_of_contacts_header
                anchors {
                    left: date_header.right
                    top: date_header.top
                }
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                width: parent.width - date_header.width
                height: parent.height
                fontSizeMode: Text.Fit
                minimumPointSize: 1
                font.pointSize: 12
                elide: Text.ElideRight
                wrapMode: Text.WordWrap
                text: "Count of contacts"
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
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
                id: count_of_contacts
                anchors {
                    left: date.right
                    top: date.top
                }
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                width: parent.width - date.width
                height: parent.height
                fontSizeMode: Text.Fit
                minimumPointSize: 5
                font.pointSize: 12
                elide: Text.ElideRight
                wrapMode: Text.WordWrap
                text: String(model.count_of_contacts)
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
