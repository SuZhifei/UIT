#ifndef CURRENTEVENTINFO_H
#define CURRENTEVENTINFO_H

#include <QObject>
#include "Type.h"

class CurrentEventInfo : public QObject
{
    Q_OBJECT

public:
    Q_PROPERTY(QString path READ path WRITE setpath NOTIFY pathChanged)
    Q_PROPERTY(Type::EventType type READ type WRITE settype NOTIFY typeChanged)

    QString path();
    void setpath(const QString &path);

    Type::EventType type();
    void settype(Type::EventType type);

    static CurrentEventInfo *current();

signals:
    void pathChanged();
    void typeChanged();

private:
    QString                 m_path;
    Type::EventType         m_type;
};

#endif // CURRENTEVENTINFO_H
