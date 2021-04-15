#pragma once
#ifndef __HEALTH_CHECK_CONDITION__
#define __HEALTH_CHECK_CONDITION__
#include "ConditionNode.h"

class HealthCheckCondition : public ConditionNode
{
public:
	HealthCheckCondition(bool within_combat_range = false);
	virtual ~HealthCheckCondition();

	void setIsWithinHealthRange(bool state);

	virtual bool Condition() override;
private:
	bool m_isHealthAboveThreshold;

};

#endif /* defined (__HEALTH_CHECK_CONDITION__) */