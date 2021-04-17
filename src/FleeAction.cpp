#include "FleeAction.h"
#include "SoundManager.h"
#include "Util.h"
#include <iostream>
#include "Zombie.h"

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

	auto currentRotation = getAgent()->getCurrentHeading();

	//change new destination if already reached
	if (Util::distance(getAgent()->getTransform()->position, destination) < 20.0f) {
		getAgent()->setDestinationNode(getAgent()->NextNode());
	}

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
	if (getAgent()->getType() == ZOMBIE && static_cast<Zombie*>(getAgent())->getState() != ZOMBIE_DEATH) {
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
