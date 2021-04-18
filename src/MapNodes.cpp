#include "MapNodes.h"
#include "Renderer.h"

MapNodes::MapNodes(){}

MapNodes::MapNodes(glm::vec2 pos)
{
	getTransform()->position = pos;
	setHeight(40);
	setWidth(40);
	setType(MAP_NODE);
	m_middleOfNode = { getTransform()->position.x + 20, getTransform()->position.y + 20};
}


MapNodes::~MapNodes(){}

bool MapNodes::getDebugMode()
{
	return m_DebugMode;
}

glm::vec2 MapNodes::getNodeMiddle()
{
	return m_middleOfNode;
}

void MapNodes::setDebugMode(bool mode)
{
	m_DebugMode = mode;
}

void MapNodes::draw()
{
	if (getDebugMode())
	{
		SDL_Rect node = { getTransform()->position.x, getTransform()->position.y, getWidth(), getHeight() };
		SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 0, 0, 0, 255);
		SDL_RenderDrawRect(Renderer::Instance()->getRenderer(), &node);
	}
}

void MapNodes::update()
{
}

void MapNodes::clean()
{
}
