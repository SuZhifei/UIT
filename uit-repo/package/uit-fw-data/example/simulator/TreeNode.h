#ifndef TREEITEM_H
#define TREEITEM_H
#include <QVariantMap>
#include <QStringList>
#include <QList>

class TreeNode
{
public:
    TreeNode();
    TreeNode(const QList<QVariant> &data);
    ~TreeNode();

    void append(TreeNode *node);

    TreeNode *parent();
    void setParent(TreeNode *node);

    int childCount() const;
    TreeNode *child(int index);

    QVariant data(int col) const;

    int index() const;

    void release();

private:
    void loopRelease(TreeNode *p);

    TreeNode            *m_Parent;
    QList<QVariant>     m_Data;
    QList<TreeNode *>   m_children;
};

#endif // TREEITEM_H
