#include "DestroyableObstacle.h"
#include "Renderer.h"
#include "SoundManager.h"
#include "TextureManager.h"

DestroyableObstacle::DestroyableObstacle(glm::vec2 pos) : Obstacle(pos, "bricks")
{
	TextureManager::Instance()->loadSpriteSheet("../Assets/sprites/Explosions_Animation.png", "../Assets/sprites/Explosions.png", "explosion");
	setSpriteSheet(TextureManager::Instance()->getSpriteSheet("explosion"));

	m_health = HealthBar(3, pos);
	setType(DESTROYABLE_OBJECT);
}

DestroyableObstacle::~DestroyableObstacle()
= default;

void DestroyableObstacle::draw()
{
	Obstacle::draw();
	m_health.draw();
	//SDL_Rect rect = { getTransform()->position.x - 30, getTransform()->position.y - 50, 20 * m_health, 10 };

	//SDL_Rect outline = { getTransform()->position.x - 30, getTransform()->position.y - 50, 60, 10 };

	//SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 0, 0, 255);
	//SDL_RenderFillRect(Renderer::Instance()->getRenderer(), &rect);
	//SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 0, 0, 0, 255);
	//SDL_RenderDrawRect(Renderer::Instance()->getRenderer(), &outline);
	//SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
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

void DestroyableObstacle::m_buildAnimations()
{
	Animation destroyed = Animation();
	destroyed.name = "destroyed";

	for (int i = 0; i < 16; i++)
		destroyed.frames.push_back(getSpriteSheet()->getFrame("explosion" + std::to_string(i)));

	setAnimation(destroyed);
}
