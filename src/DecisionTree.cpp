#include "DecisionTree.h"
#include <iostream>
#include "ActionNode.h"

DecisionTree::DecisionTree(){}

DecisionTree::~DecisionTree()
= default;

Agent* DecisionTree::getAgent()
{
    return m_pAgent;
}

void DecisionTree::setAgent(Agent* agent)
{
    m_pAgent = agent;

    for (auto node : m_treeNodeList)
        node->setAgent(agent);
}

TreeNode* DecisionTree::AddNode(TreeNode* parent, TreeNode* child_node, TreeNodeType type)
{
    switch (type)
    {
    case LEFT_TREE_NODE:
        parent->Left = child_node;

        break;
    case RIGHT_TREE_NODE:
        parent->Right = child_node;

        break;
    }

    child_node->Parent = parent;    // for backwards traversal

    return child_node;
}

void DecisionTree::DisplayTree()
{
    for (auto* node : m_treeNodeList)
    {
        std::cout << node->name << std::endl;
    }
}

// in-order traversal
std::string DecisionTree::MakeDecision()
{
    auto currentNode = m_treeNodeList[0]; // start at root node

    while (!currentNode->isLeaf)
    {
        currentNode = (currentNode->data) ? (currentNode->Right) : (currentNode->Left);
    }
    if (currentNode->getNodeType() == ACTION_NODE)
        static_cast<ActionNode*>(currentNode)->Action();
    return currentNode->name;
}