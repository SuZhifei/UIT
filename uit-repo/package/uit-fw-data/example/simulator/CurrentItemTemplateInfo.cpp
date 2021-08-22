#include "CurrentItemTemplateInfo.h"


CurrentItemTemplateInfo *CurrentItemTemplateInfo::current()
{
    static CurrentItemTemplateInfo *g_sInstance = new CurrentItemTemplateInfo();
    return g_sInstance;
}

QString CurrentItemTemplateInfo::path()
{
    return m_path;
}

void CurrentItemTemplateInfo::setpath(const QString &path)
{
    if(path != m_path)
    {
        m_path = path;
        emit pathChanged();
    }
}

QVariantList CurrentItemTemplateInfo::values()
{
    return m_values;
}

void CurrentItemTemplateInfo::setvalues(const QVariantList &values)
{
    if(values != m_values)
    {
        m_values = values;
        emit valuesChanged();
    }
}
