#include "MoveToRangeAction.h"
#include "Util.h"
#include "SoundManager.h"
#include "Enemy.h"
#include "Pigman.h"

MoveToRangeAction::MoveToRangeAction()
{
	name = "Move To Range Action";
}

MoveToRangeAction::~MoveToRangeAction()
= default;

void MoveToRangeAction::Action()
{
	auto destination = static_cast<Enemy*>(getAgent())->getStevePosition() + glm::vec2(20.0f, 20.0f);

	if (Util::distance(static_cast<Enemy*>(getAgent())->getStevePosition(), getAgent()->getTransform()->position))
	{

	}

	auto currentRotation = getAgent()->getCurrentHeading();

	static_cast<Enemy*>(getAgent())->move(destination, currentRotation);

	if (static_cast<Pigman*>(getAgent())->getState() != PIGMAN_DAMAGED && static_cast<Pigman*>(getAgent())->getState() != PIGMAN_DEATH)
		static_cast<Pigman*>(getAgent())->setState(PIGMAN_WALK);
}
