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
            update_stats_btn.rotation_anim.stop()
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
        width: height
        radius: 5
        color: mouse_area.pressed ? "#708090" : parent.color
        mouse_area.onClicked: {
            rotation_anim.start()
            client.fetch_stat_for_14_days()
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
            left: parent.left
            right: parent.right
            bottom: back_btn.top
            bottomMargin: 10
        }

        orientation: ListView.Vertical
        clip: true
        model: stat_model

        headerPositioning: ListView.OverlayHeader
        header: Item {
            id: my_header
            z: 3
            width: parent.width
            height: 40

            Rectangle {
                id: date_col
                anchors {
                    left: parent.left
                    top: parent.top
                }
                height: parent.height
                width: parent.width * 0.4
                color: date_col_m_area.pressed ? "#b22222" : "#a40404"
                Image {
                    id: sort_date_img
                    anchors {
                        left: parent.left
                        leftMargin: 2
                        verticalCenter: parent.verticalCenter
                    }
                    source: "qrc:/imgs/sort.png"
                    height: parent.height / 2
                    width: height
                    fillMode: Image.PreserveAspectFit
                }
                Text {
                    anchors {
                        left: sort_date_img.right
                    }
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    width: parent.width - sort_date_img.width - sort_date_img.anchors.leftMargin
                    height: parent.height
                    fontSizeMode: Text.Fit
                    minimumPointSize: 1
                    font.pointSize: 12
                    elide: Text.ElideRight
                    wrapMode: Text.WordWrap
                    text: "Date"
                }
                MouseArea {
                    id: date_col_m_area
                    anchors.fill: parent
                    onClicked: {
                        stat_model.sort_by_date()
                    }
                }
            }
            Rectangle {
                id: count_of_contacts_col
                anchors {
                    left: date_col.right
                }
                height: parent.height
                width: parent.width - date_col.width
                color: count_of_contacts_col_m_area.pressed ? "#b22222" : "#a40404"
                Image {
                    id: sort_count_of_contacts_img
                    anchors {
                        left: parent.left
                        verticalCenter: parent.verticalCenter
                    }
                    source: "qrc:/imgs/sort.png"
                    height: parent.height / 2
                    width: height
                    fillMode: Image.PreserveAspectFit
                }
                Text {
                    anchors {
                        left: sort_count_of_contacts_img.right
                    }
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    width: parent.width - sort_count_of_contacts_img.width
                    height: parent.height
                    fontSizeMode: Text.Fit
                    minimumPointSize: 1
                    font.pointSize: 12
                    elide: Text.ElideRight
                    wrapMode: Text.WordWrap
                    text: "Count of contacts"
                }
                MouseArea {
                    id: count_of_contacts_col_m_area
                    anchors.fill: parent
                    onClicked: {
                        stat_model.sort_by_count_of_contacts()
                    }
                }
            }
        }
        delegate: Rectangle {
            id: delegate
            z: 2
            width: parent.width
            height: 50
            color: delegate_m_area.pressed ? "#b22222" : "#d70707"
            Rectangle {
                id: bottom_line
                anchors {
                    bottom: parent.bottom
                }
                width: parent.width
                height: 1
                color: "#000000"
            }
            property string date: date.text
            Text {
                id: date
                anchors {
                    left: parent.left
                    leftMargin: 2
                    top: parent.top
                }
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                width: parent.width * 0.4 - anchors.leftMargin
                height: parent.height - bottom_line.height
                fontSizeMode: Text.Fit
                minimumPointSize: 1
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
                horizontalAlignment: Text.AlignLeft
                width: parent.width - date.width - read_more_btn.width
                height: parent.height - bottom_line.height
                fontSizeMode: Text.Fit
                minimumPointSize: 1
                font.pointSize: 12
                elide: Text.ElideRight
                wrapMode: Text.WordWrap
                text: String(model.count_of_contacts)
            }
            Image {
                id: read_more_btn
                anchors {
                    left: count_of_contacts.right
                    verticalCenter: parent.verticalCenter
                }
                source: "qrc:/imgs/read_more.png"
                fillMode: Image.PreserveAspectFit
                height: parent.height
                width: height
            }
            MouseArea {
                id: delegate_m_area
                anchors.fill: parent
                onClicked: {
                    days_list_view.currentIndex = index
                    stack_view.push(my_contacts_list_comp)
                }
            }
        }
    }
}
