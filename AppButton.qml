import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

RoundButton  {
    radius: 10

    Layout.minimumWidth: 50
    Layout.minimumHeight: Layout.minimumWidth

    Layout.preferredWidth: 70
    Layout.preferredHeight: Layout.preferredWidth

    Layout.maximumWidth: 100
    Layout.maximumHeight: Layout.maximumWidth

    Layout.fillWidth: true
    Layout.fillHeight: true

    background: Rectangle {
        radius: 10
        color: focus ? "grey" : "darkslategrey"
    }



    //onFocusChanged:


}
