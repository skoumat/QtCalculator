import QtQuick
import QtQuick.Layouts


Window {
    id: mainWindow
    width: 330
    height: 493
    minimumWidth: 330
    minimumHeight: 493
    visible: true
    title: qsTr("Mediocre Calculator")

    color: "black"

    flags: Qt.platform.os !== "android" ? Qt.Window : Qt.MaximizeUsingFullscreenGeometryHint

    property int marginMiddle: 10
    property int marginHuge: 50
    property int contentMaxWidth: 430
    property int contentMaxHeight: 410

    Item {
        //anchors.centerIn: parent
        anchors.margins: 50

        //width: Math.min(parent.width, contentMaxWidth)
        //height: Math.min(parent.height, contentMaxHeight)

        Rectangle {
            id: wrapper
            implicitHeight: columnLayout.implicitHeight + 20
            implicitWidth: columnLayout.implicitWidth + 20
            border.width: 3
            border.color: "blue"
            radius: 20
            color: "orange"

            ColumnLayout {
                id: columnLayout
                anchors.fill: parent
                anchors.margins: 10

                spacing: marginMiddle

                Text {
                    id: calculation
                    color: "white"
                    font.pointSize: 15

                    Layout.fillWidth: true

                    horizontalAlignment: Text.AlignRight

                    wrapMode: Text.Wrap

                    text: calculator.calculationString
                }

                Text {
                    id: currentNumber
                    color: "white"
                    font.pointSize: 20

                    Layout.fillWidth: true

                    horizontalAlignment: Text.AlignRight

                    wrapMode: Text.Wrap

                    text: calculator.currentNumberString
                }

                GridLayout{
                    flow: GridLayout.LeftToRight
                    columns: 4
                    rows: 5
                    columnSpacing: marginMiddle
                    rowSpacing: marginMiddle

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
