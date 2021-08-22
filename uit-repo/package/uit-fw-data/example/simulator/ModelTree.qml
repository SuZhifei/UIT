import QtQuick 2.0
import QtQuick.Controls 1.4

Rectangle
{
    objectName: "tree"
    width : parent.width
    height: parent.height

    property string searchNode: ""

    function search(rootType, names)
    {
        var currentDataIndex = tv.model.index(rootType, 0)
        console.log(tv.model.data(currentDataIndex))
        var nameIndex = 0;
        while(nameIndex < names.length)
        {
            var count = tv.model.rowCount(currentDataIndex)
            tv.expand(currentDataIndex)

            for(var i = 0; i < count; ++i)
            {
                var index = tv.model.index(i, 0, currentDataIndex)
                var nodeName = tv.model.data(index)
                if(names.indexOf(nodeName) !== -1)
                {
                    currentDataIndex = index
                    break
                }
            }
            ++nameIndex
        }
    }

    TreeView
    {
        id:tv
        model: TreeModel.model()
        width : parent.width
        height: parent.height
        headerVisible: false
        itemDelegate: Item{
            Text {
                anchors.verticalCenter: parent.verticalCenter
                color: {
                    if(text === searchNode)
                        return "red"
                    else
                        return styleData.textColor
                }

                elide: styleData.elideMode
                text: styleData.value
            }
        }

        TableViewColumn {
            role : "name"
            width : tv.width
            onTitleChanged:
            {
                role = "name"
            }
        }

        onDoubleClicked: {
            if(isExpanded(index))   tv.collapse(index)
            else tv.expand(index)
        }
        onCurrentIndexChanged: {
            var currentPath = ""
            var curIndex = tv.currentIndex
            while(model.parent(curIndex).valid)
            {
                currentPath = model.data(curIndex).toString() + "." + currentPath
                curIndex = model.parent(curIndex)
            }

            if(currentPath[currentPath.length - 1] === '.')
                currentPath = currentPath.substring(0, currentPath.length - 1)

            var rootNode = model.data(curIndex) === undefined ? "" : model.data(curIndex).toString()
            if(rootNode === "Data")
                ModelCluster.onSelectDataChanged(currentPath)
            else if(rootNode === "Event")
                ModelCluster.onSelectEventChanged(currentPath)
            else if(rootNode === "Enum")
                ModelCluster.onSelectEnumChanged(currentPath)
            else if(rootNode === "Template")
                ModelCluster.onSelectTempalteChanged(currentPath)
        }

        onHighlightOnFocusChanged:
        {
            console.log("dddddddddd")
        }

    }


}
