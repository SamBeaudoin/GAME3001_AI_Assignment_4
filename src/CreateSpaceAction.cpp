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
	auto destination = getAgent()->getTransform()->position;	// If flee is updated update this aswell
	auto currentRotation = getAgent()->getCurrentHeading();

	if (destination.y >= 300)
		destination.y = 700;
	else
		destination.y = -100;

	static_cast<Enemy*>(getAgent())->move(destination, currentRotation);

	if (static_cast<Pigman*>(getAgent())->getState() != PIGMAN_DAMAGED && static_cast<Pigman*>(getAgent())->getState() != PIGMAN_DEATH)
		static_cast<Pigman*>(getAgent())->setState(PIGMAN_WALK);
}
