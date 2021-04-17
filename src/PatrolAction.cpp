#include "PatrolAction.h"
#include "Util.h"
#include "Enemy.h"
#include "Zombie.h"
#include "Pigman.h"
#include <iostream>

PatrolAction::PatrolAction()
{
	name = "Patrol Action";
}

PatrolAction::~PatrolAction()
= default;

void PatrolAction::Action()
{
	auto destination = getAgent()->getDestinationNode()->getNodeMiddle();
	auto currentRotation = getAgent()->getCurrentHeading();

	//change new destination if already reached
	if (Util::distance(getAgent()->getTransform()->position, destination) < 20.0f) {
		getAgent()->setDestinationNode(getAgent()->NextNode());
	}

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
