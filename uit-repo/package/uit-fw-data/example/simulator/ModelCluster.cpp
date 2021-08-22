#include "ModelCluster.h"
#include "CurrentDataInfo.h"
#include "CurrentEventInfo.h"
#include "CurrentClusterInfo.h"
#include "CurrentEnumDefinitionInfo.h"
#include "CurrentItemTemplateInfo.h"
#include "SocketDebugger.h"
#include "ImageProvider.h"
#include <QFile>
#include <QDebug>
#include <QQmlApplicationEngine>
#include <QTimer>
#include <QClipboard>
#include <QApplication>
#include <functional>

extern QQmlApplicationEngine *engine;


using namespace uit::datafw;

ModelCluster *ModelCluster::g_Instance = NULL;
ModelCluster::ModelCluster()
    : m_bPostByModel(false)
    , m_bStartup(false)
{
    connect(this, SIGNAL(signalLog(QString)), this, SLOT(slotLog(QString)));
    connect(this, SIGNAL(signalOnReceivedEvent(QString,QString)), this, SLOT(slotOnReceivedEvent(QString,QString)));
    m_Model.ViewConnectedEvent += std::bind(&ModelCluster::on_Model_View_Connected, this, std::placeholders::_1);
    m_Model.ControllerConnectedEvent += std::bind(&ModelCluster::on_Model_Controller_Connected, this, std::placeholders::_1);
    m_Model.UserActionEvent += std::bind(&ModelCluster::on_Model_Received_Event, this, std::placeholders::_1);
}

ModelCluster *ModelCluster::current()
{
    if(!g_Instance)
        g_Instance = new ModelCluster();
    return g_Instance;
}

void ModelCluster::startup()
{
    m_bStartup = m_Model.startup();
    if(m_bStartup)
    {
        QString s = QStringLiteral("【model】：model启动服务成功");
        ModelCluster::current()->log(s);
    }
    else
    {
        QString s = QStringLiteral("【model】：model启动服务失败，可能是由于已经有一个model启动，在此情况下，你可以开启socket debug模式来进行远程调试");
        ModelCluster::current()->log(s);
    }
}

bool ModelCluster::load(const QUrl &dir)
{
    std::string s = dir.toLocalFile().toStdString();
    bool b = m_Model.configDataEventDir(s);
    if(b)
    {
        QString s = QStringLiteral("【配置】：model配置数据文件路径%1成功，解析完成").arg(dir.toLocalFile());
        log(s);
    }
    else
    {
        QString s = QStringLiteral("【配置】：配置数据文件路径%1失败，可能该路径不包含有效的json文件，或者解析时失败").arg(dir.toLocalFile());
        log(s);
    }

    emit resourceFresh(m_Model);
    return b;
}

