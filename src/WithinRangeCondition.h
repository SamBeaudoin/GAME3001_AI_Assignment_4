#pragma once
#ifndef __WITHIN_RANGE_CONDITION__
#define __WITHIN_RANGE_CONDITION__
#include "ConditionNode.h"
#include <string>

// "Interface" or abstract Class
class WithinRangeCondition : public ConditionNode
{
public:
	WithinRangeCondition(bool within_radius = false);
	virtual ~WithinRangeCondition();

	void setIsWithinRange(bool state);

	bool Condition() override;
private:
	bool m_isWithinRange;
};
#endif /* defined (__WITHIN_RANGE_CONDITION__) */