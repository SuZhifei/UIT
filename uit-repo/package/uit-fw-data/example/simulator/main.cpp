#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQuick>
#include <QUrl>
#include "TreeModel.h"
#include "ModelCluster.h"
#include "CurrentDataInfo.h"
#include "CurrentEventInfo.h"
#include "CurrentClusterInfo.h"
#include "CurrentEnumDefinitionInfo.h"
#include "CurrentItemTemplateInfo.h"
#include "SocketDebugger.h"
#include "ImageProvider.h"
#include "Type.h"
#include "uit/core/Runtime.h"

QQmlApplicationEngine *engine = NULL;
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<Type>("UIT.Type", 1, 0, "Type");

    engine = new QQmlApplicationEngine();
    engine->rootContext()->setContextProperty("TreeModel", TreeModel::current());
    engine->rootContext()->setContextProperty("ModelCluster", ModelCluster::current());
    engine->rootContext()->setContextProperty("CurrentDataInfo", CurrentDataInfo::current());
    engine->rootContext()->setContextProperty("CurrentEventInfo", CurrentEventInfo::current());
    engine->rootContext()->setContextProperty("CurrentClusterInfo", CurrentClusterInfo::current());
    engine->rootContext()->setContextProperty("SocketDebugger", SocketDebugger::current());
    engine->rootContext()->setContextProperty("CurrentEnumDefinitionInfo", CurrentEnumDefinitionInfo::current());
    engine->rootContext()->setContextProperty("CurrentItemTemplateInfo", CurrentItemTemplateInfo::current());
    engine->rootContext()->setContextProperty("ImageProvider", ImageProvider::current());
    engine->addImageProvider("ImgProvider", ImageProvider::current());
    engine->load(QUrl(QStringLiteral("qrc:/main.qml")));

    std::string sDir = uit::Runtime::getUitEtcDirectory() + "data-event";
    if(argc >= 2)
        sDir = argv[1];

    ModelCluster::current()->load(QUrl::fromLocalFile(QString::fromStdString(sDir)));
    ModelCluster::current()->startup();

    QObject::connect(&app, SIGNAL(aboutToQuit()), SocketDebugger::current(), SLOT(quit()));

    return app.exec();
}
