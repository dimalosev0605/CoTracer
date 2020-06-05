import QtQuick 2.14
import QtQuick.Controls 2.12

Rectangle {
    property alias mouse_area: mouse_area
    radius: 5
    Image {
        anchors.fill: parent
        source: "qrc:/imgs/chart.png"
        MouseArea {
            id: mouse_area
            anchors.centerIn: parent
            width: parent.width + 3
            height: parent.height + 3
            onClicked: {
                stack_view.push(chart_view_comp)
                stack_view.currentItem.chart_data.receive_stat(stat_model.get_stat())
            }
        }
    }
}

