import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.2
import UIT.Type 1.0

Rectangle {
    function randomInt(Min,Max)
    {
        var Range = Max - Min;
        var Rand = Math.random();
        var ret = (Min + Math.round(Rand * Range));
        return ret
    }

    function randomReal()
    {
        var ret = randomInt(-100000, 100000) + "." + randomInt(0, 1000)
        return ret
    }

    Type
    {
        id:tp
    }

    ColumnLayout
    {
        width: parent.width
        height: parent.height
        Label{text: "当前数据："}
        Rectangle
        {
            Layout.fillWidth: true
            Layout.fillHeight: true
            ColumnLayout
            {
                id: boolNode
                visible: CurrentDataInfo.type == Type.DataType_Bool
                CheckBox{
                    checked: CurrentDataInfo.value !== undefined ? CurrentDataInfo.value : false
                    text: checked ? "true" : "false"
                    onClicked: ModelCluster.onBoolChanged(checked)
                }
            }
            ColumnLayout
            {
                id: intNode
                visible: CurrentDataInfo.type == Type.DataType_Int
                TextField{text: CurrentDataInfo.value !== undefined ? CurrentDataInfo.value : 0; enabled: false; validator: IntValidator{}}
                TextField{id: i; validator: IntValidator{}  onVisibleChanged: text = "" }
                RowLayout{
                    Button{text: "随机"; onClicked: {i.text = randomInt(-10000000 ,10000000)} }
                    Button{text: "发送"; onClicked: {ModelCluster.onIntChanged(Number(i.text))} }
                }
            }
            ColumnLayout
            {
                id: realNode
                visible: CurrentDataInfo.type == Type.DataType_Real
                TextField{text: CurrentDataInfo.value !== undefined ? CurrentDataInfo.value : 0.0; enabled: false; validator: DoubleValidator{decimals: 3} }
                TextField{id: r; validator: DoubleValidator{decimals: 3} onVisibleChanged: text = "" }
                RowLayout{
                    Button{text: "随机"; onClicked: {r.text = randomReal()} }
                    Button{text: "发送"; onClicked: {ModelCluster.onRealChanged(Number(r.text))} }
                }
            }
            ColumnLayout
            {
                id: stringNode
                visible: CurrentDataInfo.type == Type.DataType_String
                Row{
                    TextField{text: CurrentDataInfo.value !== undefined ? CurrentDataInfo.value : ""; enabled: false; width: 200 }
                }
                Row{
                    TextField{id: s; width: 200; onVisibleChanged: text = "" }
                }
                RowLayout{
                    Button{text: "随机"; onClicked: {s.text = tp.randomString() } }
                    Button{text: "发送"; onClicked: {ModelCluster.onStringChanged(s.text)} }
                }
            }
            ColumnLayout
            {
                id: enumNode
                visible: CurrentDataInfo.type == Type.DataType_Enum
                TextInput{ text:CurrentDataInfo.value !== undefined ? CurrentDataInfo.value : ""; readOnly: true ; selectByMouse: true}
                Row{
                    ComboBox
                    {
                        width: 200
                        id: e
                        model: CurrentDataInfo.enumValues
                    }
                    Button {text: "复制"; onClicked: ModelCluster.setClipboard(e.currentText)}
                }
                RowLayout{
                    Button{text: "随机"; onClicked: {e.currentIndex = randomInt(0, e.count - 1)} }
                    Button{text: "发送"; onClicked: {ModelCluster.onEnumChanged(e.currentText)} }
                }
            }
            ColumnLayout
            {
                id:imgNode
                width: parent.width
                height: parent.height
                FileDialog
                {
                    id:imgChoose
                    title: "配置数据加载路径"
                    selectFolder: false
                    modality: "WindowModal"
                    onAccepted: {
                        ModelCluster.onImageChanged(imgChoose.fileUrl);
                    }
                    Component.onCompleted: visible = false
                }

                visible: CurrentDataInfo.type == Type.DataType_Image
                Image{
                    id: img
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    fillMode: Image.PreserveAspectFit

                    onVisibleChanged: {
                        source = ""
                    }

                    Connections{
                        target: ImageProvider
                        onRefreshImage:
                        {
                            img.source = ""
                            img.source = "image://ImgProvider"
                        }
                    }
                }
                Row{
                    layoutDirection: Qt.RightToLeft
                    id:imgCmd
                    Button{text: "设置图片"; onClicked: imgChoose.open();}
                }
            }

            Column
            {
                id:listNode
                visible: CurrentDataInfo.type == Type.DataType_List
                width: parent.width; height: parent.height
                TableData{id:list; width: parent.width; height: parent.height}
            }

        }

    }

}
