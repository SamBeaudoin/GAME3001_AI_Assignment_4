#include "RangedCombatDecisionTree.h"
#include "PatrolAction.h"
#include "MoveToLOSAction.h"
#include "MoveToRangeAction.h"
#include "RangedAttackAction.h"
#include "FleeAction.h"
#include "MoveToCoverAction.h"
#include "WaitBehindCoverAction.h"
#include "CreateSpaceAction.h"
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
    m_WithinRangeNode->setIsWithinRange(m_pAgent->getIsWithinAttackRange());
    m_HideCooldownNode->setIsCooldownRunning(m_pAgent->getIsHideCooldownRunning());
    m_LOSFleeNode->setLOS(m_pAgent->hasLOS());
	m_SteveTooCloseNode->setIsTooClose(m_pAgent->getIsSteveTooClose());
}

void RangedCombatDecisionTree::m_buildTree()
{
    // add the root node
	m_HideCooldownNode = new HideCooldownCondition();
	m_HideCooldownNode->setNodeType(ROOT_NODE);
	m_treeNodeList.push_back(m_HideCooldownNode);	// Root Node, Check to see if hiding

	m_HealthNode = new HealthCheckCondition();
	m_HealthNode->setNodeType(CONDITION_NODE);
	AddNode(m_HideCooldownNode, m_HealthNode, RIGHT_TREE_NODE);
	m_treeNodeList.push_back(m_HealthNode);			// Health Check if Hide Succeeds

	m_LOSSearchNode = new LOSCondition();
	m_LOSSearchNode->setNodeType(CONDITION_NODE);
	AddNode(m_HideCooldownNode, m_LOSSearchNode, LEFT_TREE_NODE);
	m_treeNodeList.push_back(m_LOSSearchNode);		// LOS Check to see if hiding within LOS

	TreeNode* fleeNode = AddNode(m_HealthNode, new FleeAction(), LEFT_TREE_NODE);
	fleeNode->setNodeType(ACTION_NODE);
	m_treeNodeList.push_back(fleeNode);				// Flee Action if LOS Fails

	m_LOSFleeNode = new LOSCondition();
	m_LOSFleeNode->setNodeType(CONDITION_NODE);
	AddNode(m_HealthNode, m_LOSFleeNode, RIGHT_TREE_NODE);
	m_treeNodeList.push_back(m_LOSFleeNode);		// LOS Check to see if hiding within LOS

	TreeNode* moveBehindCoverNode = AddNode(m_LOSFleeNode, new MoveToCoverAction(), RIGHT_TREE_NODE);
	moveBehindCoverNode->setNodeType(ACTION_NODE);
	m_treeNodeList.push_back(moveBehindCoverNode);	// Move behind cover if Hiding check passes

	TreeNode* waitNode = AddNode(m_LOSFleeNode, new WaitBehindCoverAction(), LEFT_TREE_NODE);
	waitNode->setNodeType(ACTION_NODE);
	m_treeNodeList.push_back(waitNode);				// Wait if behind cover

	m_RadiusNode = new RadiusCondition();
	m_RadiusNode->setNodeType(CONDITION_NODE);
	AddNode(m_LOSSearchNode, m_RadiusNode, LEFT_TREE_NODE);
	m_treeNodeList.push_back(m_RadiusNode);			// Detection Radius

	m_WithinRangeNode = new WithinRangeCondition();
	m_WithinRangeNode->setNodeType(CONDITION_NODE);
	AddNode(m_LOSSearchNode, m_WithinRangeNode, RIGHT_TREE_NODE);
	m_treeNodeList.push_back(m_WithinRangeNode);	// Range to shoot check

	m_SteveTooCloseNode = new SteveTooCloseCondition();
	m_SteveTooCloseNode->setNodeType(CONDITION_NODE);
	AddNode(m_WithinRangeNode, m_SteveTooCloseNode, RIGHT_TREE_NODE);
	m_treeNodeList.push_back(m_SteveTooCloseNode);	// if Steve is all up in le grille

	TreeNode* createSpaceNode = AddNode(m_SteveTooCloseNode, new CreateSpaceAction(), RIGHT_TREE_NODE);
	createSpaceNode->setNodeType(ACTION_NODE);
	m_treeNodeList.push_back(createSpaceNode);		// create space from le Steve if in le grille

	TreeNode* patrolNode = AddNode(m_RadiusNode, new PatrolAction(), LEFT_TREE_NODE);
	patrolNode->setNodeType(ACTION_NODE);
	m_treeNodeList.push_back(patrolNode);			// Basic patrol action

	TreeNode* moveToLOSNode = AddNode(m_RadiusNode, new MoveToLOSAction(), RIGHT_TREE_NODE);
	moveToLOSNode->setNodeType(ACTION_NODE);
	m_treeNodeList.push_back(moveToLOSNode);		// Move to LOS to see steve

	TreeNode* moveToRange = AddNode(m_WithinRangeNode, new MoveToRangeAction(), LEFT_TREE_NODE);
	moveToRange->setNodeType(ACTION_NODE);
	m_treeNodeList.push_back(moveToRange);			// Move inot range to begin firing

	TreeNode* rangeAttack = AddNode(m_SteveTooCloseNode, new RangedAttackAction(), LEFT_TREE_NODE);
	rangeAttack->setNodeType(ACTION_NODE);
	m_treeNodeList.push_back(rangeAttack);			// fire de nuggies
}