import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.0
import UIT.Type 1.0

Rectangle {
    property int valueType: Type.DataType_Null
    property string enumValues:""
    property var value
    property var currentV
    width: parent.width
    height: parent.height
    visible: valueType !== Type.DataType_Null

    ListModel
    {
        id:m
        ListElement{
            text: ""
        }
    }

    CheckBox{
        id: b
        checked: value !== undefined ? value : false
        text: checked ? "true" : "false"
        visible: valueType == Type.DataType_Bool
        width: 100
        onCheckedChanged: currentV = checked
        onVisibleChanged: currentV = checked
    }
    TextField{
        id: i
        text: value !== undefined ? value : 0
        visible: valueType == Type.DataType_Int
        width: 100
        validator: IntValidator{}
        onTextChanged: currentV = text
        onVisibleChanged: currentV = text
    }
    TextField{
        id: r
        text:
        {
            if(value === undefined)
                return 0.0
            else if(value === 0.0)
                return value.toFixed(1)
            else
                return value
        }
        visible: valueType == Type.DataType_Real
        width: 100
        validator: DoubleValidator{decimals: 3}
        onTextChanged: currentV = text
        onVisibleChanged: currentV = text
    }
    TextField{
        id: s
        text: value !== undefined ? value : ""
        visible: valueType == Type.DataType_String
        width: 100
        onTextChanged: currentV = text
        onVisibleChanged: currentV = text
    }
    ComboBox{
        id: e; visible: valueType == Type.DataType_Enum
        width: 100
        model: m
        onCurrentIndexChanged: currentV = currentText
        onVisibleChanged: currentV = currentText
    }
    Image{id:img; visible: valueType == Type.DataType_Image;width: 100}

    onEnumValuesChanged:
    {
        m.clear()
        var items = enumValues.split("|")
        for(var i = 0; i < items.length; ++i)
            m.append({"text" : items[i]})
        currentV = items[0]
    }
}
