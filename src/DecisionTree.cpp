#include "DecisionTree.h"
#include "PatrolAction.h"
#include "MoveToLOSAction.h"
#include "MoveToPlayerAction.h"
#include "AttackAction.h"
#include <iostream>

DecisionTree::DecisionTree()
{
    m_buildTree();
}

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

void DecisionTree::Update()
{
    m_LOSNode->setLOS(m_pAgent->hasLOS());
    m_RadiusNode->setIsWithinRadius(m_pAgent->withinRadius());
    m_CloseCobatNode->setIsWithinCombatRange(m_pAgent->withinCombatRange());
}

// in-order traversal
std::string DecisionTree::MakeDecision()
{
    Update();

    auto currentNode = m_treeNodeList[0]; // start at root node

    while (!currentNode->isLeaf)
    {
        currentNode = (currentNode->data) ? (currentNode->Right) : (currentNode->Left);
    }
    if (currentNode->getNodeType() == ACTION_NODE)
        static_cast<ActionNode*>(currentNode)->Action();
    return currentNode->name;
}

void DecisionTree::m_buildTree()
{
    // add the root node
    m_LOSNode = new LOSCondition();
    m_LOSNode->setNodeType(ROOT_NODE);
    m_treeNodeList.push_back(m_LOSNode); // Root Node, LOS Condition

    m_RadiusNode = new RadiusCondition();
    m_RadiusNode->setNodeType(CONDITION_NODE);
    AddNode(m_LOSNode, m_RadiusNode, LEFT_TREE_NODE);
    m_treeNodeList.push_back(m_RadiusNode); // Radius Condition, if LOS FAILS

    m_CloseCobatNode = new CloseCombatCondition();
    m_RadiusNode->setNodeType(CONDITION_NODE);
    AddNode(m_LOSNode, m_CloseCobatNode, RIGHT_TREE_NODE);
    m_treeNodeList.push_back(m_CloseCobatNode); // Close Combat Condition, if LOS SUCCEEDS

    TreeNode* patrolNode = AddNode(m_RadiusNode, new PatrolAction(), LEFT_TREE_NODE);
    patrolNode->setNodeType(ACTION_NODE);
    m_treeNodeList.push_back(patrolNode); // Patrol Action if Radius Condition FAILS

    TreeNode* moveToLOSNode = AddNode(m_RadiusNode, new MoveToLOSAction(), RIGHT_TREE_NODE);
    moveToLOSNode->setNodeType(ACTION_NODE);
    m_treeNodeList.push_back(moveToLOSNode); // Move To LOS action, if Radius Condition SUCCEEDS

    TreeNode* moveToPlayerNode = AddNode(m_CloseCobatNode, new MoveToPlayerAction(), LEFT_TREE_NODE);
    moveToPlayerNode->setNodeType(ACTION_NODE);
    m_treeNodeList.push_back(moveToPlayerNode); // Move To Player Action, if Close Combat Condition FAILS

    TreeNode* attackNode = AddNode(m_CloseCobatNode, new AttackAction(), RIGHT_TREE_NODE);
    attackNode->setNodeType(ACTION_NODE);
    m_treeNodeList.push_back(attackNode); // Attack Action, if Close Combat Condition SUCCEEDS
}
