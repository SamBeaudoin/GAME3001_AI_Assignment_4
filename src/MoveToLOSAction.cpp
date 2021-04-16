#include "MoveToLOSAction.h"
#include "SoundManager.h"
#include "Util.h"
#include <iostream>
#include "Zombie.h"
#include "Pigman.h"

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

	//determine an orientation and angle
	auto direction = Util::normalize(destination - getAgent()->getTransform()->position);
	auto orientation = glm::vec2(cos(currentRotation * Util::Deg2Rad), sin(currentRotation * Util::Deg2Rad));

	auto targetRotation = Util::signedAngle(orientation, direction);

	//turn tha agent accordingly
	if (abs(targetRotation) > 5.0f) {
		if (targetRotation > 0)
			getAgent()->setCurrentHeading(currentRotation + 5.0f);
		else if (targetRotation < 0)
			getAgent()->setCurrentHeading(currentRotation + 5.0f);
	}

	//move if the agent, if they are of type Zombie (so far all of them are)
	if ((getAgent()->getType() == ZOMBIE && static_cast<Zombie*>(getAgent())->getState() != ZOMBIE_DEATH) || 
		(getAgent()->getType() == PIGMAN && static_cast<Pigman*>(getAgent())->getState() != PIGMAN_DEATH)) {
		getAgent()->getRigidBody()->velocity = direction * 2.0f;
		getAgent()->setDistanceWalked(getAgent()->getDistanceWalked() + 2.0f);
		getAgent()->getTransform()->position += getAgent()->getRigidBody()->velocity;

		//Sound
		if (getAgent()->getDistanceWalked() >= 60) {
			getAgent()->setDistanceWalked(getAgent()->getDistanceWalked() - 60);
			SoundManager::Instance().playSound("grass" + std::to_string(rand() % 6));
		}
	}
}
