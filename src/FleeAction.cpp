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
	glm::vec2 destination;
	// In the bottom half
	if (getAgent()->getTransform()->position.y > 300)
	{
		destination = glm::vec2(getAgent()->getTransform()->position.x, 700);
	}
	else
	{
		destination = glm::vec2(getAgent()->getTransform()->position.x, -100);
	}

<<<<<<< HEAD
	auto destination = getAgent()->getTransform()->position;
=======
>>>>>>> 21eefb24dd2236214ec65afceaa5e65d3693bbbd
	auto currentRotation = getAgent()->getCurrentHeading();

	if (destination.y >= 300)
		destination.y = 700;
	else
		destination.y = -100;

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
