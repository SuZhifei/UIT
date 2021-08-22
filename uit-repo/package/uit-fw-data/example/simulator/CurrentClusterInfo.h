#ifndef CURRENTCLUSTERINFO_H
#define CURRENTCLUSTERINFO_H

#include <QObject>
#include "Type.h"

class CurrentClusterInfo : public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(Type::RootType type READ type WRITE settype NOTIFY typeChanged)

    Type::RootType type();
    void settype(Type::RootType type);

    static CurrentClusterInfo *current();

signals:
    void typeChanged();

public slots:


private:
    Type::RootType          m_type;
};

#endif // CURRENTCLUSTERINFO_H