void ModelCluster::onSelectDataChanged(const QString &path)
{
    CurrentClusterInfo::current()->settype(Type::RootType_Data);
    //更新当前路径、类型
    CurrentDataInfo::current()->setpath(path);
    const Parser &parser = m_Model.getParser();
    Poco::Dynamic::Var v = parser.findData(path.toStdString());
    Type::DataType type = Type::translate(parser.testType(v));
    CurrentDataInfo::current()->settype(type);

    //如果是enum，更新枚举定义类型（额外类型）
    //更新值
    switch (type) {
    case Type::DataType_Bool:
        CurrentDataInfo::current()->setextraType("[NONE]");
        CurrentDataInfo::current()->setvalue(QVariant::fromValue(v.convert<bool>()));
        break;
    case Type::DataType_Int:
    {
        CurrentDataInfo::current()->setextraType("[NONE]");
        int n = v.convert<int>();
        CurrentDataInfo::current()->setvalue(QVariant::fromValue(n));
        break;
    }
    case Type::DataType_Real:
        CurrentDataInfo::current()->setextraType("[NONE]");
        CurrentDataInfo::current()->setvalue(QVariant::fromValue(v.convert<double>()));
        break;
    case Type::DataType_String:
        CurrentDataInfo::current()->setextraType("[NONE]");
        CurrentDataInfo::current()->setvalue(QVariant::fromValue(QString::fromStdString(v.convert<std::string>())));
        break;
    case Type::DataType_Enum:
    {
        std::string sEnumDefinition;
        std::vector<std::string> stringValues;
        parser.extractEnumDefinition(v, sEnumDefinition, stringValues);
        int e = parser.extractEnumInteger(v);
        std::string sStringValue = parser.extractEnumStringValue(v);
        CurrentDataInfo::current()->setextraType(QString::fromStdString(sEnumDefinition));
        CurrentDataInfo::current()->setvalue(QVariant::fromValue(QString::fromStdString(sStringValue)));
        QVariantList x;
        for(int i = 0; i != stringValues.size(); ++i)
        {
            x.append(QString::fromStdString(stringValues[i]));
        }
        CurrentDataInfo::current()->setEnumValues(x);
        break;
    }
    case Type::DataType_Image:
    {
        CurrentDataInfo::current()->setextraType("[NONE]");
        ImageData img = parser.extractImage(v);
        QImage::Format qFormat = QImage::Format_Invalid;
        switch (img.format()) {
        case rgb555Format: qFormat = QImage::Format_RGB555; break;
        case rgb565Format: qFormat = QImage::Format_ARGB8565_Premultiplied; break;
        case rgb888Format: qFormat = QImage::Format_RGB888; break;
        case rgb8888Format: qFormat = QImage::Format_RGB32; break;
        default:    break;
        }

        //下面的接口有挂死问题
        ImageProvider::current()->setImage(img.data(), img.width(), img.height(), qFormat);
        break;
    }
    case Type::DataType_List:
    {
        CurrentDataInfo::current()->setextraType("[NONE]");
        updateCurrentListHeader();
        updateCurrentListRow();
        onResizeList(0);
      //  updateCurrentList(path);
        break;
    }
    case Type::DataType_Object:
        CurrentDataInfo::current()->setextraType("[NONE]");
        break;
    default:
        break;
    }

}

void ModelCluster::onSelectEventChanged(const QString &path)
{
    CurrentClusterInfo::current()->settype(Type::RootType_Event);
    CurrentEventInfo::current()->setpath(path);
    bool bExist = m_Model.parser().findEvent(path.toStdString());
    if(bExist)
        CurrentEventInfo::current()->settype(Type::EventType_Event);
    else
        CurrentEventInfo::current()->settype(Type::EventType_Object);

    QObject *qmlRoot = engine->rootObjects().first();
    QObject *arg = qmlRoot->findChild<QObject *>("eventArgs");
    if(arg)
    {
        QVariant ret;
        QMetaObject::invokeMethod(arg, "updateEventArg", Q_RETURN_ARG(QVariant, ret));
    }
}

void ModelCluster::onSelectEnumChanged(const QString &path)
{
    CurrentClusterInfo::current()->settype(Type::RootType_EnumDefinition);
    CurrentEnumDefinitionInfo::current()->setpath(path);
    std::vector<std::string> stringValues;
    bool bExist = m_Model.parser().findEnumDefinition(path.toStdString(), stringValues);
    QVariantList values;
    if(bExist)
    {
        for(int i = 0; i != stringValues.size(); ++i)
            values.append(QString::fromStdString(stringValues[i]));
    }
    CurrentEnumDefinitionInfo::current()->setvalues(values);
}

void ModelCluster::onSelectTempalteChanged(const QString &path)
{
    CurrentClusterInfo::current()->settype(Type::RootType_ItemTemplate);
    CurrentItemTemplateInfo::current()->setpath(path);
    Poco::JSON::Object::Ptr itemTemplate = m_Model.parser().findItemTemplate(path.toStdString());
    QVariantList values;
    if(itemTemplate)
    {
        for(Poco::JSON::Object::ConstIterator iter = itemTemplate->begin(); iter != itemTemplate->end(); ++iter)
        {
            std::string sName = iter->first;
            Poco::Dynamic::Var v = iter->second;
            QString qs = QString::fromStdString(sName);
            switch (m_Model.parser().testType(v)) {
            case booleanValue:  qs = qs + "\t\t[bool]"; break;
            case intValue:  qs = qs + "\t\t[int]"; break;
            case realValue:  qs = qs + "\t\t[real]"; break;
            case stringValue:  qs = qs + "\t\t[string]"; break;
            case imageValue:  qs = qs + "\t\t[image]"; break;
            case enumValue:
            {
                std::string sEnumDefinition;
                std::vector<std::string> stringValues;
                m_Model.parser().extractEnumDefinition(v, sEnumDefinition, stringValues);
                qs = qs + "\t\t[" + QString::fromStdString(sEnumDefinition) + "]";
                break;
            }
            default:    break;
            }
            values.append(qs);
        }
    }
    CurrentItemTemplateInfo::current()->setvalues(values);
}

