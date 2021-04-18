#pragma once
#ifndef __CREATE_SPACE_ACTION__
#define __CREATE_SPACE_ACTION__
#include "ActionNode.h"

class CreateSpaceAction : public ActionNode
{
public:
	CreateSpaceAction();
	virtual ~CreateSpaceAction();

	virtual void Action() override;
private:

};

#endif /* defined (__CREATE_SPACE_ACTION__) */