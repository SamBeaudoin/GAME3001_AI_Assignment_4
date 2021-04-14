#include "TreeNode.h"

Agent* TreeNode::getAgent()
{
    return m_agent;
}

NodeType TreeNode::getNodeType()
{
    return m_nodeType;
}

void TreeNode::setAgent(Agent* agent)
{
    m_agent = agent;
}

void TreeNode::setNodeType(NodeType type)
{
    m_nodeType = type;
}
