#include "CurrentEventInfo.h"

CurrentEventInfo *CurrentEventInfo::current()
{
    static CurrentEventInfo *g_sInstance = new CurrentEventInfo();
    return g_sInstance;
}

QString CurrentEventInfo::path()
{
    return m_path;
}

void CurrentEventInfo::setpath(const QString &path)
{
    if(path != m_path)
    {
        m_path = path;
        emit pathChanged();
    }
}

Type::EventType CurrentEventInfo::type()
{
    return m_type;
}

void CurrentEventInfo::settype(Type::EventType type)
{
    if(type != m_type)
    {
        m_type = type;
        emit typeChanged();
    }
}
