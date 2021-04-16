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
	m_HideCooldownNode = new HideCooldownCondition();
	m_HideCooldownNode->setNodeType(ROOT_NODE);
	m_treeNodeList.push_back(m_HideCooldownNode);

	m_HealthNode = new HealthCheckCondition();
	m_HealthNode->setNodeType(CONDITION_NODE);
	AddNode(m_HideCooldownNode, m_HealthNode, RIGHT_TREE_NODE);
	m_treeNodeList.push_back(m_HealthNode);

	m_LOSSearchNode = new LOSCondition();
	m_LOSSearchNode->setNodeType(CONDITION_NODE);
	AddNode(m_HideCooldownNode, m_LOSSearchNode, LEFT_TREE_NODE);
	m_treeNodeList.push_back(m_LOSSearchNode);

	TreeNode* fleeNode = AddNode(m_HealthNode, new FleeAction(), LEFT_TREE_NODE);
	fleeNode->setNodeType(ACTION_NODE);
	m_treeNodeList.push_back(fleeNode);

	m_LOSFleeNode = new LOSCondition();
	m_LOSFleeNode->setNodeType(CONDITION_NODE);
	AddNode(m_HealthNode, m_LOSFleeNode, RIGHT_TREE_NODE);
	m_treeNodeList.push_back(m_LOSFleeNode);

	TreeNode* moveBehindCoverNode = AddNode(m_LOSFleeNode, new MoveToCoverAction(), RIGHT_TREE_NODE);
	moveBehindCoverNode->setNodeType(ACTION_NODE);
	m_treeNodeList.push_back(moveBehindCoverNode);

	TreeNode* waitNode = AddNode(m_LOSFleeNode, new WaitBehindCoverAction(), LEFT_TREE_NODE);
	waitNode->setNodeType(ACTION_NODE);
	m_treeNodeList.push_back(waitNode);

	m_RadiusNode = new RadiusCondition();
	m_RadiusNode->setNodeType(CONDITION_NODE);
	AddNode(m_LOSSearchNode, m_RadiusNode, LEFT_TREE_NODE);
	m_treeNodeList.push_back(m_RadiusNode);

	m_WithinRangeNode = new WithinRangeCondition();
	m_WithinRangeNode->setNodeType(CONDITION_NODE);
	AddNode(m_LOSSearchNode, m_WithinRangeNode, RIGHT_TREE_NODE);
	m_treeNodeList.push_back(m_WithinRangeNode);

	TreeNode* patrolNode = AddNode(m_RadiusNode, new PatrolAction(), LEFT_TREE_NODE);
	patrolNode->setNodeType(ACTION_NODE);
	m_treeNodeList.push_back(patrolNode);

	TreeNode* moveToLOSNode = AddNode(m_RadiusNode, new MoveToLOSAction(), RIGHT_TREE_NODE);
	moveToLOSNode->setNodeType(ACTION_NODE);
	m_treeNodeList.push_back(moveToLOSNode);

	TreeNode* moveToRange = AddNode(m_WithinRangeNode, new MoveToRangeAction(), LEFT_TREE_NODE);
	moveToRange->setNodeType(ACTION_NODE);
	m_treeNodeList.push_back(moveToRange);

	TreeNode* rangeAttack = AddNode(m_WithinRangeNode, new RangedAttackAction(), RIGHT_TREE_NODE);
	rangeAttack->setNodeType(ACTION_NODE);
	m_treeNodeList.push_back(rangeAttack);
}