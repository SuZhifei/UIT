import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import UIT.Type 1.0

Rectangle {
    id:root
    width: parent.width
    height: parent.height
    ColumnLayout{
        Row{
        GroupBox
        {
            id: eInfo
            width: root.width - 5
            height: root.height - listener.height
            title: "事件"

            ColumnLayout{
                width: parent.width
                height: parent.height
                spacing: 20

                Row
                {
                    width: 100
                    height: 100
                    Image {
                        anchors.fill: parent
                        fillMode: Image.PreserveAspectFit
                        source: "images/event.png"
                    }
                }

                ColumnLayout
                {
                    Label{ text: "路径：" }
                    TextInput{ id:ti; text: CurrentEventInfo.path; readOnly: true ; selectByMouse: true}
                }

                ColumnLayout
                {
                    width: parent.width
                    height: 50
                    EventArg
                    {
                        width: parent.width
                        height: parent.height
                        visible: CurrentEventInfo.type == Type.EventType_Event
                    }
                }

            }
        }
        }
        Row{
            id:listener
            EventListener
            {
                width: eInfo.width
                height: 300
            }
        }
    }
}
