#ifndef TYPE_H
#define TYPE_H

#include <QObject>
#include <QTime>
#include "uit/datafw/Type.h"

using namespace uit::datafw;
class Type : public QObject
{
    Q_OBJECT
    Q_ENUMS(RootType)
    Q_ENUMS(DataType)
    Q_ENUMS(EventType)
    Q_ENUMS(EnumDefinitionType)
    Q_ENUMS(ItemTemplateType)
    Q_ENUMS(keyId)
    Q_ENUMS(keyState)

public:
    enum RootType
    {
        RootType_None,
        RootType_Data,
        RootType_Event,
        RootType_EnumDefinition,
        RootType_ItemTemplate,
    };

    enum DataType
    {
        DataType_Null,
        DataType_Bool,
        DataType_Int,
        DataType_Real,
        DataType_String,
        DataType_Enum,
        DataType_Image,
        DataType_List,
        DataType_Object,
    };

    enum EventType
    {
        EventType_Null,
        EventType_Object,
        EventType_Event,

    };
    enum EnumDefinitionType
    {
        EnumDefinitionType_Null,
        EnumDefinitionType_Object,
        EnumDefinitionType_Event,

    };
    enum ItemTemplateType
    {
        ItemTemplateType_Null,
        ItemTemplateType_Object,
        ItemTemplateType_Event,

    };

    enum keyId
    {
        None,
        Up,
        Down,
        Left,
        Right,
        LeftUp,
        RightUp,
        LeftDown,
        RightDown,
        Clockwise,
        AntiClockwise,
        Ok,
        Back,
    };
    enum keyState
    {
        Unknown,
        Press,
        Cancel,
        LongPress,
        Release,
        Click
    };

    static QString toString(keyId k);
    static QString toString(keyState state);

    static DataType translate(ValueType t);
    static VKeyId toVKey(keyId k);
    static VKeyState toVkeyState(keyState state);

public slots:
    QString randomString()
    {
        int max = 20;
        QString tmp = QString("0123456789abcedfghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWZYZ~!@#$%^&*()_+-=[]{}|?,.<>");
        QString str;
        QTime t;
        t= QTime::currentTime();
        qsrand(t.msec() + t.second() * 1000);
        for(int i = 0; i < max; i++) {
            int ir = qrand()%tmp.length();
            str[i] = tmp.at(ir);
        }
        return str;
    }
};

#endif // TYPE_H
