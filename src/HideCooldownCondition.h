#pragma once
#ifndef __HIDE_COOLDOWN_CONDITION__
#define __HIDE_COOLDOWN_CONDITION__
#include "ConditionNode.h"
#include <string>

// "Interface" or abstract Class
class HideCooldownCondition : public ConditionNode
{
public:
	HideCooldownCondition(bool within_radius = false);
	virtual ~HideCooldownCondition();

	void setIsCooldownRunning(bool state);

	bool Condition() override;
private:
	bool m_isCooldownRunning;
};
#endif /* defined (__HIDE_COOLDOWN_CONDITION__) */