void ModelCluster::postEvent(const QVariantList &values)
{
    m_bPostByModel = true;
    std::string sPath = CurrentEventInfo::current()->path().toStdString();
    //不要直接使用args的set，这样会导致源event被修改。实际项目不会直接操作model的parser，所以不必担心这个问题
    Poco::JSON::Object::Ptr args = m_Model.parser().findEvent(sPath);
    if(!args)
    {
        qDebug() << "can't find event" << QString::fromStdString(sPath);
    }

    Poco::JSON::Object::Ptr postArgs = new Poco::JSON::Object();
    int i = 0;
    for(Poco::JSON::Object::Iterator iter = args->begin(); iter != args->end(), i != args->size(); ++i, ++iter)
    {
        const std::string &sName = iter->first;
        Poco::Dynamic::Var var = iter->second;
        QVariant v = values[i];
        switch (m_Model.parser().testType(var)) {
        case booleanValue:      postArgs->set(sName, v.toBool());                   break;
        case intValue:			postArgs->set(sName, v.toInt());                    break;
        case realValue:         postArgs->set(sName, v.toReal());                   break;
        case stringValue:		postArgs->set(sName, v.toString().toStdString());   break;
        case enumValue:
        {
            std::string sEnumDefinition = var.extract<Poco::JSON::Array::Ptr>()->get(0);
            std::string sEnum = v.toString().toStdString();
            postArgs->set(sName, m_Model.parser().getEnumIntegerByStringValue(sEnumDefinition, sEnum));
            break;
        }
        default:		qDebug() << "unknown arg element type.\r\n"; break;
        }
    }

    bool b = m_Model.postEvent(sPath, postArgs);
    if(b)
    {
        QString s = QStringLiteral("【发送事件】：%1").arg(CurrentEventInfo::current()->path());
        log(s);
    }
    else
    {
        QString s = QStringLiteral("【设置图片数据】：%1 失败，可能参数类型不允许").arg(CurrentEventInfo::current()->path());
        log(s);
    }

    SocketDebugger::current()->postEvent(sPath, postArgs);
    m_bPostByModel = false;
}

void ModelCluster::updateCurrentListHeader()
{
    const Parser &parser = m_Model.getParser();
    //更新list表头
    QList<QString> headers;
    Poco::JSON::Array::Ptr list = parser.findData(CurrentDataInfo::current()->path().toStdString()).extract<Poco::JSON::Array::Ptr>();
    Poco::JSON::Object::Ptr itemTemplate = parser.extractItemTemplate(list);
    for(Poco::JSON::Object::ConstIterator iter = itemTemplate->begin(); iter != itemTemplate->end(); ++iter)
    {
        headers.append(QString::fromStdString(iter->first));
    }

    QObject *qmlRoot = engine->rootObjects().first();
    QObject *tbView = qmlRoot->findChild<QObject *>("tbView");
    if(tbView)
    {
        QVariant ret;
        QMetaObject::invokeMethod(tbView, "updateHeader", Q_RETURN_ARG(QVariant, ret), Q_ARG(QVariant, QVariant::fromValue(headers)));
    }
}

void ModelCluster::updateCurrentListRow()
{
    QObject *qmlRoot = engine->rootObjects().first();
    QObject *rowInfo = qmlRoot->findChild<QObject *>("rowInfo");
    if(rowInfo)
    {
        QVariant ret;
        QMetaObject::invokeMethod(rowInfo, "updateRowInfo", Q_RETURN_ARG(QVariant, ret));
    }
}

void ModelCluster::log(const QString &str)
{
    emit signalLog(str);
}

void ModelCluster::slotLog(const QString &s)
{
    QObject *qmlRoot = engine->rootObjects().first();
    QObject *logP = qmlRoot->findChild<QObject *>("logPanel");
    if(logP)
    {
        QVariant ret;
        QMetaObject::invokeMethod(logP, "log", Q_RETURN_ARG(QVariant, ret), Q_ARG(QVariant, QVariant::fromValue(s)));
    }
}

