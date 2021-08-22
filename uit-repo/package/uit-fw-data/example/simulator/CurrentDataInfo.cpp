#include "CurrentDataInfo.h"

using namespace uit::datafw;
CurrentDataInfo *CurrentDataInfo::g_sInstance = NULL;
QString CurrentDataInfo::path()
{
    return m_path;
}

void CurrentDataInfo::setpath(const QString &path)
{
    if(path != m_path)
    {
        m_path = path;
        emit pathChanged();
    }
}

Type::DataType CurrentDataInfo::type()
{
    return m_type;
}

void CurrentDataInfo::settype(Type::DataType type)
{
    if(type != m_type)
    {
        m_type = type;
        emit typeChanged();
    }
}

QString CurrentDataInfo::extraType()
{
    return m_extraType;
}

void CurrentDataInfo::setextraType(const QString &extraType)
{
    if(extraType != m_extraType)
    {
        m_extraType = extraType;
        emit extraTypeChanged();
    }
}

QVariant CurrentDataInfo::value()
{
    return m_value;
}

void CurrentDataInfo::setvalue(const QVariant &value)
{
    if(value != m_value)
    {
        m_value = value;
        emit valueChanged();
    }
}

QVariantList CurrentDataInfo::enumValues()
{
    return m_enumValues;
}

void CurrentDataInfo::setEnumValues(const QVariantList &enumValues)
{
    if(enumValues != m_enumValues)
    {
        m_enumValues = enumValues;
        emit enumValuesChanged();
    }
}

CurrentDataInfo *CurrentDataInfo::current()
{
    if(!g_sInstance)
        g_sInstance = new CurrentDataInfo();
    return g_sInstance;
}
