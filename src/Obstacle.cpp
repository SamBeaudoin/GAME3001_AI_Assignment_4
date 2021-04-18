#include "Obstacle.h"
#include "Renderer.h"
#include "SoundManager.h"
#include "TextureManager.h"

Obstacle::Obstacle(glm::vec2 pos, std::string sprite)
{
	TextureManager::Instance()->load("../Assets/sprites/MineCraft_Bush.png", "obstacle");
	TextureManager::Instance()->load("../Assets/sprites/Cracked_Bricks.png", "bricks");

	m_sprite = sprite;

	auto size = TextureManager::Instance()->getTextureSize(sprite);
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = pos;
	m_detectRect = new DetectRect(getTransform()->position, size.x + 60, size.y + 60);

	setType(OBSTACLE);
	getRigidBody()->isColliding = false;
}

Obstacle::~Obstacle() = default;

void Obstacle::draw()
{
	TextureManager::Instance()->draw(m_sprite,
		getTransform()->position.x, getTransform()->position.y, 0, 255, true);

	if (getDebugMode()) {
		SDL_Rect colliderBoundry = { getTransform()->position.x - (getWidth() / 2), getTransform()->position.y - (getHeight() / 2), getWidth(), getHeight() };
		SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 0, 0, 0, 255);
		SDL_RenderDrawRect(Renderer::Instance()->getRenderer(), &colliderBoundry);
		m_detectRect->draw();
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

DetectRect* Obstacle::GetDetection()
{
	return m_detectRect;
}

void Obstacle::UpdateDetection()
{
	m_detectRect->setPos(getTransform()->position);
}