void ModelCluster::slotOnReceivedEvent(const QString &sEvent, const QString &sJson)
{
    (void)sJson;
    QString sxx = QStringLiteral("【监测到事件】：%1").arg(sEvent);
    log(sxx);

    QObject *qmlRoot = engine->rootObjects().first();
    QObject *eListener = qmlRoot->findChild<QObject *>("eventListener");
    if(eListener)
    {
        QVariant eventRet;
        QMetaObject::invokeMethod(eListener, "getEvent", Q_RETURN_ARG(QVariant, eventRet));

        QVariant cmdsRet;
        QMetaObject::invokeMethod(eListener, "getCmds", Q_RETURN_ARG(QVariant, cmdsRet));

        QString sEventName = eventRet.toString();
        QString s = cmdsRet.toString();
        QStringList list = s.split("|", QString::SkipEmptyParts);
        if(list.isEmpty())
        {
            return;
        }
        else
        {
            QString s = QStringLiteral("【监测到事件】执行以下%1条指令：------").arg(QString::number(list.size()));
            log(s);
        }
        for(int i = 0; i != list.size(); ++i)
        {
            QString one = list[i];
            int index = one.indexOf('=');
            QString path = one.left(index);
            QString value = one.right(one.length() - index - 1);
            Poco::Dynamic::Var v = m_Model.parser().findData(path.toStdString());
            switch (m_Model.parser().testType(v)) {
            case booleanValue:
            {
                bool b = value.toInt();
                m_Model.setBoolean(path.toStdString(), b);
                QString s = QStringLiteral("【设置布尔值】：%1=%2").arg(path).arg(b ? "true" : "false");
                log(s);
                break;
            }
            case intValue:
            {
                bool bOk = false;
                int n;
                n = value.toInt(&bOk);
                if(!bOk)
                    n = value.toDouble();
                m_Model.setInteger(path.toStdString(), n);
                QString s = QStringLiteral("【设置整型值】：%1=%2").arg(path).arg(QString::number(n));
                log(s);
                break;
            }
            case realValue:
            {
                bool bOk = false;
                double d = value.toDouble(&bOk);
                if(!bOk)
                    d = value.toInt();
                m_Model.setReal(path.toStdString(), d);
                QString s = QStringLiteral("【设置浮点值】：%1=%2").arg(path).arg(QString::number(d));
                log(s);
                break;
            }
            case stringValue:
            {
                std::string str = value.toStdString();
                m_Model.setString(path.toStdString(), str);
                QString s = QStringLiteral("【设置字符串值】：%1=%2").arg(path).arg(QString::fromStdString(str));
                log(s);
                break;
            }
            case enumValue:
            {
                bool bOk = false;
                int n = value.toInt(&bOk);
                if(bOk)
                {
                    m_Model.setEnum(path.toStdString(), n);
                    QString s = QStringLiteral("【设置枚举值】：%1=%2").arg(path).arg(n);
                    log(s);
                }
                else
                {
                    m_Model.setEnumStringValue(path.toStdString(), value.toStdString());
                    QString s = QStringLiteral("【设置枚举值】：%1=%2").arg(path).arg(value);
                    log(s);
                }
                break;
            }
            default:
                QString s = QStringLiteral("【错误】：%1不存在").arg(path);
                log(s);
                break;
            }
        }

        log(QStringLiteral("【监测到事件】执行指令完成：------"));
    }
}

void ModelCluster::onBoolChanged(bool b)
{
    CurrentDataInfo::current()->setvalue(QVariant::fromValue(b));
    SocketDebugger::current()->setBoolean(CurrentDataInfo::current()->path().toStdString(), b);
    m_Model.setBoolean(CurrentDataInfo::current()->path().toStdString(), b);
    QString s = QStringLiteral("【设置布尔值】：%1=%2").arg(CurrentDataInfo::current()->path()).arg(b ? "true" : "false");
    log(s);
}

void ModelCluster::onIntChanged(int n)
{
    CurrentDataInfo::current()->setvalue(QVariant::fromValue(n));
    SocketDebugger::current()->setInteger(CurrentDataInfo::current()->path().toStdString(), n);
    m_Model.setInteger(CurrentDataInfo::current()->path().toStdString(), n);
    QString s = QStringLiteral("【设置整型值】：%1=%2").arg(CurrentDataInfo::current()->path()).arg(QString::number(n));
    log(s);
}

