#pragma once
#ifndef __FLEE_ACTION__
#define __FLEE_ACTION__
#include "ActionNode.h"

class FleeAction : public ActionNode
{
public:
	FleeAction();
	virtual ~FleeAction();

	virtual void Action() override;

private:
	glm::vec2 m_playerPosition;
};

#endif /* defined (__FLEE_ACTION__) */