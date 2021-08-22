#include "CurrentEnumDefinitionInfo.h"

CurrentEnumDefinitionInfo *CurrentEnumDefinitionInfo::current()
{
    static CurrentEnumDefinitionInfo *g_sInstance = new CurrentEnumDefinitionInfo();
    return g_sInstance;
}

QString CurrentEnumDefinitionInfo::path()
{
    return m_path;
}

void CurrentEnumDefinitionInfo::setpath(const QString &path)
{
    if(path != m_path)
    {
        m_path = path;
        emit pathChanged();
    }
}

QVariantList CurrentEnumDefinitionInfo::values()
{
    return m_values;
}

void CurrentEnumDefinitionInfo::setvalues(const QVariantList &values)
{
    if(values != m_values)
    {
        m_values = values;
        emit valuesChanged();
    }
}
