import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.0

Rectangle
{
    property int menu_and_sb_space: 40
    property int space: 5
    GridLayout
    {
        width: parent.width
        height: parent.height

        Column{
            width: 300
            Layout.fillHeight: true
            TextField{
                width: parent.width
                height: 20
                //validator: RegExpValidator{regExp: /[a-zA-Z.]+/}
                text: "搜索"
                onFocusChanged: {
                    if(!focus && text === "")
                        text = "搜索"
                    else if(focus && text === "搜索")
                        text = ""
                }
                onTextChanged:
                {
                       ModelCluster.search(text)
                }
            }
            ModelTree
            {
                width: parent.width
                Layout.fillHeight: true
                id: tree
            }
        }


        CurrentInfo
        {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        LogPanel
        {
            width: 300
            Layout.fillHeight: true
        }

    }

}
