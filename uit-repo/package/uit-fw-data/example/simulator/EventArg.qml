import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4
import UIT.Type 1.0

Rectangle {
    width: parent.width
    height: parent.height
    objectName: "eventArgs"

    function updateEventArg()
    {
        var titles = ModelCluster.argsTitles()
        var values = ModelCluster.argsValues()
        var types = ModelCluster.argsTypes()
        v0.valueType = Type.DataType_Null
        v1.valueType = Type.DataType_Null
        v2.valueType = Type.DataType_Null
        v3.valueType = Type.DataType_Null
        v4.valueType = Type.DataType_Null
        v5.valueType = Type.DataType_Null
        v6.valueType = Type.DataType_Null
        v7.valueType = Type.DataType_Null
        v8.valueType = Type.DataType_Null
        v9.valueType = Type.DataType_Null
        t0.text = ""
        t1.text = ""
        t2.text = ""
        t3.text = ""
        t4.text = ""
        t5.text = ""
        t6.text = ""
        t7.text = ""
        t8.text = ""
        t9.text = ""
        for(var i = 0; i < types.length; ++i)
        {
            var enumValues = ModelCluster.argsEnumValues(titles[i])
            if(i === 0) {v0.value = values[i]; v0.valueType = types[i]; v0.enumValues = enumValues; t0.text = titles[i]}
            else if(i === 1) {v1.value = values[i]; v1.valueType = types[i]; v1.enumValues = enumValues; t1.text = titles[i]}
            else if(i === 2) {v2.value = values[i]; v2.valueType = types[i]; v2.enumValues = enumValues; t2.text = titles[i]}
            else if(i === 3) {v3.value = values[i]; v3.valueType = types[i]; v3.enumValues = enumValues; t3.text = titles[i]}
            else if(i === 4) {v4.value = values[i]; v4.valueType = types[i]; v4.enumValues = enumValues; t4.text = titles[i]}
            else if(i === 5) {v5.value = values[i]; v5.valueType = types[i]; v5.enumValues = enumValues; t5.text = titles[i]}
            else if(i === 6) {v6.value = values[i]; v6.valueType = types[i]; v6.enumValues = enumValues; t6.text = titles[i]}
            else if(i === 7) {v7.value = values[i]; v7.valueType = types[i]; v7.enumValues = enumValues; t7.text = titles[i]}
            else if(i === 8) {v8.value = values[i]; v8.valueType = types[i]; v8.enumValues = enumValues; t8.text = titles[i]}
            else if(i === 9) {v9.value = values[i]; v9.valueType = types[i]; v9.enumValues = enumValues; t9.text = titles[i]}
        }
    }

    ColumnLayout{
        spacing: 20

        Row{
            Label{ text: "参数："}
        }

       Column {
            width: parent.width
            height: parent.height
            Row{
                id: roles
                height: 20
                spacing: 10
                Text{id: t0; width: 100; visible: v0.visible}
                Text{id: t1; width: 100; visible: v1.visible}
                Text{id: t2; width: 100; visible: v2.visible}
                Text{id: t3; width: 100; visible: v3.visible}
                Text{id: t4; width: 100; visible: v4.visible}
                Text{id: t5; width: 100; visible: v5.visible}
                Text{id: t6; width: 100; visible: v6.visible}
                Text{id: t7; width: 100; visible: v7.visible}
                Text{id: t8; width: 100; visible: v8.visible}
                Text{id: t9; width: 100; visible: v9.visible}
            }

            Row
            {
                height: roles.height
                spacing: roles.spacing
                ValueInput{id: v0; width: t0.width}
                ValueInput{id: v1; width: t1.width}
                ValueInput{id: v2; width: t2.width}
                ValueInput{id: v3; width: t3.width}
                ValueInput{id: v4; width: t4.width}
                ValueInput{id: v5; width: t5.width}
                ValueInput{id: v6; width: t6.width}
                ValueInput{id: v7; width: t7.width}
                ValueInput{id: v8; width: t8.width}
                ValueInput{id: v9; width: t9.width}
            }
        }
       ColumnLayout
       {
           Row{
               Button{
                   text: "发送";
                   onClicked: {
                       var titles = ModelCluster.argsTitles()
                       var values = []
                       for(var i = 0; i < titles.length; ++i)
                       {
                           var v
                           if(i === 0) v = v0.currentV
                           else if(i === 1) v = v1.currentV
                           else if(i === 2) v = v2.currentV
                           else if(i === 3) v = v3.currentV
                           else if(i === 4) v = v4.currentV
                           else if(i === 5) v = v5.currentV
                           else if(i === 6) v = v6.currentV
                           else if(i === 7) v = v7.currentV
                           else if(i === 8) v = v8.currentV
                           else if(i === 9) v = v9.currentV
                           values.push(v)
                       }
                       ModelCluster.postEvent(values)
                   }
               }
           }
       }
    }
}