void ModelCluster::onRealChanged(double d)
{
    CurrentDataInfo::current()->setvalue(QVariant::fromValue(d));
    SocketDebugger::current()->setReal(CurrentDataInfo::current()->path().toStdString(), d);
    m_Model.setReal(CurrentDataInfo::current()->path().toStdString(), d);
    QString s = QStringLiteral("【设置浮点值】：%1=%2").arg(CurrentDataInfo::current()->path()).arg(QString::number(d));
    log(s);
}

void ModelCluster::onStringChanged(const QString &str)
{
    CurrentDataInfo::current()->setvalue(QVariant::fromValue(str));
    SocketDebugger::current()->setString(CurrentDataInfo::current()->path().toStdString(), str.toStdString());
    m_Model.setString(CurrentDataInfo::current()->path().toStdString(), str.toStdString());
    QString s = QStringLiteral("【设置字符串值】：%1=%2").arg(CurrentDataInfo::current()->path()).arg(str);
    log(s);
}

void ModelCluster::onEnumChanged(const QString &enumStringValue)
{
    CurrentDataInfo::current()->setvalue(QVariant::fromValue(enumStringValue));
    SocketDebugger::current()->setEnumStringValue(CurrentDataInfo::current()->path().toStdString(), enumStringValue.toStdString());
    m_Model.setEnumStringValue(CurrentDataInfo::current()->path().toStdString(), enumStringValue.toStdString());
    QString s = QStringLiteral("【设置枚举值】：%1=%2").arg(CurrentDataInfo::current()->path()).arg(enumStringValue);
    log(s);
}

void ModelCluster::onImageChanged(const QString &imgPath)
{
    bool b = ImageProvider::current()->setImage(imgPath);
    if(b)
    {
        const QImage &qImg = ImageProvider::current()->m_img;
        ImageFormat format = invalidFormat;
        QImage::Format qFormat = qImg.format();
        switch (qFormat) {
        case QImage::Format_RGB555: format = rgb555Format;  break;
        case QImage::Format_ARGB8565_Premultiplied: format = rgb565Format;  break;
        case QImage::Format_RGB888: format = rgb888Format;  break;
        case QImage::Format_RGB32: format = rgb8888Format;  break;
        case QImage::Format_ARGB32: format = rgb8888Format;  break;
        default:    break;
        }
        ImageData img(qImg.bits(), qImg.width(), qImg.height(), format);
        std::string sPath = CurrentDataInfo::current()->path().toStdString();
        if(m_Model.setImage(sPath, img))
        {
            QString s = QStringLiteral("【设置图片数据】：%1，%2 字节").arg(CurrentDataInfo::current()->path()).arg(QString::number(img.bytes()));
            log(s);
        }
        else
        {
            QString s = QStringLiteral("【设置图片数据】：%1，%2 字节失败，可能图片过大").arg(CurrentDataInfo::current()->path()).arg(QString::number(img.bytes()));
            log(s);
        }
    }
}

void ModelCluster::onResizeList(int size)
{
    QObject *qmlRoot = engine->rootObjects().first();
    QObject *tbView = qmlRoot->findChild<QObject *>("tbView");
    if(tbView)
    {
        QVariant ret;
        QMetaObject::invokeMethod(tbView, "resize", Q_RETURN_ARG(QVariant, ret), Q_ARG(QVariant, QVariant::fromValue(size)));
    }
    m_Model.resizeList(CurrentDataInfo::current()->path().toStdString(), size);
    QString s = QStringLiteral("【设置列表大小】：%1=%2").arg(CurrentDataInfo::current()->path()).arg(QString::number(size));
    log(s);
    SocketDebugger::current()->resizeList(CurrentDataInfo::current()->path().toStdString(), size);
}

void ModelCluster::onRemoveListItem(int index)
{
    SocketDebugger::current()->removeListItems(CurrentDataInfo::current()->path().toStdString(), index, 1);
    m_Model.removeListItems(CurrentDataInfo::current()->path().toStdString(), index, 1);
    QString s = QStringLiteral("【移除列表项】：%1，第 %2 项").arg(CurrentDataInfo::current()->path()).arg(QString::number(index));
    log(s);
}

