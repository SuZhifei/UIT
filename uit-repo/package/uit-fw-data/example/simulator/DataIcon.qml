import QtQuick 2.0
import UIT.Type 1.0

Rectangle {

    property int valueType: Type.DataType_Null

    id:root
    onValueTypeChanged:
    {
        switch(valueType)
        {
        case Type.DataType_Null: root.state = "none"; break;
        case Type.DataType_Bool: root.state = "bool"; break;
        case Type.DataType_Int: root.state = "int"; break;
        case Type.DataType_Real: root.state = "real"; break;
        case Type.DataType_String: root.state = "string"; break;
        case Type.DataType_Enum: root.state = "enum"; break;
        case Type.DataType_Image: root.state = "image"; break;
        case Type.DataType_List: root.state = "list"; break;
        case Type.DataType_Object: root.state = "object"; break;
        default: root.state = "none"
        }
    }

    Image {
        id: icon
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
    }

    states: [
        State {
            name: "none"
            PropertyChanges {
                target: icon
                source:""
            }
        },
        State {
            name: "bool"
            PropertyChanges {
                target: icon
                source:"images/bool.png"
            }
        },
        State {
            name: "int"
            PropertyChanges {
                target: icon
                source:"images/int.png"
            }
        },
        State {
            name: "real"
            PropertyChanges {
                target: icon
                source:"images/real.png"
            }
        },
        State {
            name: "string"
            PropertyChanges {
                target: icon
                source:"images/string.png"
            }
        },
        State {
            name: "enum"
            PropertyChanges {
                target: icon
                source:"images/enum.png"
            }
        },
        State {
            name: "image"
            PropertyChanges {
                target: icon
                source:"images/image.png"
            }
        },
        State {
            name: "list"
            PropertyChanges {
                target: icon
                source:"images/list.png"
            }
        },
        State {
            name: "object"
            PropertyChanges {
                target: icon
                source:"images/object.png"
            }
        }
    ]

}
