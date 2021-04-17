#include "MoveToLOSAction.h"
#include "Enemy.h"
#include "Zombie.h"
#include "Pigman.h"
#include "Util.h"
#include <iostream>

MoveToLOSAction::MoveToLOSAction()
{
	name = "Move to LOS Action";
}

MoveToLOSAction::~MoveToLOSAction()
= default;

void MoveToLOSAction::Action()
{
	auto destination = getAgent()->getDestination();
	auto currentRotation = getAgent()->getCurrentHeading();

	static_cast<Enemy*>(getAgent())->move(destination, currentRotation);

	if (getAgent()->getType() == ZOMBIE) {
		if (static_cast<Zombie*>(getAgent())->getState() != ZOMBIE_DAMAGED && static_cast<Zombie*>(getAgent())->getState() != ZOMBIE_DEATH)
			static_cast<Zombie*>(getAgent())->setState(ZOMBIE_WALK);
	}
	else {
		if (static_cast<Pigman*>(getAgent())->getState() != PIGMAN_DAMAGED && static_cast<Pigman*>(getAgent())->getState() != PIGMAN_DEATH)
			static_cast<Pigman*>(getAgent())->setState(PIGMAN_WALK);
	}
}
