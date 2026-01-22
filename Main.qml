import QtQuick
import QtQuick.Layouts


Window {
    id: mainWindow

    property int minWidth: 330
    property int minHeight: 510

    property int marginMiddle: 10
    property int marginHuge: 50

    visible: true
    title: qsTr("Mediocre Calculator")

    color: "black"

    width: Qt.platform.os === "android" ? Screen.width : minWidth
    height: Qt.platform.os === "android" ? Screen.height : minHeight

    minimumWidth: Qt.platform.os === "android" ? 0 : minWidth
    minimumHeight: Qt.platform.os === "android" ? 0 : minHeight

    maximumWidth: Qt.platform.os === "android" ? undefined : minWidth
    maximumHeight: Qt.platform.os === "android" ? undefined : minHeight

    flags: Qt.platform.os !== "android" ? Qt.Window : Qt.MaximizeUsingFullscreenGeometryHint

    Item {
        anchors.fill: parent

        anchors.topMargin: Qt.platform.os === "android" ? marginHuge : 0
        anchors.bottomMargin: Qt.platform.os === "android" ? marginHuge : 0

        Rectangle {
            id: wrapper

            radius: 20
            color: "#1c2e2e"

            anchors.fill: parent

            Item{
                anchors.fill: parent
                anchors.leftMargin: marginMiddle
                anchors.rightMargin: marginMiddle
                anchors.bottomMargin: Qt.platform.os === "android" ? marginHuge : marginMiddle

                ColumnLayout {
                    id: columnLayout
                    anchors.fill: parent

                    spacing: marginMiddle

                    Item {
                            Layout.fillHeight: true
                        }

                    Item {

                        Layout.fillWidth: true

                        clip: true

                        Layout.alignment: Qt.AlignBottom

                        Layout.preferredHeight: Qt.platform.os === "android" ? (mainWindow.height - 2*marginHuge) / 3.5 : calculation.height

                        Text {
                            id: calculation
                            anchors.bottom: parent.bottom

                            width: parent.width

                            color: "white"
                            font.pixelSize: mainWindow.height/25
                            horizontalAlignment: Text.AlignRight


                            elide: Qt.platform.os === "android" ? undefined : Text.ElideLeft
                            wrapMode: Qt.platform.os === "android" ? Text.Wrap : undefined

                            text: calculator.calculationString
                        }
                    }

                    Text {
                        id: currentNumber
                        color: "white"

                        font.pixelSize: mainWindow.height/20

                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignBottom

                        horizontalAlignment: Text.AlignRight

                        elide: Text.ElideLeft

                        text: calculator.currentNumberString
                    }

                    GridLayout{
                        flow: GridLayout.LeftToRight
                        columns: 4
                        rows: 5
                        columnSpacing: marginMiddle
                        rowSpacing: marginMiddle

                        width: partent.width

                        anchors.bottom: parent.bottom
                        Layout.alignment: Qt.AlignBottom

                        AppButton {
                            id: partLeft
                            text: qsTr("(")
                            Layout.row: 0
                            Layout.column: 0

                            onClicked: calculator.insertParth("(")
                        }
                        AppButton {
                            id: partRight
                            text: qsTr(")")
                            Layout.row: 0
                            Layout.column: 1

                            onClicked: calculator.insertParth(")")
                        }
                        AppButton {
                            id: backspace
                            text: qsTr("B")
                            Layout.row: 0
                            Layout.column: 2

                            onClicked: calculator.backspace()
                        }



                        AppButton {
                            id: seven
                            text: qsTr("7")
                            Layout.row: 1
                            Layout.column: 0

                            onClicked: calculator.insertDigit(7)
                        }
                        AppButton {
                            id: eight
                            text: qsTr("8")
                            Layout.row: 1
                            Layout.column: 1

                            onClicked: calculator.insertDigit(8)
                        }
                        AppButton {
                            id: nine
                            text: qsTr("9")
                            Layout.row: 1
                            Layout.column: 2

                            onClicked: calculator.insertDigit(9)
                        }



                        AppButton {
                            id: four
                            text: qsTr("4")
                            Layout.row: 2
                            Layout.column: 0

                            onClicked: calculator.insertDigit(4)
                        }
                        AppButton {
                            id: five
                            text: qsTr("5")
                            Layout.row: 2
                            Layout.column: 1

                            onClicked: calculator.insertDigit(5)
                        }
                        AppButton {
                            id: six
                            text: qsTr("6")
                            Layout.row: 2
                            Layout.column: 2

                            onClicked: calculator.insertDigit(6)
                        }



                        AppButton {
                            id: one
                            text: qsTr("1")
                            Layout.row: 3
                            Layout.column: 0

                            onClicked: calculator.insertDigit(1)
                        }
                        AppButton {
                            id: two
                            text: qsTr("2")
                            Layout.row: 3
                            Layout.column: 1

                            onClicked: calculator.insertDigit(2)
                        }
                        AppButton {
                            id: three
                            text: qsTr("3")
                            Layout.row: 3
                            Layout.column: 2

                            onClicked: calculator.insertDigit(3)
                        }



                        AppButton {
                            id: clear
                            text: qsTr("C")
                            Layout.row: 4
                            Layout.column: 0

                            onClicked: calculator.clear()
                        }
                        AppButton {
                            id: zero
                            text: qsTr("0")
                            Layout.row: 4
                            Layout.column: 1

                            onClicked: calculator.insertDigit(0)
                        }
                        AppButton {
                            id: decimal
                            text: qsTr(".")
                            Layout.row: 4
                            Layout.column: 2

                            onClicked: calculator.insertDecimal()
                        }




                        AppButton {
                            id: div
                            text: qsTr("/")
                            Layout.row: 0
                            Layout.column: 3

                            onClicked: calculator.insertOperation("/")
                        }
                        AppButton {
                            id: mult
                            text: qsTr("*")
                            Layout.row: 1
                            Layout.column: 3

                            onClicked: calculator.insertOperation("*")
                        }
                        AppButton {
                            id: minus
                            text: qsTr("-")
                            Layout.row: 2
                            Layout.column: 3

                            onClicked: calculator.insertOperation("-")
                        }
                        AppButton {
                            id: plus
                            text: qsTr("+")
                            Layout.row: 3
                            Layout.column: 3

                            onClicked: calculator.insertOperation("+")
                        }
                        AppButton {
                            id: equals
                            text: qsTr("=")
                            Layout.row: 4
                            Layout.column: 3

                            onClicked: calculator.calculate()
                        }
                    }
                }


            }



        }
    }

}
