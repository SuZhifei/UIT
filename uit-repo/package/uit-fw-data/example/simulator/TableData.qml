import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2
import UIT.Type 1.0

Item {
    Component
    {
         id: columnComponent
         TableViewColumn{width: 100 }
    }

    Column{
        width: parent.width
        height: parent.height

        ListModel
        {
            id:m
        }
        TableView
        {
            objectName: "tbView"
            id: table
            width: parent.width; height: parent.height - command.height
            model: m

            onCurrentRowChanged:
            {
                command.updateRowInfo()
            }

            function updateHeader(headers)
            {
                var count = table.columnCount
                for(var i = 0; i < count; ++i)
                    table.removeColumn(0);
                for(var j = 0; j < headers.length; j++)
                {
                    var role  = headers[j]
                    table.addColumn(columnComponent.createObject(table, { "role": role, "title": role}))
                }
            }
            function resize(size)
            {
                var oldSize = m.count
                var dif = size - oldSize
                if(dif > 0)
                {
                    for(var i = 0; i < dif; ++i)
                        m.append({});

                    var titles = ModelCluster.rowTitles()
                    var values = ModelCluster.rowValues()
                    for(var x = oldSize; x < m.count; ++x)
                    {
                        for(var y = 0; y < titles.length; ++y)
                        {
                            var key = titles[y]
                            var value = values[y]
                            m.setProperty(x, key, value)
                        }
                    }

                }
                else
                {
                    for(var j = 0; j < (-dif); ++j)
                        m.remove(m.count - 1)
                }
            }
        }
        Column
        {
            id: command
            objectName: "rowInfo"
            spacing: 10
            function updateRowInfo()
            {
                var titles = ModelCluster.rowTitles()
                var values = ModelCluster.rowValues()
                var types = ModelCluster.rowTypes()
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
                for(var i = 0; i < types.length; ++i)
                {
                    var enumValues = ModelCluster.rowEnumValues(titles[i])
                    if(i === 0) {v0.value = values[i]; v0.valueType = types[i]; v0.enumValues = enumValues}
                    else if(i === 1) {v1.value = values[i]; v1.valueType = types[i]; v1.enumValues = enumValues}
                    else if(i === 2) {v2.value = values[i]; v2.valueType = types[i]; v2.enumValues = enumValues}
                    else if(i === 3) {v3.value = values[i]; v3.valueType = types[i]; v3.enumValues = enumValues}
                    else if(i === 4) {v4.value = values[i]; v4.valueType = types[i]; v4.enumValues = enumValues}
                    else if(i === 5) {v5.value = values[i]; v5.valueType = types[i]; v5.enumValues = enumValues}
                    else if(i === 6) {v6.value = values[i]; v6.valueType = types[i]; v6.enumValues = enumValues}
                    else if(i === 7) {v7.value = values[i]; v7.valueType = types[i]; v7.enumValues = enumValues}
                    else if(i === 8) {v8.value = values[i]; v8.valueType = types[i]; v8.enumValues = enumValues}
                    else if(i === 9) {v9.value = values[i]; v9.valueType = types[i]; v9.enumValues = enumValues}
                }
            }

            GroupBox{
                title: "第" + table.currentRow + "行"
                width: table.width
                visible: table.currentRow != -1
                Column{
                Row
                {
                    width: parent.width
                    height: 30
                    id:inputs
                    ValueInput{ id: v0; width: 100}
                    ValueInput{id: v1; width: 100}
                    ValueInput{id: v2; width: 100}
                    ValueInput{id: v3; width: 100}
                    ValueInput{id: v4; width: 100}
                    ValueInput{id: v5; width: 100}
                    ValueInput{id: v6; width: 100}
                    ValueInput{id: v7; width: 100}
                    ValueInput{id: v8; width: 100}
                    ValueInput{id: v9; width: 100}
                }
                Row
                {
                    id: send
                    //anchors.left: inputs.right
                    Button{
                        text: "修改"
                        onClicked: {
                            var titles = ModelCluster.rowTitles()
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
                                m.setProperty(table.currentRow, titles[i], v)
                                values.push(v)
                            }
                            ModelCluster.onUpdateListItems(table.currentRow, values)
                        }
                    }
                    Button{text:"删除"
                        onClicked:{
                            m.remove(table.currentRow)
                            ModelCluster.onRemoveListItem(table.currentRow)
                        }
                    }
                    Button{text:"插入"
                        onClicked:{
                            m.insert(table.currentRow, {})
                            var titles = ModelCluster.rowTitles()
                            var values = ModelCluster.rowValues()
                            for(var y = 0; y < titles.length; ++y)
                            {
                                var key = titles[y]
                                var value = values[y]
                                m.setProperty(x, key, value)
                            }
                            ModelCluster.onInsertListItem(table.currentRow)
                        }
                    }
                }}
            }

            GroupBox{
                width: table.width
                title: "列表属性设置"
                Column{
                    spacing: 10
                    Row
                    {
                        spacing: 20
                        Row
                        {
                            Button{
                                text: "设置项数"
                                onClicked: {ModelCluster.onResizeList(Number(count.text))}
                            }
                            TextField{id: count; text: "5"; width: 40;validator: IntValidator{bottom: 0; top: 5000}}
                        }

                        Row{
                            Button{
                                text: "滚动至"
                                onClicked: {ModelCluster.onUpdateListViewport(Number(viewport.text))}
                            }
                            TextField{id: viewport; width: 40; text: "0"; validator: IntValidator{bottom: 0; top: 5000}}
                        }

                        Row{
                            Button{
                                text: "选中项"
                                onClicked: {ModelCluster.onCheckListItem(Number(check.text))}
                            }
                            TextField{id: check; width: 40; text: "0"; validator: IntValidator{bottom: 0; top: 5000}}
                        }
                        Row{
                            Button{
                                text: "焦点项"
                                onClicked: {ModelCluster.onFocusListItem(Number(fs.text))}
                            }
                            TextField{id: fs; width: 40; text: "0"; validator: IntValidator{bottom: 0; top: 5000}}
                        }
                    }
                    Row{
                        spacing: 20
                        Row{
                            Button{
                                text: "添加标签"
                                onClicked: {ModelCluster.onAddListLabel(Number(index.text), lb.text)}
                            }
                            TextField{id: lb; width: 40; text: "A"}
                            Label{text: "到第"; verticalAlignment: Qt.AlignVCenter; height: index.height}
                            TextField{id: index; width: 40; text: "0"; validator: IntValidator{bottom: 0; top: 5000}}
                            Label{text: "项"; verticalAlignment: Qt.AlignVCenter; height: index.height}
                        }
                        Row{
                            Button{text: "删除标签"+lb.text
                                onClicked: ModelCluster.onRemoveListLabel(lb.text)
                            }
                        }
                    }
                    }
            }
        }
    }

}
