import QtQuick 2.14
import QtQuick.Controls 2.12
import QtCharts 2.14
import QtQuick.Window 2.14

import "../buttons"
import Chart_data_qml 1.0

Rectangle {
    id: root

    property alias chart_data: chart_data
    objectName: "Chart_view" // for enabling rotation

    Back_btn {
        id: back_btn
        anchors {
            bottom: parent.bottom
            left: parent.left
            bottomMargin: 10
            leftMargin: 10
        }
        color: mouse_area.pressed ? "#708090" : parent.color
        visible: Screen.orientation === Qt.PortraitOrientation
    }

    Chart_data {
        id: chart_data
        onPoint: {
            line_series.append(x, y)
        }
        onSet_max_count_of_contacts: {
            y_axis.max = max_value
        }
    }

    ScrollView {
        id: scroll
        anchors {
            top: parent.top
            topMargin: 5
            bottom: Screen.orientation === Qt.PortraitOrientation ?
                        back_btn.top : parent.bottom
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
            backgroundColor: root.color
            legend.visible: false
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
                labelsFont: Qt.font({pointSize: 8, weight: 99})
                color: "#000000"
                titleVisible: true
                gridVisible: false
            }
            ValueAxis {
                id: y_axis
                min: 0
                max: 14
                tickCount: if(Screen.orientation === Qt.PortraitOrientation) {
                               max - min + 1
                           }
                           else {
                               (max - min + 1) / 2
                           }
                titleText: "Count of contacts"
                labelsFont: Qt.font({pointSize: 8, weight: 99})
                color: "#000000"
                titleVisible: true
                gridVisible: false
            }
            LineSeries {
                id: line_series
                axisX: x_axis
                axisY: y_axis
                color: "#000000"
                width: 2
                pointsVisible: true
                pointLabelsClipping: false
                pointLabelsVisible: true
                pointLabelsFormat: "@yPoint"
                pointLabelsColor: "#000000"
                pointLabelsFont: Qt.font({family: "Times New Romans", pointSize: 10, color: "#000000"})
            }
        }
    }
}
