#pragma once
#ifndef __CLOSE_COMBAT_DECISION_TREE__
#define __CLOSE_COMBAT_DECISION_TREE__
#include "DecisionTree.h"
#include "LOSCondition.h"
#include "RadiusCondition.h"
#include "CloseCombatCondition.h"
#include "HealthCheckCondition.h"


class CloseCombatDecisionTree : public DecisionTree
{
public:
	CloseCombatDecisionTree();
	~CloseCombatDecisionTree();

	virtual void Update();

private:
	LOSCondition* m_LOSNode;
	RadiusCondition* m_RadiusNode;
	HealthCheckCondition* m_HealthNode;
	CloseCombatCondition* m_CloseCombatNode;

	virtual void m_buildTree();

};

#endif /* defined (__CLOSE_COMBAT_DECISION_TREE__) */