#include "Obstacle.h"
#include "Renderer.h"
#include "SoundManager.h"
#include "TextureManager.h"

Obstacle::Obstacle()
{
	TextureManager::Instance()->load("../Assets/sprites/MineCraft_Bush.png", "obstacle");

	auto size = TextureManager::Instance()->getTextureSize("obstacle");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(300.0f, 300.0f);

	setType(OBSTACLE);
	getRigidBody()->isColliding = false;

	SoundManager::Instance().load("../Assets/audio/yay.ogg", "yay", SOUND_SFX);
}

Obstacle::~Obstacle()
= default;

void Obstacle::draw()
{
	TextureManager::Instance()->draw("obstacle", 
		getTransform()->position.x, getTransform()->position.y, 0, 255, true);

	if (getDebugMode()) {
		SDL_Rect colliderBoundry = { getTransform()->position.x - (getWidth() / 2), getTransform()->position.y - (getHeight() / 2), getWidth(), getHeight() };
		SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 0, 0, 0, 255);
		SDL_RenderDrawRect(Renderer::Instance()->getRenderer(), &colliderBoundry);
	}
}

void Obstacle::update()
{
}

void Obstacle::clean()
{
}

bool Obstacle::getDebugMode() const
{
	return m_debugMode;
}

void Obstacle::setDebugMode(bool mode)
{
	m_debugMode = mode;
}
