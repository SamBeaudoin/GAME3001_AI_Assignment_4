#include "HealthBar.h"
#include "SDL.h"
#include "Renderer.h"
#include "TextureManager.h"

HealthBar::HealthBar(int health, glm::vec2 pos)
{
	getTransform()->position = pos;
	m_health = m_total = health;
}

HealthBar::~HealthBar()
{
}

void HealthBar::draw()
{
	int pieceOfBar = 60 / m_total;
	SDL_Rect rect = { getTransform()->position.x - 30, getTransform()->position.y - 50, pieceOfBar * m_health, 10 };

	SDL_Rect outline = { getTransform()->position.x - 30, getTransform()->position.y - 50, 60, 10 };

	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 0, 0, 255);
	SDL_RenderFillRect(Renderer::Instance()->getRenderer(), &rect);
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 0, 0, 0, 255);
	SDL_RenderDrawRect(Renderer::Instance()->getRenderer(), &outline);
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
}

void HealthBar::update()
{
}

void HealthBar::clean()
{
}

int HealthBar::getHealth()
{
	return m_health;
}

void HealthBar::setHealth(int health)
{
	m_health = health;
}

void HealthBar::takeDamage()
{
	m_health--;
}

glm::vec2 HealthBar::getPos()
{
	return getTransform()->position;
}

void HealthBar::setPos(glm::vec2 pos)
{
	getTransform()->position = pos;
}
