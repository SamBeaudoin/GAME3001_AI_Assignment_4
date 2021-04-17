#include "AttackAction.h"
#include "Util.h"
#include <iostream>
#include "Enemy.h"
#include "Zombie.h"

AttackAction::AttackAction()
{
	name = "Attack Action";
}

AttackAction::~AttackAction()
= default;

void AttackAction::Action()
{
	auto destination = static_cast<Enemy*>(getAgent())->getStevePosition() + glm::vec2(20.0f, 20.0f);
	auto currentRotation = getAgent()->getCurrentHeading();

	static_cast<Enemy*>(getAgent())->turn(destination, currentRotation);

	if (static_cast<Zombie*>(getAgent())->getState() != ZOMBIE_DAMAGED && static_cast<Zombie*>(getAgent())->getState() != ZOMBIE_DEATH)
		static_cast<Zombie*>(getAgent())->setState(ZOMBIE_ATTACK);
}
