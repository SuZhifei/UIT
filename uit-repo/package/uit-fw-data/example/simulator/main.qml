import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.0


ApplicationWindow
{
    id:window
    visible: true
    width: 1200
    height: 700
    minimumWidth: 1200
    minimumHeight: 700
    title: "数据调试工具"

    FileDialog
    {
        id:dirChoose
        title: qsTr("配置数据加载路径")
        selectFolder: true
        modality: "WindowModal"
        onAccepted: {
            ModelCluster.load(dirChoose.folder)
        }
        Component.onCompleted: visible = false
    }

    menuBar: MenuBar
    {
        Menu
        {
            title: "文件"
            MenuItem
            {
                text: "配置"
                onTriggered: dirChoose.open()
            }
            MenuItem {
                text: "退出"
                onTriggered: Qt.quit()
            }
        }

        Menu {
            title: "选项"
            MenuItem {
                text: "设置"
                onTriggered:dia.visible = true
            }
        }

    }

    MainPage
    {
        width: window.width
        height: window.height - 50
        focus: true
    }

    Dialog{
        id: dia
        title: "设置远程socket"
        modality: Qt.ApplicationModal
        width: 300
        height: 150
        contentItem:
            Rectangle{
            width: dia.width
            height: dia.height
            ColumnLayout{
                width: dia.width
                height: dia.height
                Row{
                    Label{id: l1; text: "远程IP："; width: 80; height: 30}
                    TextField{id: ip; width: 200;height: 30; text: "10.219.118.25"}
                }
                Row{
                    Label{text: "远程端口："; width: l1.width; height: l1.height}
                    TextField{id: port; width: ip.width;height: ip.height; text: "8888"}
                }

                Button{
                        text: "连接"
                        Layout.fillWidth: true
                        height: 80
                        onClicked: {
                            SocketDebugger.connectToHost(ip.text, Number(port.text))
                        }
                }
            }
        }

    }

    statusBar: StatusBar {
        RowLayout {
            anchors.fill: parent
            Label {
                id:lb
                text: "ready"
            }
        }
    }

}
