#ifndef CURRENTDATAINFO_H
#define CURRENTDATAINFO_H

#include <QObject>
#include <QVariant>
#include "uit/datafw/Type.h"
#include "Type.h"

class CurrentDataInfo : public QObject
{
    Q_OBJECT

public:
    Q_PROPERTY(QString path READ path WRITE setpath NOTIFY pathChanged)
    Q_PROPERTY(Type::DataType type READ type WRITE settype NOTIFY typeChanged)
    Q_PROPERTY(QString extraType READ extraType WRITE setextraType NOTIFY extraTypeChanged)
    Q_PROPERTY(QVariant value READ value WRITE setvalue NOTIFY valueChanged)
    Q_PROPERTY(QVariantList enumValues READ enumValues WRITE setEnumValues NOTIFY enumValuesChanged)

    QString path();
    void setpath(const QString &path);

    Type::DataType type();
    void settype(Type::DataType type);

    QString extraType();
    void setextraType(const QString &extraType);

    QVariant value();
    void setvalue(const QVariant &value);

    QVariantList enumValues();
    void setEnumValues(const QVariantList &enumValues);

signals:
    void pathChanged();
    void typeChanged();
    void extraTypeChanged();
    void valueChanged();
    void enumValuesChanged();

public:
    static CurrentDataInfo *current();

private:
    QString                 m_path;
    Type::DataType          m_type;
    QString                 m_extraType;
    QVariant                m_value;
    QVariantList            m_enumValues;
    static CurrentDataInfo  *g_sInstance;
};

#endif // CURRENTDATAINFO_H