void ModelCluster::onInsertListItem(int index)
{
    SocketDebugger::current()->insertListItems(CurrentDataInfo::current()->path().toStdString(), index, 1);
    m_Model.insertListItems(CurrentDataInfo::current()->path().toStdString(), index, 1);
    QString s = QStringLiteral("【插入列表项】：%1，第 %2 项").arg(CurrentDataInfo::current()->path()).arg(QString::number(index));
    log(s);
}

void ModelCluster::onUpdateListItems(int index, const QVariantList &values)
{
    std::set<unsigned int> indexs;
    indexs.insert(index);
    Poco::JSON::Array::Ptr items = new Poco::JSON::Array();
    Poco::JSON::Object::Ptr item = new Poco::JSON::Object();
    std::string sPath = CurrentDataInfo::current()->path().toStdString();
    Poco::JSON::Array::Ptr list = m_Model.parser().findData(sPath).extract<Poco::JSON::Array::Ptr>();
    Poco::JSON::Object::Ptr itemTemplate = m_Model.parser().extractItemTemplate(list);
    int i = 0;
    for(Poco::JSON::Object::ConstIterator iter = itemTemplate->begin(); iter != itemTemplate->end(); ++iter, ++i)
    {
        QString srcV = values[i].toString();
        std::string key = iter->first;
        const Poco::Dynamic::Var &v = iter->second;
        switch (m_Model.parser().testType(v)) {
        case booleanValue:
            item->set(key, (bool)(srcV.toInt()));
            break;
        case intValue:
            item->set(key, (int)(srcV.toInt()));
            break;
        case realValue:
            item->set(key, (double)(srcV.toDouble()));
            break;
        case stringValue:
            item->set(key, srcV.toStdString());
            break;
        case enumValue:
            item->set(key, srcV.toStdString());
            break;
        case imageValue:
            //item->set(key, ImageData());
            break;
        default:
            break;
        }
    }
    items->add(item);
    SocketDebugger::current()->updateListItems(CurrentDataInfo::current()->path().toStdString(), indexs, items);
    m_Model.updateListItems(sPath, indexs, items);
    QString s = QStringLiteral("【更新列表项】：%1，第%2项").arg(CurrentDataInfo::current()->path()).arg(QString::number(index));
    log(s);
}

void ModelCluster::onUpdateListViewport(int index)
{
    std::string sPath = CurrentDataInfo::current()->path().toStdString();
    SocketDebugger::current()->updateListViewport(CurrentDataInfo::current()->path().toStdString(), index);
    m_Model.updateListViewport(sPath, index);
    QString s = QStringLiteral("【更新列表视口】：%1到第%2项").arg(CurrentDataInfo::current()->path()).arg(QString::number(index));
    log(s);
}

void ModelCluster::onCheckListItem(int index)
{
    SocketDebugger::current()->checkListItem(CurrentDataInfo::current()->path().toStdString(), index);
    std::string sPath = CurrentDataInfo::current()->path().toStdString();
    m_Model.checkListItem(sPath, index);
    QString s = QStringLiteral("【选中列表项】：%1，第%2项").arg(CurrentDataInfo::current()->path()).arg(QString::number(index));
    log(s);
}

void ModelCluster::onFocusListItem(int index)
{
    SocketDebugger::current()->focusListItem(CurrentDataInfo::current()->path().toStdString(), index);
    std::string sPath = CurrentDataInfo::current()->path().toStdString();
    m_Model.focusListItem(sPath, index);
    QString s = QStringLiteral("【设置列表焦点项】：%1，第%2项").arg(CurrentDataInfo::current()->path()).arg(QString::number(index));
    log(s);
}

void ModelCluster::onVKey(int key, int state)
{
    SocketDebugger::current()->vkey(Type::toVKey((Type::keyId)key), Type::toVkeyState((Type::keyState)state));
    m_Model.vkey(Type::toVKey((Type::keyId)key), Type::toVkeyState((Type::keyState)state));
    QString s = QStringLiteral("【按键】：%1=%2").arg(Type::toString((Type::keyId)(key))).arg(Type::toString(Type::keyState(state)));
    log(s);
}

