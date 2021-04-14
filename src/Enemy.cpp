#include "Enemy.h"
#include "Config.h"
#include "TextureManager.h"
#include "Renderer.h"
#include "Util.h"

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
	m_health = 3;

	setLOSDistance(350.0f); // 5 ppf x 80 feet
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

		SDL_Rect colliderBoundry = { getTransform()->position.x - 35, getTransform()->position.y - 35, getWidth(), getHeight() };
		SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 0, 0, 0, 255);
		SDL_RenderDrawRect(Renderer::Instance()->getRenderer(), &colliderBoundry);
	}

	SDL_Rect rect = { getTransform()->position.x - 30, getTransform()->position.y - 50, 20 * m_health, 10 };

	SDL_Rect outline = { getTransform()->position.x - 30, getTransform()->position.y - 50, 60, 10 };

	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 0, 0, 255);
	SDL_RenderFillRect(Renderer::Instance()->getRenderer(), &rect);
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 0, 0, 0, 255);
	SDL_RenderDrawRect(Renderer::Instance()->getRenderer(), &outline);
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
}

void Enemy::update()
{
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

void Enemy::setStevePosition(glm::vec2 position)
{
	m_stevePosition = position;
}

void Enemy::setTargetPosition(glm::vec2 position)
{
	m_targetPosition = position;
}

void Enemy::takeDamage()
{
	if (m_health > 0)
		m_health -= 1;
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
