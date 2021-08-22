#include "Type.h"

using namespace uit::datafw;
QString Type::toString(Type::keyId k)
{
    switch (k) {
    case Up:            return QStringLiteral("Up");
    case Down:           return QStringLiteral("Down");
    case Left:           return QStringLiteral("Left");
    case Right:          return QStringLiteral("Right");
    case LeftUp:         return QStringLiteral("LeftUp");
    case RightUp:        return QStringLiteral("RightUp");
    case LeftDown:       return QStringLiteral("LeftDown");
    case RightDown:      return QStringLiteral("RightDown");
    case Clockwise:      return QStringLiteral("Clockwise");
    case AntiClockwise:  return QStringLiteral("AntiClockwise");
    case Ok:             return QStringLiteral("OK");
    case Back:           return QStringLiteral("Back");
    default:            return QStringLiteral("None");
    }
}

QString Type::toString(Type::keyState state)
{
    switch (state) {
    case Press:            return QStringLiteral("Press");
    case Cancel:           return QStringLiteral("Cancel");
    case LongPress:        return QStringLiteral("LongPress");
    case Release:          return QStringLiteral("Release");
    case Click:             return QStringLiteral("Click");
    default:                return QStringLiteral("Unknown");
    }
}

Type::DataType Type::translate(ValueType t)
{
    switch (t) {
    case nullValue:     return DataType_Null;
    case booleanValue:  return DataType_Bool;
    case intValue:      return DataType_Int;
    case realValue:     return DataType_Real;
    case stringValue:   return DataType_String;
    case enumValue:     return DataType_Enum;
    case imageValue:    return DataType_Image;
    case listValue:     return DataType_List;
    case objectValue:   return DataType_Object;
    default:            return DataType_Null;
    }
}

VKeyId Type::toVKey(Type::keyId k)
{
    switch (k) {
    case Up:            return vkeyUp;
    case Down:           return vkeyDown;
    case Left:           return vkeyLeft;
    case Right:          return vkeyRight;
    case LeftUp:         return vkeyLeftUp;
    case RightUp:        return vkeyRightUp;
    case LeftDown:       return vkeyLeftDown;
    case RightDown:      return vkeyRightDown;
    case Clockwise:      return vkeyClockwise;
    case AntiClockwise:  return vkeyAntiClockwise;
    case Ok:             return vkeyOK;
    case Back:           return vkeyBack;
    default:            return vkeyNone;
    }
}

VKeyState Type::toVkeyState(Type::keyState state)
{
    switch (state) {
    case Press:            return vkeyStatePress;
    case Cancel:           return vkeyStateCancel;
    case LongPress:        return vkeyStateLongPress;
    case Release:          return vkeyStateRelease;
    case Click:             return vkeyStateClick;
    default:                return vkeyStateUnknown;
    }
}
