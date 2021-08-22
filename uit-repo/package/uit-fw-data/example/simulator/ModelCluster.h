#ifndef MODELCLUSTER_H
#define MODELCLUSTER_H

#include <QObject>
#include "uit/datafw/Model.h"
#include "Type.h"
#include <QMap>
#include <QVariant>

using namespace uit::datafw;
class ModelCluster : public QObject
{
    Q_OBJECT

public:
    ModelCluster();
    static ModelCluster *current();

    void startup();

    void log(const QString &str);

signals:
    void signalLog(const QString &s);
    void signalOnReceivedEvent(const QString &sEvent, const QString &sJson);
    void resourceFresh(Model &model);

public slots:
    void slotLog(const QString &s);
    void slotOnReceivedEvent(const QString &sEvent, const QString &sJson);

    bool load(const QUrl &dir);
    void onSelectDataChanged(const QString &path);
    void onSelectEventChanged(const QString &path);
    void onSelectEnumChanged(const QString &path);
    void onSelectTempalteChanged(const QString &path);

    //post event
    void postEvent(const QVariantList &values);

    //UI side data changed
    void onBoolChanged(bool b);
    void onIntChanged(int n);
    void onRealChanged(double d);
    void onStringChanged(const QString &str);
    void onEnumChanged(const QString &enumStringValue);
    void onImageChanged(const QString &imgPath);

    void onResizeList(int size);
    void onRemoveListItem(int index);
    void onInsertListItem(int index);
    void onUpdateListItems(int index, const QVariantList &values);
    void onUpdateListViewport(int index);
    void onCheckListItem(int index);
    void onFocusListItem(int index);
    void onVKey(int key, int state);

    //current list template
    QStringList rowTitles();
    QList<int> rowTypes();
    QVariantList rowValues();
    QString rowEnumValues(const QString &sElement);

    //current event args
    QStringList argsTitles();
    QList<int> argsTypes();
    QVariantList argsValues();
    QString argsEnumValues(const QString &sArg);

    //Clipboard
    void setClipboard(const QString &str);
    void search(const QString &path);

private:
    //thread
    void on_Model_View_Connected(const Model::ViewConnectedArgs &args);
    void on_Model_Controller_Connected(const Model::ControllerConnectedArgs &args);
    void on_Model_Received_Event(const Model::UserActionArgs &args);

    void updateCurrentListHeader();
    void updateCurrentListRow();

	
    bool                    m_bPostByModel;
    bool                    m_bStartup;
    Model                   m_Model;
    static ModelCluster     *g_Instance;
};

#endif // MODELCLUSTER_H
