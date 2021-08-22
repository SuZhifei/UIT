import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2

Rectangle {
    id:root
    width: parent.width
    height: parent.height

    objectName: "eventListener"

    function getEvent()
    {
        return event.text
    }

    function getCmds()
    {
        var ret = ""
        for(var i = 0; i < listMode.count; ++i)
        {
            var one = listMode.get(i).path + "=" + listMode.get(i).value
            ret = ret + "|" + one
        }
        return ret
    }

    ListModel{
        id:listMode
    }

    GroupBox{
        title: "监听（收到事件后执行指令序列）"

        width: parent.width
        height: parent.height - 5
        ColumnLayout{
            width: parent.width
            height: parent.height

            Row{
                Label{text: "监听的事件名称："; verticalAlignment: Text.AlignVCenter}
                TextField{id: event; width: 300}
            }

            Row{
                TextInput{text: "执行的指令序列：（形如：RadioData.CurrentMediaInfo.Frequency.Current=0)";readOnly: true ; selectByMouse: true}
                }

            Row{
                spacing: 5
                TextField{id: iPath;text: ""; width: 300}
                TextInput{text: "="; verticalAlignment: TextInput.AlignVCenter}
                TextField{id: iValue; text: ""; width: 100}
                Button{
                    text: "添加指令"
                    onClicked: {
                        if(listMode.count >= 10 || iPath.text ==="")
                            return
                        listMode.append({})
                        var count = listMode.count
                        listMode.setProperty(count - 1, "path", iPath.text)
                        listMode.setProperty(count - 1, "value", iValue.text)
                        iPath.text=""
                        iValue.text=""
                    }
                }
            }

            Row{
                ListView{
                    width: root.width
                    height: 100
                    model: listMode
                    delegate:
                        Row{
                            Text { text: path; width: iPath.width; height: 10}
                            Text { text: "="}
                            Text { text: value; width: iValue.width; height: 10}
                            Button{
                                text: "del"; height: 20
                                onClicked: {
                                    listMode.remove(listMode.count - 1)
                                }
                            }
                        }

                    }
            }
        }

    }

}
