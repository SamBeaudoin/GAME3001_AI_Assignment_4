#include "Enemy.h"
#include "Config.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "Renderer.h"
#include "Util.h"
#include "Zombie.h"
#include "Pigman.h"

Enemy::Enemy() : Agent()
{
	setWidth(64);
	setHeight(64);

	getTransform()->position = glm::vec2(800.0f, 300.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;

	setCurrentHeading(180.0f); // current facing angle
	setCurrentDirection(glm::vec2(-1.0f, 0.0f)); // facing left
	m_turnRate = 5.0f; // 5 degrees per frame
	m_health = 4;
	m_detectRect = nullptr;

	setLOSDistance(300.0f); // 5 ppf x 80 feet
	setLOSColour(glm::vec4(1, 0, 0, 1));
	setHasLOS(false);	// initialize boolean
}

Enemy::~Enemy() = default;

void Enemy::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	if (getDebugMode())
	{
		// draw LOS to Steve
		Util::DrawLine((getTransform()->position), (m_stevePosition), getLOSColour());

		// draw radius circle
		Util::DrawCircle({ (getTransform()->position.x - 5),(getTransform()->position.y - 5) }, getLOSDistance(), glm::vec4(255, 105, 180, 255), SYMMETRICAL);

		SDL_Rect colliderBoundry = { getTransform()->position.x - getWidth()/2, getTransform()->position.y - getHeight()/2, getWidth(), getHeight() };
		SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 0, 0, 0, 255);
		SDL_RenderDrawRect(Renderer::Instance()->getRenderer(), &colliderBoundry);
	}

	SDL_Rect rect = { getTransform()->position.x - 30, getTransform()->position.y - 50, 15 * m_health, 10 };

	SDL_Rect outline = { getTransform()->position.x - 30, getTransform()->position.y - 50, 60, 10 };

	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 0, 0, 255);
	SDL_RenderFillRect(Renderer::Instance()->getRenderer(), &rect);
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 0, 0, 0, 255);
	SDL_RenderDrawRect(Renderer::Instance()->getRenderer(), &outline);
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
}

void Enemy::update()
{
	setIsWithinHealthThreshold(m_health > 1);
}

void Enemy::clean()
{
}

int Enemy::getHealth()
{
	return m_health;
}

glm::vec2 Enemy::getStevePosition()
{
	return m_stevePosition;
}

glm::vec2 Enemy::getTargetPosition()
{
	return m_targetPosition;
}

int Enemy::getAttackRange()
{
	return m_attackRange;
}

DetectRect* Enemy::getDetectRect()
{
	return m_detectRect;
}

void Enemy::setStevePosition(glm::vec2 position)
{
	m_stevePosition = position;
}

void Enemy::setTargetPosition(glm::vec2 position)
{
	m_targetPosition = position;
}

void Enemy::setDetectRect(DetectRect* rect)
{
	m_detectRect = rect;
}

void Enemy::takeDamage()
{
	if (m_health > 0)
		m_health -= 1;
}

void Enemy::MakeDecision()
{
	std::cout << m_decisionTree->MakeDecision() << std::endl;
}

void Enemy::move(glm::vec2 destination, float currentRotation)
{
	auto agent = m_decisionTree->getAgent();

	if (getDetectRect() != nullptr) {
		switch (getDetectRect()->getCollidingSide(agent))
		{
		case TOP_SIDE:
			std::cout << "TOP SIDE" << std::endl;
			destination.y = std::min(agent->getTransform()->position.y, destination.y);
			break;
		case BOTTOM_SIDE:
			std::cout << "BOTTOM SIDE" << std::endl;
			destination.y = std::max(agent->getTransform()->position.y, destination.y);
			break;
		case LEFT_SIDE:
			std::cout << "LEFT SIDE" << std::endl;
			destination.x = std::min(agent->getTransform()->position.x, destination.x);
			break;
		case RIGHT_SIDE:
			std::cout << "RIGHT SIDE" << std::endl;
			destination.x = std::max(agent->getTransform()->position.x, destination.x);
			break;
		default:
			std::cout << "NO SIDE" << std::endl;
			break;
		}
	}

	auto direction = turn(destination, currentRotation);

	//move if the agent, if they are of type Zombie (so far all of them are)
	if ((agent->getType() == ZOMBIE && static_cast<Zombie*>(agent)->getState() != ZOMBIE_DEATH && static_cast<Zombie*>(agent)->getState() != ZOMBIE_IDLE) ||
		(agent->getType() == PIGMAN && static_cast<Pigman*>(agent)->getState() != PIGMAN_DEATH && static_cast<Pigman*>(agent)->getState() != PIGMAN_IDLE)) {
		agent->getRigidBody()->velocity = direction * 2.0f;
		agent->setDistanceWalked(agent->getDistanceWalked() + 2.0f);
		agent->getTransform()->position += agent->getRigidBody()->velocity;

		//Sound
		if (agent->getDistanceWalked() >= 60) {
			agent->setDistanceWalked(agent->getDistanceWalked() - 60);
			SoundManager::Instance().playSound("grass" + std::to_string(rand() % 6));
		}
	}
}

glm::vec2 Enemy::turn(glm::vec2 destination, float currentRotation)
{
	auto agent = m_decisionTree->getAgent();

	//determine an orientation and angle
	auto direction = Util::normalize(destination - agent->getTransform()->position);
	auto orientation = glm::vec2(cos(currentRotation * Util::Deg2Rad), sin(currentRotation * Util::Deg2Rad));

	auto targetRotation = Util::signedAngle(orientation, direction);

	//turn tha agent accordingly
	if (abs(targetRotation) > 5.0f) {
		if (targetRotation > 0)
			agent->setCurrentHeading(currentRotation + 5.0f);
		else if (targetRotation < 0)
			agent->setCurrentHeading(currentRotation - 5.0f);
	}

	return direction;
}

void Enemy::m_checkBounds()
{

	if (getTransform()->position.x > Config::SCREEN_WIDTH)
	{
		getTransform()->position.x = Config::SCREEN_WIDTH;
	}

	if (getTransform()->position.x < 0)
	{
		getTransform()->position.x = 0;
	}

	if (getTransform()->position.y > Config::SCREEN_HEIGHT)
	{
		getTransform()->position.y = Config::SCREEN_HEIGHT;
	}

	if (getTransform()->position.y < 0)
	{
		getTransform()->position.y = 0;
	}
}
