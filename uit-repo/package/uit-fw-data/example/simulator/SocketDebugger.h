#ifndef SOCKETDEBUGGER_H
#define SOCKETDEBUGGER_H

#include <QObject>
#include <QTcpSocket>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Array.h>

class SocketDebugger : public QObject
{
    Q_OBJECT
public:
    static SocketDebugger *current();

    void setBoolean(const std::string &path, bool b);
    void setInteger(const std::string &path, int n);
    void setReal(const std::string &path, double d);
    void setString(const std::string &path, const std::string &str);
    void setEnumStringValue(const std::string &path, const std::string &str);
    void resizeList(const std::string &path, int size);
    void insertListItems(const std::string &path, int beg, int count);
    void removeListItems(const std::string &path, int beg, int count);
    void updateListItems(const std::string &path, const std::set<uint32_t> &indexs, Poco::JSON::Array::Ptr items);
    void updateListViewport(const std::string &path, int first);
    void checkListItem(const std::string &path, int index);
    void focusListItem(const std::string &path, int index);
    void addListLabel(const std::string &path, int index, const std::string &label);
    void removeListLabel(const std::string &path, const std::string &label);
    void vkey(int key, int state);

    void postEvent(const std::string &path, Poco::JSON::Object::Ptr args);

signals:

public slots:
    bool connectToHost(const QString &ip, int port);
    void quit();


private:
    QTcpSocket  m_socket;
};

#endif // SOCKETDEBUGGER_H
