#ifndef TreeModel_H
#define TreeModel_H
#include <QAbstractItemModel>
#include "TreeNode.h"
#include "uit/datafw/Model.h"
#include "uit/datafw/Parser.h"

using namespace uit::datafw;
class TreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    static TreeModel *current();

protected:
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
    virtual QModelIndex parent(const QModelIndex &child) const;
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;
    QHash<int, QByteArray> roleNames() const;

public slots:
    QAbstractItemModel *model();

    void onResourceRefresh(Model &model);

private:
    TreeModel();
    ~TreeModel();

    void loopData(Poco::JSON::Object::Ptr obj, TreeNode *node);
    void loopEvent(Poco::JSON::Object::Ptr obj, TreeNode *node, const std::string &sParent);
    void loopEnum(Poco::JSON::Object::Ptr obj, TreeNode *node, const std::string &sParent);
    void loopItemTemplate(Poco::JSON::Object::Ptr obj, TreeNode *node, const std::string &sParent);

    TreeNode           *m_Root;
    static TreeModel   *g_sInstance;
    Parser             *m_Parser;
};

#endif // TreeModel_H
