#include "datafw/Type.h"

using namespace uit::datafw;
std::string uit::datafw::toString(ValueType type)
{
	switch (type)
	{
	case nullValue:				return "nullValue";
	case booleanValue:			return "booleanValue";
	case intValue:				return "intValue";
	case enumValue:				return "enumValue";
	case realValue:				return "realValue";
	case stringValue:			return "stringValue";
	case imageValue:			return "imageValue";
	case listValue:				return "listValue";
	case objectValue:			return "objectValue";
	default:					return "";
	}
}

std::string uit::datafw::toString(ImageFormat type)
{
	switch (type)
	{
	case invalidFormat:			return "invalidFormat";
	case rgb555Format:			return "rgb555Format";
	case rgb565Format:			return "rgb565Format";
	case rgb888Format:			return "rgb888Format";
	case rgb8888Format:			return "rgb8888Format";
	default:					return "";
	}
}

std::string uit::datafw::toString(VKeyId id)
{
	switch (id)
	{
	case vkeyNone:				return "vkeyNone";
	case vkeyUp:				return "vkeyUp";
	case vkeyDown:				return "vkeyDown";
	case vkeyLeft:				return "vkeyLeft";
	case vkeyRight:				return "vkeyRight";
	case vkeyLeftUp:			return "vkeyLeftUp";
	case vkeyRightUp:			return "vkeyRightUp";
	case vkeyLeftDown:			return "vkeyLeftDown";
	case vkeyRightDown:			return "vkeyRightDown";
	case vkeyClockwise:			return "vkeyClockwise";
	case vkeyAntiClockwise:		return "vkeyAntiClockwise";
	case vkeyOK:				return "vkeyOK";
	case vkeyBack:				return "vkeyBack";
	default:					return "";
	}
}

std::string uit::datafw::toString(VKeyState state)
{
	switch (state)
	{
	case vkeyStateUnknown:		return "vkeyStateUnknown";
	case vkeyStatePress:		return "vkeyStatePress";
	case vkeyStateCancel:		return "vkeyStateCancel";
	case vkeyStateLongPress:	return "vkeyStateLongPress";
	case vkeyStateRelease:		return "vkeyStateRelease";
	case vkeyStateClick:		return "vkeyStateClick";
	default:					return "";
	}
}