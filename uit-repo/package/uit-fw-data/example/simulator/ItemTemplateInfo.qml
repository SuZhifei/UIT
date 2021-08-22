import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2
import UIT.Type 1.0

Rectangle {
    id:root
    width: parent.width
    height: parent.height

    ListModel{
        id:m
    }

    GroupBox
    {
        id: eInfo
        width: root.width - 5
        height: root.height - 5
        title: "列表项模板"

        ColumnLayout{
            spacing: 10
            Row{
            Image{
                width:100
                height: 100
                fillMode: Image.PreserveAspectFit
                source: "images/Template.png"
            }}

            ColumnLayout
            {
                Label{ text: "路径：" }
                TextInput{ id:ti; text: CurrentItemTemplateInfo.path ; readOnly: true ; selectByMouse: true}
            }

            Row{
                Label{text: "字段："}
            }

            Row{
                ListView{
                    width: eInfo.width
                    height: 400
                    model: CurrentItemTemplateInfo.values
                    delegate:
                        TextInput {text: modelData; readOnly: true ; selectByMouse: true}


                }
            }
        }

    }

}
