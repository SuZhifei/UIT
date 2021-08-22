import QtQuick 2.0
import UIT.Type 1.0

Rectangle {


    DataInfo{visible: CurrentClusterInfo.type == Type.RootType_Data}
    EventInfo{visible: CurrentClusterInfo.type == Type.RootType_Event}
    EnumDefinitionInfo{visible: CurrentClusterInfo.type == Type.RootType_EnumDefinition}
    ItemTemplateInfo{visible: CurrentClusterInfo.type == Type.RootType_ItemTemplate}
}
