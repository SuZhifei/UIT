#ifndef CURRENTENUMDEFINITIONINFO_H
#define CURRENTENUMDEFINITIONINFO_H

#include <QObject>
#include <QVariant>

class CurrentEnumDefinitionInfo : public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(QString path READ path WRITE setpath NOTIFY pathChanged)
    Q_PROPERTY(QVariantList values READ values WRITE setvalues NOTIFY valuesChanged)
    QString path();
    void setpath(const QString &path);

    QVariantList values();
    void setvalues(const QVariantList &values);

    static CurrentEnumDefinitionInfo *current();

signals:
    void pathChanged();
    void valuesChanged();

private:
    QString                 m_path;
    QVariantList            m_values;
};

#endif // CURRENTENUMDEFINITIONINFO_H
