#pragma once
#ifndef __STEVE_TOO_CLOSE_CONDITION__
#define __STEVE_TOO_CLOSE_CONDITION__
#include "ConditionNode.h"
#include <string>

class SteveTooCloseCondition : public ConditionNode
{
public:
	SteveTooCloseCondition(bool Too_close = false);
	virtual ~SteveTooCloseCondition();

	void setIsTooClose(bool state);

	bool Condition() override;
private:
	bool m_isTooClose;
};
#endif /* defined (__STEVE_TOO_CLOSE_CONDITION__) */