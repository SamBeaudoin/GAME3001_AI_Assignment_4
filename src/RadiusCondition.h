#pragma once
#ifndef __RADIUS_CONDITION__
#define __RADIUS_CONDITION__
#include "ConditionNode.h"
#include <string>

// "Interface" or abstract Class
class RadiusCondition : public ConditionNode
{
public:
	RadiusCondition(bool within_radius = false);
	virtual ~RadiusCondition();

	void setIsWithinRadius(bool state);

	bool Condition() override;
private:
	bool m_isWithinRadius;

};

#endif /* defined (__RADIUS_CONDITION__) */#pragma once
