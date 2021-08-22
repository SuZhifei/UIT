#include "SocketDebugger.h"
#include "ModelCluster.h"

SocketDebugger *SocketDebugger::current()
{
    static SocketDebugger *g_sInstance = new SocketDebugger();

    return g_sInstance;
}

bool SocketDebugger::connectToHost(const QString &ip, int port)
{
    m_socket.connectToHost(ip, port);
    bool b = m_socket.waitForConnected(3 * 1000);
    if(b)
    {
        QString s = QStringLiteral("【socket debugger】：连接成功，ip:%1, port:%2").arg(ip).arg(QString::number(port));
        ModelCluster::current()->log(s);
    }
    else
    {
        QString s = QStringLiteral("【socket debugger】：连接失败，ip:%1, port:%2").arg(ip).arg(QString::number(port));
        ModelCluster::current()->log(s);
    }
    return b;
}

void SocketDebugger::setBoolean(const std::string &path, bool b)
{
    Poco::JSON::Object root;
    Poco::JSON::Object content;
    content.set("path", path);
    content.set("value", b);
    root.set("setBoolean", content);
    if(m_socket.isValid())
        m_socket.write(Poco::Dynamic::Var(root).toString().data());
}

void SocketDebugger::setInteger(const std::string &path, int n)
{
    Poco::JSON::Object root;
    Poco::JSON::Object content;
    content.set("path", path);
    content.set("value", n);
    root.set("setInteger", content);
    if(m_socket.isValid())
        m_socket.write(Poco::Dynamic::Var(root).toString().data());
}

void SocketDebugger::setReal(const std::string &path, double d)
{
    Poco::JSON::Object root;
    Poco::JSON::Object content;
    content.set("path", path);
    content.set("value", d);
    root.set("setReal", content);
    if(m_socket.isValid())
        m_socket.write(Poco::Dynamic::Var(root).toString().data());
}

void SocketDebugger::setString(const std::string &path, const std::string &str)
{
    Poco::JSON::Object root;
    Poco::JSON::Object content;
    content.set("path", path);
    content.set("value", str);
    root.set("setString", content);
    if(m_socket.isValid())
        m_socket.write(Poco::Dynamic::Var(root).toString().data());
}

void SocketDebugger::setEnumStringValue(const std::string &path, const std::string &str)
{
    Poco::JSON::Object root;
    Poco::JSON::Object content;
    content.set("path", path);
    content.set("value", str);
    root.set("setEnumStringValue", content);
    if(m_socket.isValid())
        m_socket.write(Poco::Dynamic::Var(root).toString().data());
}

void SocketDebugger::resizeList(const std::string &path, int size)
{
    Poco::JSON::Object root;
    Poco::JSON::Object content;
    content.set("path", path);
    content.set("size", size);
    root.set("resizeList", content);
    if(m_socket.isValid())
        m_socket.write(Poco::Dynamic::Var(root).toString().data());
}

void SocketDebugger::insertListItems(const std::string &path, int beg, int count)
{
    Poco::JSON::Object root;
    Poco::JSON::Object content;
    content.set("path", path);
    content.set("beg", beg);
    content.set("count", count);
    root.set("insertListItems", content);
    if(m_socket.isValid())
        m_socket.write(Poco::Dynamic::Var(root).toString().data());
}

void SocketDebugger::removeListItems(const std::string &path, int beg, int count)
{
    Poco::JSON::Object root;
    Poco::JSON::Object content;
    content.set("path", path);
    content.set("beg", beg);
    content.set("count", count);
    root.set("removeListItems", content);
    if(m_socket.isValid())
        m_socket.write(Poco::Dynamic::Var(root).toString().data());
}

void SocketDebugger::updateListItems(const std::string &path, const std::set<uint32_t> &indexs, Poco::JSON::Array::Ptr items)
{
    Poco::JSON::Object root;
    Poco::JSON::Object content;
    Poco::JSON::Array ids;
    for(auto iter = indexs.begin(); iter != indexs.end(); ++iter)
        ids.add(*iter);
    content.set("path", path);
    content.set("indexs", ids);
    content.set("items", items);
    root.set("updateListItems", content);
    if(m_socket.isValid())
        m_socket.write(Poco::Dynamic::Var(root).toString().data());
}

void SocketDebugger::updateListViewport(const std::string &path, int first)
{
    Poco::JSON::Object root;
    Poco::JSON::Object content;
    content.set("path", path);
    content.set("first", first);
    root.set("updateListViewport", content);
    if(m_socket.isValid())
        m_socket.write(Poco::Dynamic::Var(root).toString().data());
}

void SocketDebugger::checkListItem(const std::string &path, int index)
{
    Poco::JSON::Object root;
    Poco::JSON::Object content;
    content.set("path", path);
    content.set("index", index);
    root.set("checkListItem", content);
    if(m_socket.isValid())
        m_socket.write(Poco::Dynamic::Var(root).toString().data());
}

void SocketDebugger::focusListItem(const std::string &path, int index)
{
    Poco::JSON::Object root;
    Poco::JSON::Object content;
    content.set("path", path);
    content.set("index", index);
    root.set("focusListItem", content);
    if(m_socket.isValid())
        m_socket.write(Poco::Dynamic::Var(root).toString().data());
}

void SocketDebugger::addListLabel(const std::string &path, int index, const std::string &label)
{
    Poco::JSON::Object root;
    Poco::JSON::Object content;
    content.set("path", path);
    content.set("index", index);
    content.set("label", label);
    root.set("addListLabel", content);
    if(m_socket.isValid())
        m_socket.write(Poco::Dynamic::Var(root).toString().data());
}

void SocketDebugger::removeListLabel(const std::string &path, const std::string &label)
{
    Poco::JSON::Object root;
    Poco::JSON::Object content;
    content.set("path", path);
    content.set("label", label);
    root.set("removeListLabel", content);
    if(m_socket.isValid())
        m_socket.write(Poco::Dynamic::Var(root).toString().data());
}

void SocketDebugger::vkey(int key, int state)
{
    Poco::JSON::Object root;
    Poco::JSON::Object content;
    content.set("key", key);
    content.set("state", state);
    root.set("vkey", content);
    if(m_socket.isValid())
        m_socket.write(Poco::Dynamic::Var(root).toString().data());
}

void SocketDebugger::postEvent(const std::string &path, Poco::JSON::Object::Ptr args)
{
    Poco::JSON::Object root;
    Poco::JSON::Object content;
    content.set("path", path);
    content.set("args", args);
    root.set("postEvent", content);
    if(m_socket.isValid())
        m_socket.write(Poco::Dynamic::Var(root).toString().data());
}

void SocketDebugger::quit()
{
    Poco::JSON::Object root;
    Poco::JSON::Object content;
    content.set("path", 1);
    content.set("args", 2);
    root.set("postEvent", content);
    std::string s = Poco::Dynamic::Var(root).toString();
    if(m_socket.isValid())
        m_socket.write(s.data());
}
