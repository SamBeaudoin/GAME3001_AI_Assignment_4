#include "Arrow.h"
#include "TextureManager.h"
#include "Util.h"

Arrow::Arrow(glm::vec2 pos, float rotation)
{
	TextureManager::Instance()->load("../Assets/sprites/Arrow.png", "arrow");

	auto size = TextureManager::Instance()->getTextureSize("arrow");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = pos;
	m_turningAngle = rotation;

	setType(PROJECTILE);
	getRigidBody()->isColliding = false;

	m_speed = 6.0f;
}

Arrow::~Arrow()
{
}

void Arrow::setTurningAngle(float angle)
{
	m_turningAngle = angle;
}

void Arrow::setSpeed(float speed)
{
	m_speed = speed;
}

float Arrow::getTurningAngle()
{
	return m_turningAngle;
}

float Arrow::getSpeed()
{
	return m_speed;
}

void Arrow::draw()
{
	TextureManager::Instance()->draw("arrow",
		getTransform()->position.x, getTransform()->position.y, (m_turningAngle + 90.0f), 255, true);
}

void Arrow::update()
{
	getRigidBody()->velocity.x = cos(m_turningAngle * Util::Deg2Rad) * m_speed;
	getRigidBody()->velocity.y = sin(m_turningAngle * Util::Deg2Rad) * m_speed;
	getTransform()->position += getRigidBody()->velocity;
}

void Arrow::clean()
{
}
