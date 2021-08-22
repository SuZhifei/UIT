#include "TreeNode.h"

TreeNode::TreeNode()
    : m_Parent(NULL)
{

}

TreeNode::TreeNode(const QList<QVariant> &data)
    : m_Parent(NULL)
    , m_Data(data)
{
}

TreeNode::~TreeNode()
{

}

void TreeNode::append(TreeNode *node)
{
    node->setParent(this);
    m_children.append(node);
}

TreeNode *TreeNode::parent()
{
    return m_Parent;
}

void TreeNode::setParent(TreeNode *node)
{
    m_Parent = node;
}

int TreeNode::childCount() const
{
    return m_children.size();
}

TreeNode *TreeNode::child(int index)
{
    return m_children.value(index);
}

QVariant TreeNode::data(int col) const
{
    return m_Data.value(col);
}

int TreeNode::index() const
{
    if(!m_Parent)
        return 0;
    return m_Parent->m_children.indexOf(const_cast<TreeNode *>(this));
}

void TreeNode::release()
{
    loopRelease(this);
    this->m_children.clear();
}

void TreeNode::loopRelease(TreeNode *p)
{
    if(p->childCount() == 0 && p != this)
        delete p;
    else
    {
        for(int i = 0; i != p->childCount(); ++i)
        {
            TreeNode *pxx = p->child(i);
            loopRelease(pxx);
        }
    }
}
