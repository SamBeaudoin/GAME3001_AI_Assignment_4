#include "RangedAttackAction.h"
#include "Util.h"
#include <iostream>
#include "Enemy.h"
#include "Pigman.h"

RangedAttackAction::RangedAttackAction()
{
	name = "Ranged Attack Action";
}

RangedAttackAction::~RangedAttackAction()
= default;

void RangedAttackAction::Action()
{
	auto destination = static_cast<Enemy*>(getAgent())->getStevePosition() + glm::vec2(20.0f, 20.0f);
	auto currentRotation = getAgent()->getCurrentHeading();

	static_cast<Enemy*>(getAgent())->turn(destination, currentRotation);

	if (static_cast<Pigman*>(getAgent())->getState() != PIGMAN_DAMAGED && static_cast<Pigman*>(getAgent())->getState() != PIGMAN_DEATH)
		static_cast<Pigman*>(getAgent())->setState(PIGMAN_ATTACK);
}
