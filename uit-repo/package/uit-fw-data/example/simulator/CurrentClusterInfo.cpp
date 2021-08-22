#include "CurrentClusterInfo.h"

Type::RootType CurrentClusterInfo::type()
{
    return m_type;
}

void CurrentClusterInfo::settype(Type::RootType type)
{
    if(type != m_type)
    {
        m_type = type;
        emit typeChanged();
    }
}

CurrentClusterInfo *CurrentClusterInfo::current()
{
    static CurrentClusterInfo *g_sInstance = new CurrentClusterInfo();
    return g_sInstance;
}
