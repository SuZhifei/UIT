import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 1.4

Rectangle
{
    objectName: "logPanel"
    function log(str)
    {
        txt.append(str)
    }

    TextArea
    {
        id: txt
        width: parent.width
        height: parent.height
        activeFocusOnPress: false

        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        readOnly: true
    }

}




