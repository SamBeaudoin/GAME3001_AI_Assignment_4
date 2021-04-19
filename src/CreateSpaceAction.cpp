#include "CreateSpaceAction.h"
#include "Util.h"
#include <iostream>
#include "Enemy.h"
#include "Zombie.h"
#include "Pigman.h"

CreateSpaceAction::CreateSpaceAction()
{
	name = "Create Space Action";
}

CreateSpaceAction::~CreateSpaceAction()
= default;

void CreateSpaceAction::Action()
{
	auto destination = static_cast<Enemy*>(getAgent())->getStevePosition() + glm::vec2(20.0f, 20.0f);
	auto currentRotation = getAgent()->getCurrentHeading();

	static_cast<Enemy*>(getAgent())->move(destination, currentRotation, true);

	if (static_cast<Pigman*>(getAgent())->getState() != PIGMAN_DAMAGED && static_cast<Pigman*>(getAgent())->getState() != PIGMAN_DEATH)
		static_cast<Pigman*>(getAgent())->setState(PIGMAN_WALK);
}
