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

    background: Rectangle {
        radius: 10
        color: focus ? "grey" : "darkslategrey" // TODO: not working
    }

    contentItem: Text {
            text: parent.text
            font.pointSize: Qt.platform.os === "android" ? 25 : 16
            color: "white"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

    //onFocusChanged: // TODO:


}
