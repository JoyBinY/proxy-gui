import QtQuick 2.2
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4

ApplicationWindow {
    property int margin: 11

    id: mainWin
    title: "proxy-go"
    width: mainLayout.implicitWidth + 2 * margin
    height: mainLayout.implicitHeight + 2 * margin
    minimumWidth: mainLayout.Layout.minimumWidth + 2 * margin
    minimumHeight: mainLayout.Layout.minimumHeight + 2 * margin

    GridLayout {
        id: mainLayout
        anchors.fill: parent
        anchors.margins: margin
        columns: 2
        flow: GridLayout.LeftToRight
        columnSpacing: 20
        rowSpacing: 20

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            border.color: "gray"
            ListView {
                id: listView
                anchors.fill: parent
                anchors.margins: 2
                model: listModel
                delegate: listDelegate
                focus: true
                Keys.enabled: true
                highlight: Rectangle { color: "lightblue" }
                onCurrentIndexChanged: {
                    txt_ip.text = model.data(currentIndex, "ip")
                    txt_port.text = model.data(currentIndex, "port")
                    txt_remarks.text = model.data(currentIndex, "remarks")
                    txt_cert.text = model.data(currentIndex, "cert")
                    txt_key.text = model.data(currentIndex, "key")
                }
                Keys.onPressed: {
                    var size = listModel.rowCount()
                    if (event.key === Qt.Key_Up) {
                        currentIndex += (currentIndex != 0) ? -1 : size - 1
                    } else if (event.key === Qt.Key_Down) {
                        currentIndex -= (currentIndex != size - 1) ? -1 : size - 1
                    }
                    event.accepted = true;
                }
            }
        }

        GroupBox {
            title: qsTr("Server:")
            Layout.fillWidth: true
            Layout.fillHeight: true
            GridLayout {
                rows: 5
                flow: GridLayout.TopToBottom
                Label { text: qsTr("Server IP:") }
                Label { text: qsTr("Server Port:") }
                Label { text: qsTr("Cert:") }
                Label { text: qsTr("Key:") }
                Label { text: qsTr("Remarks:") }

                TextField {
                    id: txt_ip
                    Layout.minimumWidth: 150
                    onTextChanged: {
                        listModel.setData(listView.currentIndex, text, "ip")
                    }
                }
                TextField {
                    id: txt_port
                    Layout.minimumWidth: 150
                    onTextChanged: {
                        listModel.setData(listView.currentIndex, text, "port")
                    }
                }
                TextField {
                    id: txt_cert
                    Layout.minimumWidth: 150
                    onTextChanged: {
                        listModel.setData(listView.currentIndex, text, "cert")
                    }
                }
                TextField {
                    id: txt_key
                    Layout.minimumWidth: 150
                    onTextChanged: {
                        listModel.setData(listView.currentIndex, text, "key")
                    }
                }
                TextField {
                    id: txt_remarks
                    Layout.minimumWidth: 150
                    onTextChanged: {
                        listModel.setData(listView.currentIndex, text, "remarks")
                    }
                }
            }
        }

        GridLayout {
            columns: 2
            Layout.alignment : Qt.AlignHCenter
            columnSpacing: 20
            rowSpacing: 10
            Button {
                text: qsTr("Add")
                onClicked: {
                    listModel.addItem("new server", "127.0.0.1", "8888", "", "", "", "")
                    listView.currentIndex = listModel.rowCount() - 1
                }
            }
            Button {
                text: qsTr("Remove")
                onClicked: {
                    listModel.removeItem(listView.currentIndex)
                    listView.currentIndex -= 1
                }
            }
            Button {
                text: qsTr("Up")
                onClicked: {
                    if (listModel.updownItem(listView.currentIndex, -1)) {
                        listView.currentIndex -= 1
                    }
                }
            }
            Button {
                text: qsTr("Down")
                onClicked: {
                    if (listModel.updownItem(listView.currentIndex, 1)) {
                        listView.currentIndex += 1
                    }
                }
            }
        }

        GridLayout {
            columns: 2
            Layout.alignment : Qt.AlignHCenter
            columnSpacing: 20
            rowSpacing: 10

            Label { text: qsTr("Local Port:") }
            TextField {
                text: configure.port
                onTextChanged: configure.port = text
            }
            Button {
                text: qsTr("OK")
                onClicked: {
                    listModel.save()
                    mainWin.hide()
                }
            }
            Button {
                text: qsTr("Cannel")
                onClicked: {
                    listModel.reset()
                    mainWin.hide()
                }
            }
        }
    }

    Component {
        id: listDelegate
        Item {
            height: 20
            width: parent.width
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    parent.ListView.view.currentIndex = index//from model
                    parent.ListView.view.focus = true
                }
            }
            Text {
                anchors.fill: parent
                color: parent.ListView.view.isCurrentItem ? "blue" : "black"
                text: "<b>" + remarks + "</b>"
                font.pixelSize: 18
            }
        }
    }
}
