#include "MoveToCoverAction.h"
#include "Util.h"
#include "Zombie.h"
#include "Pigman.h"
#include "SoundManager.h"
#include <iostream>

MoveToCoverAction::MoveToCoverAction()
{
	name = "Move To Cover Action";
}

MoveToCoverAction::~MoveToCoverAction()
= default;

void MoveToCoverAction::Action()
{
	auto destination = getAgent()->getDestinationNode()->getNodeMiddle();
	auto currentRotation = getAgent()->getCurrentHeading();

	static_cast<Enemy*>(getAgent())->move(destination, currentRotation);

	if (static_cast<Pigman*>(getAgent())->getState() != PIGMAN_DAMAGED && static_cast<Pigman*>(getAgent())->getState() != PIGMAN_DEATH)
		static_cast<Pigman*>(getAgent())->setState(PIGMAN_WALK);
}
