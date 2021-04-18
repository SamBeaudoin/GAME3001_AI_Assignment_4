#pragma once
#ifndef __RANGED_COMBAT_DECISION_TREE__
#define __RANGED_COMBAT_DECISION_TREE__
#include "DecisionTree.h"
#include "LOSCondition.h"
#include "RadiusCondition.h"
#include "HealthCheckCondition.h"
#include "RangedCombatCondition.h"
#include "HideCooldownCondition.h"
#include "WithinRangeCondition.h"
#include "SteveTooCloseCondition.h"


class RangedCombatDecisionTree : public DecisionTree
{
public:
	RangedCombatDecisionTree();
	~RangedCombatDecisionTree();

	virtual void Update();

private:
	LOSCondition* m_LOSSearchNode;
	LOSCondition* m_LOSFleeNode;
	RadiusCondition* m_RadiusNode;
	HealthCheckCondition* m_HealthNode;
	HideCooldownCondition* m_HideCooldownNode;
	WithinRangeCondition* m_WithinRangeNode;
	SteveTooCloseCondition* m_SteveTooCloseNode;

	virtual void m_buildTree();
};

#endif /* defined (__RANGED_COMBAT_DECISION_TREE__) */