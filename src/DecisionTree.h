#pragma once
#ifndef __DECISION_TREE__
#define __DECISION_TREE__
#include "TreeNode.h"
#include "TreeNodeType.h"
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
	virtual void Update() = 0;

	std::string MakeDecision();	// in order traversal

	std::vector<TreeNode*> m_treeNodeList;
	Agent* m_pAgent;

private:

	virtual void m_buildTree() = 0;
};

#endif /* defined (__DECISION_TREE__) */