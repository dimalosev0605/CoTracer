import QtQuick 2.14
import QtQuick.Controls 2.12
import QtCharts 2.14

import "../buttons"
import Chart_data_qml 1.0

Rectangle {
    id: root
    color: "#e00d0d"

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

    ChartView {
        id: chart
        antialiasing: true
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

        ValueAxis {
            id: x_axis
            min: 0
            max: 13
            tickCount: max - min + 1
            titleText: "Days ago"
            labelsFont:Qt.font({pointSize: 5})
        }
        ValueAxis {
            id: y_axis
            min: 0
            max: 15
            tickCount: max - min + 1
            titleText: "Count of contacts"
            labelsFont:Qt.font({pointSize: 5})
        }
        LineSeries {
            id: sum_line_series
            name: "Sum"
            axisX: x_axis
            axisY: y_axis
            color: "blue"
            width: 2.5
        }
        LineSeries {
            id: reg_line_series
            name: "Registered"
            axisX: x_axis
            axisY: y_axis
            color: "red"
            width: 2.5
        }
        LineSeries {
            id: unreg_line_series
            name: "Unregistered"
            axisX: x_axis
            axisY: y_axis
            color: "green"
            width: 2.5
        }
    }
}
