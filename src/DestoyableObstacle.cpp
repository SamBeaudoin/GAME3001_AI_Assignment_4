#include "DestroyableObstacle.h"
#include "Renderer.h"
#include "SoundManager.h"
#include "TextureManager.h"

DestroyableObstacle::DestroyableObstacle()
{
	TextureManager::Instance()->load("../Assets/sprites/MineCraft_Bush.png", "obstacle");

	auto size = TextureManager::Instance()->getTextureSize("obstacle");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(300.0f, 300.0f);

	m_health = 3;

	setType(OBSTACLE);
	getRigidBody()->isColliding = false;

	SoundManager::Instance().load("../Assets/audio/yay.ogg", "yay", SOUND_SFX);
}

DestroyableObstacle::~DestroyableObstacle()
= default;

void DestroyableObstacle::draw()
{
	TextureManager::Instance()->draw("obstacle",
		getTransform()->position.x, getTransform()->position.y, 0, 255, true);

	if (getDebugMode()) {
		SDL_Rect colliderBoundry = { getTransform()->position.x - (getWidth() / 2), getTransform()->position.y - (getHeight() / 2), getWidth(), getHeight() };
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

void DestroyableObstacle::update()
{
}

void DestroyableObstacle::clean()
{
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
