import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import Qt.labs.platform 1.0
import QtQml 2.2

SystemTrayIcon {
    id: tray_main
    visible: true
    tooltip: service.tooltip
    iconSource: "qrc:/images/logo_1.ico"

    property ConfigureView cfg_dlg: ConfigureView{}

    menu: Menu {
        MenuItem {
            text: qsTr("connect")
            checkable: true
            checked: configure.enable
            onCheckedChanged: {
                configure.enable = checked
                tray_main.iconSource = checked ? "qrc:/images/logo.ico" : "qrc:/images/logo_1.ico";
            }
        }
        Menu {
            id: serverList
            title: /*qsTr(*/"server list"/*)*/
            MenuSeparator {}
            MenuItem {
                text: qsTr("configure server...")
                onTriggered: {
                    cfg_dlg.show()
                }
            }
        }
        MenuSeparator {}
        MenuItem {
            text: qsTr("start-up")
            checkable: true
            checked: service.startUp()
            onCheckedChanged: {
                service.setStartUp(checked)
            }
        }
        MenuItem {
            text: qsTr("quit")
            onTriggered: {
                service.stop()
                Qt.quit()
            }
        }

        /////////////////////////////////////////////////////////////////
        Instantiator {
            model: listModel
            onObjectAdded: serverList.insertItem(index, object)
            onObjectRemoved: serverList.removeItem(object)
            delegate: MenuItem {
                checkable: true
                checked: configure.index == index
                text: remarks + "(" + ip + ":" + port + ")"//modelData
                onCheckedChanged: {
                    if (checked != (configure.index == index)) {//to avoid loop first time
                        configure.index = index
                    }
                }
            }
        }
    }
}