QStringList ModelCluster::rowTitles()
{
    QStringList ret;
    std::string sPath = CurrentDataInfo::current()->path().toStdString();
    Poco::Dynamic::Var vL = m_Model.parser().findData(sPath);
    if(m_Model.getParser().testType(vL) != listValue)
        return ret;

    Poco::JSON::Array::Ptr list = vL.extract<Poco::JSON::Array::Ptr>();
    Poco::JSON::Object::Ptr itemTemplate = m_Model.parser().extractItemTemplate(list);
    for(Poco::JSON::Object::ConstIterator iter = itemTemplate->begin(); iter != itemTemplate->end(); ++iter)
    {
        QString sName = QString::fromStdString(iter->first);
        ret.append(sName);
    }
    return ret;
}

QList<int> ModelCluster::rowTypes()
{
    QList<int> ret;
    std::string sPath = CurrentDataInfo::current()->path().toStdString();
    Poco::JSON::Array::Ptr list = m_Model.parser().findData(sPath).extract<Poco::JSON::Array::Ptr>();
    Poco::JSON::Object::Ptr itemTemplate = m_Model.parser().extractItemTemplate(list);
    for(Poco::JSON::Object::ConstIterator iter = itemTemplate->begin(); iter != itemTemplate->end(); ++iter)
    {
        const Poco::Dynamic::Var &v = iter->second;
        Type::DataType t = Type::translate(m_Model.parser().testType(v));
        ret.append(t);
    }
    return ret;
}

QVariantList ModelCluster::rowValues()
{
    QVariantList ret;
    std::string sPath = CurrentDataInfo::current()->path().toStdString();
    Poco::JSON::Array::Ptr list = m_Model.parser().findData(sPath).extract<Poco::JSON::Array::Ptr>();
    Poco::JSON::Object::Ptr itemTemplate = m_Model.parser().extractItemTemplate(list);
    for(Poco::JSON::Object::ConstIterator iter = itemTemplate->begin(); iter != itemTemplate->end(); ++iter)
    {
        const Poco::Dynamic::Var &v = iter->second;
        switch (m_Model.parser().testType(v)) {
        case booleanValue:      ret.append(v.convert<bool>());       break;
        case intValue:			ret.append(v.convert<int>());        break;
        case realValue:         ret.append(v.convert<double>());     break;
        case stringValue:		ret.append(v.toString().data());     break;
        case enumValue:
        {
            ret.append(m_Model.parser().extractEnumStringValue(v).data());
            break;
        }
        case imageValue:
        {
            ret.append(QVariant());
            break;
        }
        default:		printf("unknown list element type.\r\n"); break;
        }
    }
    return ret;
}

QString ModelCluster::rowEnumValues(const QString &sElement)
{
    QString ret;
    std::string sPath = CurrentDataInfo::current()->path().toStdString();
    Poco::JSON::Array::Ptr list = m_Model.parser().findData(sPath).extract<Poco::JSON::Array::Ptr>();
    Poco::JSON::Object::Ptr itemTemplate = m_Model.parser().extractItemTemplate(list);
    Poco::Dynamic::Var v = itemTemplate->get(sElement.toStdString());
    if(m_Model.parser().testType(v) == enumValue)
    {
        std::vector<std::string> stringValues;
        std::string sEnumDefinitionName;
        bool b = m_Model.parser().extractEnumDefinition(v, sEnumDefinitionName, stringValues);
        for(int i = 0; i != stringValues.size(); ++i)
        {
            ret.append(stringValues[i].data());
            if(i != stringValues.size() - 1)
                ret.append("|");
        }
    }
    return ret;
}

QStringList ModelCluster::argsTitles()
{
    QString sPath = CurrentEventInfo::current()->path();
    Poco::JSON::Object::Ptr args = m_Model.parser().findEvent(sPath.toStdString());
    if(args)
    {
        QStringList ret;
        std::vector<std::string> names;
        args->getNames(names);
        for(int i = 0; i != names.size(); ++i)
            ret.append(QString::fromStdString(names[i]));
        return ret;
    }
    else
    {
        return QStringList();
    }
}

QList<int> ModelCluster::argsTypes()
{
    QString sPath = CurrentEventInfo::current()->path();
    Poco::JSON::Object::Ptr args = m_Model.parser().findEvent(sPath.toStdString());
    if(args)
    {
        QList<int> ret;
        for(auto iter = args->begin(); iter != args->end(); ++iter)
        {
            const Poco::Dynamic::Var &v = iter->second;
            Type::DataType t = Type::translate(m_Model.parser().testType(v));
            ret.append(t);
        }
        return ret;
    }
    else
    {
        return QList<int>();
    }
}

