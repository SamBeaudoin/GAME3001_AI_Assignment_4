#include "RangedCombatDecisionTree.h"
#include "PatrolAction.h"
#include "MoveToLOSAction.h"
#include "MoveToRangeAction.h"
#include "RangedAttackAction.h"
#include "FleeAction.h"
#include "MoveToCoverAction.h"
#include "WaitBehindCoverAction.h"
#include <iostream>

RangedCombatDecisionTree::RangedCombatDecisionTree() : DecisionTree()
{
    m_buildTree();
}

RangedCombatDecisionTree::~RangedCombatDecisionTree()
= default;

void RangedCombatDecisionTree::Update()
{
    m_LOSSearchNode->setLOS(m_pAgent->hasLOS());
    m_RadiusNode->setIsWithinRadius(m_pAgent->withinRadius());
    m_HealthNode->setIsWithinHealthRange(m_pAgent->getIsWithinHealthThreshold());
    m_WithinRangeNode->setIsWithinRange(m_pAgent->getIsWithinRange());
    m_HideCooldownNode->setIsCooldownRunning(m_pAgent->getIsHideCooldownRunning());
    m_LOSFleeNode->setLOS(m_pAgent->hasLOS());
}

void RangedCombatDecisionTree::m_buildTree()
{
    // add the root node
    m_HealthNode = new HealthCheckCondition();
    m_HealthNode->setNodeType(ROOT_NODE);
    m_treeNodeList.push_back(m_HealthNode); // Health Check Condition, Root Node

    TreeNode* fleeNode = AddNode(m_HealthNode, new FleeAction(), LEFT_TREE_NODE);
    fleeNode->setNodeType(ACTION_NODE);
    m_treeNodeList.push_back(fleeNode); // Flee Action if Health Check fails

}