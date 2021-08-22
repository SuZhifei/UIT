import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import UIT.Type 1.0

Rectangle
{
    width: parent.width
    height: parent.height
    GroupBox
    {
        width: parent.width - 5
        height: parent.height - 5
        title: "数据"

        ColumnLayout
        {
            width: parent.width
            height: parent.height
            spacing: 10
            ColumnLayout
            {
                DataIcon { width: 100; height: 100; valueType: CurrentDataInfo.type }
            }

            ColumnLayout
            {
                Label{ text: "路径：　　" }
                TextInput{ id:ti; text: CurrentDataInfo.path; readOnly: true ; selectByMouse: true}
            }
            ColumnLayout
            {
                Label{ text: "qml属性：" }
                TextInput{
                    text:
                    {
                        if(CurrentDataInfo.path === "")
                        {
                            return ""
                        }
                        else
                        {
                            var str = CurrentDataInfo.path
                            str = str.replace(/\./g, '_')
                            return str.substring(0, 1).toLowerCase() + str.substring(1)
                        }
                    }
                    readOnly: true
                    selectByMouse: true
                }
            }

            ColumnLayout
            {
                Label{ text: "附加类型：" }
                TextInput{ text: CurrentDataInfo.extraType; readOnly: true; selectByMouse: true }
            }
            ColumnLayout
            {
                width: parent.width
                height: parent.height
                ValueInfo
                {
                    id:vPanel
                    width: parent.width
                    height: parent.height
                }
            }

            GroupBox
            {
                title: "按键"
                Row{
                    spacing: 30
                    GridLayout{
                        id: vkey
                        rows:3
                        columns: 3
                        Button{text: "左上"; onClicked: ModelCluster.onVKey(Type.LeftUp, keystate.getKeyState())}
                        Button{text: "上"; onClicked: ModelCluster.onVKey(Type.Up, keystate.getKeyState())}
                        Button{text: "右上"; onClicked: ModelCluster.onVKey(Type.RightUp, keystate.getKeyState())}
                        Button{text: "左"; onClicked: ModelCluster.onVKey(Type.Left, keystate.getKeyState())}
                        Button{text: "OK"; onClicked: ModelCluster.onVKey(Type.Ok, keystate.getKeyState())}
                        Button{text: "右"; onClicked: ModelCluster.onVKey(Type.Right, keystate.getKeyState())}
                        Button{text: "左下"; onClicked: ModelCluster.onVKey(Type.LeftDown, keystate.getKeyState())}
                        Button{text: "下"; onClicked: ModelCluster.onVKey(Type.Down, keystate.getKeyState())}
                        Button{text: "右下"; onClicked: ModelCluster.onVKey(Type.RightDown, keystate.getKeyState())}
                    }
                    Column{
                        spacing: 5
                        Button{text: "逆时针"; onClicked: ModelCluster.onVKey(Type.AntiClockwise, keystate.getKeyState())}
                        Button{text: "顺时针"; onClicked: ModelCluster.onVKey(Type.Clockwise, keystate.getKeyState())}
                        Button{text: "返回"; onClicked: ModelCluster.onVKey(Type.Back, keystate.getKeyState())}
                    }
                    GroupBox{
                        id: keystate
                        function getKeyState(){
                            if(p.checked)   return Type.Press
                            if(c.checked)   return Type.Cancel
                            if(release.checked)   return Type.Release
                            if(l.checked)   return Type.LongPress
                            if(clk.checked)   return Type.Click
                        }

                        Row{
                            ExclusiveGroup{id: group}
                            RadioButton{id: p; text:"压下"; exclusiveGroup: group; checked: true}
                            RadioButton{id: c; text:"取消"; exclusiveGroup: group}
                            RadioButton{id: release; text:"释放"; exclusiveGroup: group}
                            RadioButton{id: l; text:"长按"; exclusiveGroup: group}
                            RadioButton{id: clk; text:"点击"; exclusiveGroup: group}
                        }
                    }
                }
        }


        }
    }



}
