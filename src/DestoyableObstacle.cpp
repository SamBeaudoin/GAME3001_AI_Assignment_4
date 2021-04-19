#include "DestroyableObstacle.h"
#include "Renderer.h"
#include "SoundManager.h"
#include "TextureManager.h"

DestroyableObstacle::DestroyableObstacle(glm::vec2 pos) : Obstacle(pos, "bricks")
{
	TextureManager::Instance()->loadSpriteSheet("../Assets/sprites/Explosion_Animations.txt", "../Assets/sprites/Explosions.png", "explosion");
	setSpriteSheet(TextureManager::Instance()->getSpriteSheet("explosion"));

	m_health = HealthBar(3, pos);
	setType(DESTROYABLE_OBJECT);

	m_buildAnimations();
}

DestroyableObstacle::~DestroyableObstacle()
= default;

void DestroyableObstacle::draw()
{
	if (!m_isDestroyed)
		Obstacle::draw();
	else
		TextureManager::Instance()->playAnimation("explosion", getAnimation("destroyed"), getTransform()->position.x, getTransform()->position.y, 4.0f, NULL, 255, true);

	m_health.draw();
}

void DestroyableObstacle::update()
{
	Obstacle::update();
}

void DestroyableObstacle::clean()
{
	Obstacle::clean();
}

bool DestroyableObstacle::getDebugMode() const
{
	return m_debugMode;
}

void DestroyableObstacle::setDebugMode(bool mode)
{
	m_debugMode = mode;
}

void DestroyableObstacle::takeDamage()
{
	if (m_health.getHealth() > 0)
		m_health.takeDamage();
}

int DestroyableObstacle::getHealth()
{
	return m_health.getHealth();
}

void DestroyableObstacle::startDespawnTimer()
{
	m_despawnTimer = 30;
}

void DestroyableObstacle::updateDespawnTimer()
{
	m_despawnTimer--;
}

int DestroyableObstacle::getDespawnTimer()
{
	return m_despawnTimer;
}

void DestroyableObstacle::setIsDestroyed(bool destroyed)
{
	m_isDestroyed = destroyed;

	if (destroyed)
		startDespawnTimer();
}

bool DestroyableObstacle::isDestroyed()
{
	return m_isDestroyed;
}

void DestroyableObstacle::m_buildAnimations()
{
	Animation destroyed = Animation();
	destroyed.name = "destroyed";

	for (int i = 0; i < 16; i++)
		destroyed.frames.push_back(getSpriteSheet()->getFrame("explosion" + std::to_string(i)));

	setAnimation(destroyed);
}
