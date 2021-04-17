#include "WaitBehindCoverAction.h"
#include "Util.h"
#include "Zombie.h"
#include "Pigman.h"
#include "SoundManager.h"
#include <iostream>

WaitBehindCoverAction::WaitBehindCoverAction()
{
	name = "Wait Behind Cover Action";
}

WaitBehindCoverAction::~WaitBehindCoverAction()
= default;

void WaitBehindCoverAction::Action()
{
	static_cast<Pigman*>(getAgent())->UpdateHideCooldown();

	if (static_cast<Pigman*>(getAgent())->getState() != PIGMAN_DAMAGED && static_cast<Pigman*>(getAgent())->getState() != PIGMAN_DEATH)
		static_cast<Pigman*>(getAgent())->setState(PIGMAN_IDLE);
}
