#include "GoldenNuggs.h"
#include "TextureManager.h"
#include "Util.h"

GoldenNugg::GoldenNugg(glm::vec2 pos, float rotation)
{
	TextureManager::Instance()->load("../Assets/sprites/Gold_Nug.png", "Nuggies");

	auto size = TextureManager::Instance()->getTextureSize("Nuggies");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = pos;
	m_turningAngle = rotation;

	setType(PROJECTILE);
	getRigidBody()->isColliding = false;

	m_speed = 5.0f;
}

GoldenNugg::~GoldenNugg()
{
}

void GoldenNugg::setTurningAngle(float angle)
{
	m_turningAngle = angle;
}

void GoldenNugg::setSpeed(float speed)
{
	m_speed = speed;
}

float GoldenNugg::getTurningAngle()
{
	return m_turningAngle;
}

float GoldenNugg::getSpeed()
{
	return m_speed;
}

void GoldenNugg::draw()
{
	TextureManager::Instance()->draw("Nuggies",
		getTransform()->position.x, getTransform()->position.y, (m_turningAngle + 90.0f), 255, true);
}

void GoldenNugg::update()
{
	getRigidBody()->velocity.x = cos(m_turningAngle * Util::Deg2Rad) * m_speed;
	getRigidBody()->velocity.y = sin(m_turningAngle * Util::Deg2Rad) * m_speed;
	getTransform()->position += getRigidBody()->velocity;
}

void GoldenNugg::clean()
{
}
