#pragma once
#ifndef __TREE_NODE__
#define __TREE_NODE__
#include <string>
#include "Agent.h"
#include "NodeType.h"

struct TreeNode
{
	TreeNode() = default;
	virtual ~TreeNode() = default;

	std::string name = "";
	bool data = false;

	TreeNode* Left = nullptr;	// Left Child
	TreeNode* Right = nullptr;	// Right Child
	TreeNode* Parent = nullptr;	// So we can traverse back
	bool isLeaf = false;

	// Agent in Treenode?
	Agent* getAgent();
	NodeType getNodeType();
	void setAgent(Agent* agent);
	void setNodeType(NodeType type);

private:
	Agent* m_agent;
	NodeType m_nodeType;
};

#endif /* defined (__TREE_NODE__) */