#include "CloseCombatDecisionTree.h"
#include "PatrolAction.h"
#include "MoveToLOSAction.h"
#include "MoveToPlayerAction.h"
#include "AttackAction.h"
#include "FleeAction.h"
#include <iostream>

CloseCombatDecisionTree::CloseCombatDecisionTree() : DecisionTree()
{
    m_buildTree();
}

CloseCombatDecisionTree::~CloseCombatDecisionTree()
= default;

void CloseCombatDecisionTree::Update()
{
    m_LOSNode->setLOS(m_pAgent->hasLOS());
    m_RadiusNode->setIsWithinRadius(m_pAgent->withinRadius());
    m_CloseCombatNode->setIsWithinCombatRange(m_pAgent->withinCombatRange());
    m_HealthNode->setIsWithinHealthRange(m_pAgent->getIsWithinHealthThreshold());
}

void CloseCombatDecisionTree::m_buildTree()
{
    // add the root node
    m_HealthNode = new HealthCheckCondition();
    m_HealthNode->setNodeType(ROOT_NODE);
    m_treeNodeList.push_back(m_HealthNode); // Health Check Condition, Root Node

    TreeNode* fleeNode = AddNode(m_HealthNode, new FleeAction(), LEFT_TREE_NODE);
    fleeNode->setNodeType(ACTION_NODE);
    m_treeNodeList.push_back(fleeNode); // Flee Action if Health Check fails

    m_LOSNode = new LOSCondition();
    AddNode(m_HealthNode, m_LOSNode, RIGHT_TREE_NODE);
    m_LOSNode->setNodeType(CONDITION_NODE);
    m_treeNodeList.push_back(m_LOSNode); // LOS Condition, if Health Check Succeeds 

    m_RadiusNode = new RadiusCondition();
    m_RadiusNode->setNodeType(CONDITION_NODE);
    AddNode(m_LOSNode, m_RadiusNode, LEFT_TREE_NODE);
    m_treeNodeList.push_back(m_RadiusNode); // Radius Condition, if LOS FAILS

    m_CloseCombatNode = new CloseCombatCondition();
	m_CloseCombatNode->setNodeType(CONDITION_NODE);
    AddNode(m_LOSNode, m_CloseCombatNode, RIGHT_TREE_NODE);
    m_treeNodeList.push_back(m_CloseCombatNode); // Close Combat Condition, if LOS SUCCEEDS

    TreeNode* patrolNode = AddNode(m_RadiusNode, new PatrolAction(), LEFT_TREE_NODE);
    patrolNode->setNodeType(ACTION_NODE);
    m_treeNodeList.push_back(patrolNode); // Patrol Action if Radius Condition FAILS

    TreeNode* moveToLOSNode = AddNode(m_RadiusNode, new MoveToLOSAction(), RIGHT_TREE_NODE);
    moveToLOSNode->setNodeType(ACTION_NODE);
    m_treeNodeList.push_back(moveToLOSNode); // Move To LOS action, if Radius Condition SUCCEEDS

    TreeNode* moveToPlayerNode = AddNode(m_CloseCombatNode, new MoveToPlayerAction(), LEFT_TREE_NODE);
    moveToPlayerNode->setNodeType(ACTION_NODE);
    m_treeNodeList.push_back(moveToPlayerNode); // Move To Player Action, if Close Combat Condition FAILS

    TreeNode* attackNode = AddNode(m_CloseCombatNode, new AttackAction(), RIGHT_TREE_NODE);
    attackNode->setNodeType(ACTION_NODE);
    m_treeNodeList.push_back(attackNode); // Attack Action, if Close Combat Condition SUCCEEDS
}