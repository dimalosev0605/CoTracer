import QtQuick 2.14
import QtQuick.Controls 2.12
import QtCharts 2.14

import "../buttons"
import Chart_data_qml 1.0

Rectangle {
    id: root

    property alias chart_data: chart_data

    Back_btn {
        id: back_btn
        anchors {
            bottom: parent.bottom
            left: parent.left
            bottomMargin: 10
            leftMargin: 10
        }
        color: mouse_area.pressed ? "#708090" : parent.color
    }
    Settings_btn {
        id: settings_btn
        height: back_btn.height
        width: height
        anchors {
            right: parent.right
            rightMargin: back_btn.anchors.leftMargin
            bottom: parent.bottom
            bottomMargin: back_btn.anchors.bottomMargin
        }
        color: mouse_area.pressed ? "#708090" : parent.color
        mouse_area.onClicked: {
            menu.open()
        }
        Menu {
            id: menu
            MenuItem {
                checkable: true
                checked: true
                text: "Show sum"
                onTriggered: {
                    checked ? sum_line_series.visible = true :
                              sum_line_series.visible = false
                }
            }
            MenuItem {
                checkable: true
                text: "Show reg"
                onTriggered: {
                   checked ? reg_line_series.visible = true :
                             reg_line_series.visible = false
                }
            }
            MenuItem {
                checkable: true
                text: "Show unreg"
                onTriggered: {
                    checked ? unreg_line_series.visible = true :
                              unreg_line_series.visible = false
                }
            }
        }
    }

    Chart_data {
        id: chart_data
        onSum_point: {
            sum_line_series.append(x, y)
        }
        onReg_point: {
            reg_line_series.append(x, y)
        }
        onUnreg_point: {
            unreg_line_series.append(x, y)
        }
        onSet_max: {
            y_axis.max = max;
        }
    }

    ScrollView {
        id: scroll
        anchors {
            top: parent.top
            topMargin: 5
            bottom: back_btn.top
            bottomMargin: 5
            left: parent.left
            leftMargin: 5
            right: parent.right
            rightMargin: 5
        }
        ScrollBar.vertical.policy: ScrollBar.AlwaysOff
        clip: true

        ChartView {
            id: chart
            antialiasing: true
            height: scroll.height
            implicitWidth: if(root.width > root.height) {
                               scroll.width
                           }
                           else {
                               scroll.width * 2
                           }

            ValueAxis {
                id: x_axis
                min: 0
                max: 13
                tickCount: max - min + 1
                titleText: "Days ago"
                labelsFont:Qt.font({pointSize: 8})
            }
            ValueAxis {
                id: y_axis
                min: 0
                max: 15
                tickCount: max - min + 1
                titleText: "Count of contacts"
                labelsFont:Qt.font({pointSize: 5})
                visible: false
            }
            LineSeries {
                id: sum_line_series
                name: "Sum"
                axisX: x_axis
                axisY: y_axis
                color: "blue"
                width: 2
                pointsVisible: true
                pointLabelsClipping: false
                pointLabelsVisible: true
                pointLabelsFormat: "@yPoint"
                pointLabelsColor: "#000000"
                pointLabelsFont: Qt.font({family: "Times New Romans", pointSize: 12})
                onClicked: console.log("onClicked: " + point.x + ", " + point.y);
            }
            LineSeries {
                id: reg_line_series
                name: "Registered"
                axisX: x_axis
                axisY: y_axis
                color: "red"
                width: 2
                visible: false

                pointsVisible: true
                pointLabelsClipping: false
                pointLabelsVisible: true
                pointLabelsFormat: "@yPoint"
                pointLabelsColor: "#000000"
                pointLabelsFont: Qt.font({family: "Times New Romans", pointSize: 12})
            }
            LineSeries {
                id: unreg_line_series
                name: "Unregistered"
                axisX: x_axis
                axisY: y_axis
                color: "green"
                width: 2
                visible: false

                pointsVisible: true
                pointLabelsClipping: false
                pointLabelsVisible: true
                pointLabelsFormat: "@yPoint"
                pointLabelsColor: "#000000"
                pointLabelsFont: Qt.font({family: "Times New Romans", pointSize: 12})
            }
        }
    }
}
