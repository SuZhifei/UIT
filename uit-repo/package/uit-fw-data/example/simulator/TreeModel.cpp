#include "TreeModel.h"
#include "ModelCluster.h"
#include "CurrentClusterInfo.h"
#include <QDebug>

using namespace uit::datafw;

TreeModel *TreeModel::g_sInstance = NULL;
TreeModel *TreeModel::current()
{
    if(!g_sInstance)
        g_sInstance = new TreeModel();
    return g_sInstance;
}

TreeModel::TreeModel()
    : m_Parser(NULL)
{
    m_Root = new TreeNode();
    connect(ModelCluster::current(), SIGNAL(resourceFresh(Model&)), this, SLOT(onResourceRefresh(Model&)));
}

TreeModel::~TreeModel()
{
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    switch (role) {
    case 0:
        return static_cast<TreeNode *>(index.internalPointer())->data(0);
    default:
        return QVariant();
    }
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!hasIndex(row, column, parent))
        return QModelIndex();

    TreeNode *parentItem;
    if (!parent.isValid())
    {
        parentItem = m_Root;
    }
    else
    {
        parentItem = static_cast<TreeNode*>(parent.internalPointer());
    }

    TreeNode *childItem = parentItem->child(row);
    if (childItem)
    {
        return createIndex(row, column, childItem);
    }
    else
    {
        return QModelIndex();
    }
}

QModelIndex TreeModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
    {
        return QModelIndex();
    }

    TreeNode *childItem = static_cast<TreeNode*>(child.internalPointer());
    TreeNode *parentItem = childItem->parent();

    if (parentItem == m_Root)
    {
        return QModelIndex();
    }

    return createIndex(parentItem->index(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    if(parent.column() > 0)
        return 0;

    if(parent.isValid())
        return (static_cast<TreeNode *>(parent.internalPointer()))->childCount();
    else
        return m_Root->childCount();
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    (void)parent;
    return 1;
}

QHash<int, QByteArray> TreeModel::roleNames() const
{
    QHash<int, QByteArray> keys(QAbstractItemModel::roleNames());
    keys[0] = "name";
    return keys;
}

QAbstractItemModel *TreeModel::model()
{
    return this;
}

void TreeModel::onResourceRefresh(Model &model)
{
    m_Parser = &model.parser();
    QList<QVariant> dataValue;
    dataValue.append("Data");
    TreeNode *pData = new TreeNode(dataValue);
    loopData(m_Parser->getDataRoot(), pData);
    QList<QVariant> eventValue;
    eventValue.append("Event");
    TreeNode *pEvent = new TreeNode(eventValue);
    loopEvent(m_Parser->getEventRoot(), pEvent, "");
    QList<QVariant> enumValue;
    enumValue.append("Enum");
    TreeNode *pEnum = new TreeNode(enumValue);
    loopEnum(m_Parser->getEnumRoot(), pEnum, "");
    QList<QVariant> itemTemplateValue;
    itemTemplateValue.append("Template");
    TreeNode *pItemTemplate = new TreeNode(itemTemplateValue);
    loopItemTemplate(m_Parser->getItemTemplateRoot(), pItemTemplate, "");

    beginResetModel();
    m_Root->release();
    m_Root->append(pData);
    m_Root->append(pEvent);
    m_Root->append(pEnum);
    m_Root->append(pItemTemplate);
    endResetModel();

    CurrentClusterInfo::current()->settype(Type::RootType_None);
}

void TreeModel::loopData(Poco::JSON::Object::Ptr obj, TreeNode *node)
{
    for (Poco::JSON::Object::ConstIterator iter = obj->begin(); iter != obj->end(); ++iter)
    {
        QString sName = QString::fromStdString(iter->first);
        const Poco::Dynamic::Var &v = iter->second;
        ValueType type = m_Parser->testType(v);

        QList<QVariant> value;
        value.append(sName);            //路径
        switch (type)
        {
        case booleanValue:
        case intValue:
        case realValue:
        case stringValue:
        case enumValue:
        case imageValue:
        case listValue:
        {
            node->append(new TreeNode(value));
            break;
        }
        case objectValue:
        {
            TreeNode *pNew = new TreeNode(value);
            node->append(pNew);
            loopData(v.extract<Poco::JSON::Object::Ptr>(), pNew);
            break;
        }
        default:		printf("unknown type.\r\n"); break;
        }
    }
}

void TreeModel::loopEvent(Poco::JSON::Object::Ptr obj, TreeNode *node, const std::string &sParent)
{
    for (Poco::JSON::Object::ConstIterator iter = obj->begin(); iter != obj->end(); ++iter)
    {
        std::string sName = iter->first;
        const Poco::Dynamic::Var &v = iter->second;
        std::string sEventPath = sParent + "." + sName;
        Poco::JSON::Object::Ptr args = m_Parser->findEvent(sEventPath.substr(1));
        QList<QVariant> value;
        value.append(QString::fromStdString(sName));
        if(args)
        {
            node->append(new TreeNode(value));
        }
        else
        {
            TreeNode *pNew = new TreeNode(value);
            node->append(pNew);
            loopEvent(v.extract<Poco::JSON::Object::Ptr>(), pNew, sEventPath);
        }
    }
}

void TreeModel::loopEnum(Poco::JSON::Object::Ptr obj, TreeNode *node, const std::string &sParent)
{
    for (Poco::JSON::Object::ConstIterator iter = obj->begin(); iter != obj->end(); ++iter)
    {
        std::string sName = iter->first;
        const Poco::Dynamic::Var &v = iter->second;
        std::string sCombine = sParent + "." + sName;

        QList<QVariant> value;
        value.append(QString::fromStdString(sName));
        std::vector<std::string> stringValues;
        bool bExist = m_Parser->findEnumDefinition(sCombine.substr(1), stringValues);
        if(bExist)
        {
            node->append(new TreeNode(value));
        }
        else
        {
            TreeNode *pNew = new TreeNode(value);
            node->append(pNew);
            loopEnum(v.extract<Poco::JSON::Object::Ptr>(), pNew, sCombine);
        }
    }
}

void TreeModel::loopItemTemplate(Poco::JSON::Object::Ptr obj, TreeNode *node, const std::string &sParent)
{
    for (Poco::JSON::Object::ConstIterator iter = obj->begin(); iter != obj->end(); ++iter)
    {
        std::string sName = iter->first;
        const Poco::Dynamic::Var &v = iter->second;
        std::string sCombine = sParent + "." + sName;

        QList<QVariant> value;
        value.append(QString::fromStdString(sName));
        bool bExist = m_Parser->findItemTemplate(sCombine.substr(1));
        if(bExist)
        {
            node->append(new TreeNode(value));
        }
        else
        {
            TreeNode *pNew = new TreeNode(value);
            node->append(pNew);
            loopItemTemplate(v.extract<Poco::JSON::Object::Ptr>(), pNew, sCombine);
        }
    }
}