QVariantList ModelCluster::argsValues()
{
    QString sPath = CurrentEventInfo::current()->path();
    Poco::JSON::Object::Ptr args = m_Model.parser().findEvent(sPath.toStdString());
    if(args)
    {
        QVariantList ret;
        for(auto iter = args->begin(); iter != args->end(); ++iter)
        {
            const Poco::Dynamic::Var &v = iter->second;
            switch (m_Model.parser().testType(v)) {
            case booleanValue:      ret.append(v.convert<bool>());       break;
            case intValue:			ret.append(v.convert<int>());        break;
            case realValue:         ret.append(v.convert<double>());     break;
            case stringValue:		ret.append(v.toString().data());     break;
            case enumValue:
            {
                ret.append(m_Model.parser().extractEnumStringValue(v).data());
                break;
            }
            default:		qDebug() << "unknown list element type.\r\n"; break;
            }
        }
        return ret;
    }
    else
    {
        return QVariantList();
    }
}

QString ModelCluster::argsEnumValues(const QString &sArg)
{
    QString sPath = CurrentEventInfo::current()->path();
    Poco::JSON::Object::Ptr args = m_Model.parser().findEvent(sPath.toStdString());
    if(args)
    {
        QString ret;
        std::vector<std::string> stringValues;
        std::string sEnumDefinitionName;
        m_Model.parser().extractEnumDefinition(args->get(sArg.toStdString()), sEnumDefinitionName, stringValues);
        for(int i = 0; i != stringValues.size(); ++i)
        {
            ret.append(stringValues[i].data());
            if(i != stringValues.size() - 1)
                ret.append("|");
        }
        return ret;
    }
    else
    {
        return "";
    }
}

void ModelCluster::setClipboard(const QString &str)
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(str);
}

void ModelCluster::search(const QString &path)
{
    QList<QObject*> xx = engine->rootObjects();
    if(xx.isEmpty())
        return;
    QObject *qmlRoot = engine->rootObjects().first();
    QObject *tree = qmlRoot->findChild<QObject *>("tree");

    if(!m_bStartup || path.trimmed().isEmpty())
    {
        tree->setProperty("searchNode", QVariant(""));
        return;
    }

    QStringList names = path.split(".", QString::SkipEmptyParts);
    Parser &parser = m_Model.parser();
    std::string sPath = path.toStdString();

    if(!parser.findData(sPath).isEmpty())
    {
        if(tree)
        {
            QVariant ret;
            QMetaObject::invokeMethod(tree, "search", Q_RETURN_ARG(QVariant, ret), Q_ARG(QVariant, QVariant::fromValue(0)), Q_ARG(QVariant, QVariant::fromValue(names)));
            tree->setProperty("searchNode", QVariant(names.last()));
        }
    }
    else if(parser.findEvent(sPath))
    {
        if(tree)
        {
            QVariant ret;
            QMetaObject::invokeMethod(tree, "search", Q_RETURN_ARG(QVariant, ret), Q_ARG(QVariant, QVariant::fromValue(1)), Q_ARG(QVariant, QVariant::fromValue(names)));
            tree->setProperty("searchNode", QVariant(names.last()));
        }
    }
    else
    {
        tree->setProperty("searchNode", QVariant(""));
    }
}

void ModelCluster::on_Model_View_Connected(const Model::ViewConnectedArgs &args)
{
    QString s = QStringLiteral("【连接】：GUI进程[%1](%2)已接入").arg(QString::fromStdString(args.name)).arg(QString::number(args.pid));
    log(s);
}

void ModelCluster::on_Model_Controller_Connected(const Model::ControllerConnectedArgs &args)
{
    QString s = QStringLiteral("【连接】：控制器[%1](%2)已接入").arg(QString::fromStdString(args.name)).arg(QString::number(args.pid));
    log(s);
}

void ModelCluster::on_Model_Received_Event(const Model::UserActionArgs &args)
{
    if(!m_bPostByModel)
    {
        emit signalOnReceivedEvent(QString::fromStdString(args.sEvent), QString::fromStdString(args.argsJson));
    }
}
