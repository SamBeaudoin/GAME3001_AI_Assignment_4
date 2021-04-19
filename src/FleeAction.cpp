#include "FleeAction.h"
#include "Util.h"
#include <iostream>
#include "Enemy.h"
#include "Zombie.h"
#include "Pigman.h"

FleeAction::FleeAction()
{
	name = "Flee Action";
}

FleeAction::~FleeAction()
= default;

void FleeAction::Action()
{
	auto destination = static_cast<Enemy*>(getAgent())->getStevePosition() + glm::vec2(20.0f, 20.0f);
	auto currentRotation = getAgent()->getCurrentHeading();

	static_cast<Enemy*>(getAgent())->move(destination, currentRotation, true);
	
	if (getAgent()->getType() == ZOMBIE) {
		if (static_cast<Zombie*>(getAgent())->getState() != ZOMBIE_DAMAGED && static_cast<Zombie*>(getAgent())->getState() != ZOMBIE_DEATH)
			static_cast<Zombie*>(getAgent())->setState(ZOMBIE_WALK);
	}
	else {
		if (static_cast<Pigman*>(getAgent())->getState() != PIGMAN_DAMAGED && static_cast<Pigman*>(getAgent())->getState() != PIGMAN_DEATH)
			static_cast<Pigman*>(getAgent())->setState(PIGMAN_WALK);
	}
}
