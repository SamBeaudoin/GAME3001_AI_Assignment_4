#include "DestroyableObstacle.h"
#include "Renderer.h"
#include "SoundManager.h"
#include "TextureManager.h"

DestroyableObstacle::DestroyableObstacle(glm::vec2 pos) : Obstacle(pos, "bricks")
{
	m_health = 3;
	setType(DESTROYABLE_OBJECT);
}

DestroyableObstacle::~DestroyableObstacle()
= default;

void DestroyableObstacle::draw()
{
	Obstacle::draw();

	SDL_Rect rect = { getTransform()->position.x - 30, getTransform()->position.y - 50, 20 * m_health, 10 };

	SDL_Rect outline = { getTransform()->position.x - 30, getTransform()->position.y - 50, 60, 10 };

	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 0, 0, 255);
	SDL_RenderFillRect(Renderer::Instance()->getRenderer(), &rect);
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 0, 0, 0, 255);
	SDL_RenderDrawRect(Renderer::Instance()->getRenderer(), &outline);
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
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
	if (m_health > 0)
		m_health -= 1;
}

int DestroyableObstacle::getHealth()
{
	return m_health;
}
