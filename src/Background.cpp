#include "Background.h"
#include "TextureManager.h"

Background::Background()
{
	TextureManager::Instance()->load("../Assets/textures/Grass_Background.png", "grassBackground");
	auto size = TextureManager::Instance()->getTextureSize("grassBackground");

	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(0.0f, 0.0f);
}

Background::~Background() = default;

void Background::draw()
{
	TextureManager::Instance()->draw("grassBackground", getTransform()->position.x, getTransform()->position.y, 0, 255, false);
}

void Background::update()
{
}

void Background::clean()
{
}
