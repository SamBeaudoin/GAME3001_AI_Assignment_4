#pragma once
#ifndef __DECISION_TREE__
#define __DECISION_TREE__
#include "TreeNode.h"
#include "TreeNodeType.h"
#include "LOSCondition.h"
#include "RadiusCondition.h"
#include "CloseCombatCondition.h"
#include "Agent.h"

class DecisionTree
{
public:
	DecisionTree();
	~DecisionTree();

	//getters and setters
	Agent* getAgent();
	void setAgent(Agent* agent);

	// convenince funtion
	TreeNode* AddNode(TreeNode* parent, TreeNode* child_node, TreeNodeType type);
	void DisplayTree();
	void Update();

	std::string MakeDecision();	// in order traversal


private:
	Agent* m_pAgent;

	LOSCondition* m_LOSNode;
	RadiusCondition* m_RadiusNode;
	CloseCombatCondition* m_CloseCobatNode;

	void m_buildTree();

	std::vector<TreeNode*> m_treeNodeList;
};

#endif /* defined (__DECISION_TREE__) */