#include "DetectRect.h"
#include "Renderer.h"
#include "CollisionManager.h"

DetectRect::DetectRect()
{
	m_detectRect = new SDL_Rect();
	m_detectRect->x = 300;
	m_detectRect->y = 300;
	m_detectRect->w = 0;
	m_detectRect->h = 0;
}

DetectRect::DetectRect(glm::vec2 position, int width, int height)
{
	m_detectRect = new SDL_Rect();
	m_detectRect->x = position.x - (width / 2);
	m_detectRect->y = position.y - (height / 2);
	m_detectRect->w = width;
	m_detectRect->h = height;
	getTransform()->position = glm::vec2(m_detectRect->x, m_detectRect->y);
	setWidth(width);
	setHeight(height);
}

DetectRect::~DetectRect()
{
	
}

void DetectRect::draw()
{
	SDL_RenderDrawRect(Renderer::Instance()->getRenderer(), m_detectRect);
}

void DetectRect::update()
{

}

void DetectRect::clean()
{
}

SDL_Rect* DetectRect::getDetectRect()
{
	return m_detectRect;
}

void DetectRect::setPos(glm::vec2 pos)
{
	const auto position = pos;
	m_detectRect->x = position.x - (m_detectRect->w / 2);
	m_detectRect->y = position.y - (m_detectRect->h / 2);
	getTransform()->position = glm::vec2(m_detectRect->x + getWidth() / 2, m_detectRect->y + getHeight() / 2);
}

CollidingSide DetectRect::getCollidingSide(DisplayObject* object)
{
	const auto position = object->getTransform()->position - glm::vec2(object->getWidth() / 2, object->getHeight() / 2);
	//const auto obstacle = getObstacle()->getTransform()->position = glm::vec2(getObstacle()->getWidth() / 2, getObstacle()->getHeight() / 2);
	const auto detection = getTransform()->position - glm::vec2(getWidth() / 2, getHeight() / 2);

	if (position.x < (detection.x + getWidth() - 5) && position.x > (detection.x + 5) && position.y > detection.y)
		return BOTTOM_SIDE;
	else if (position.x < (detection.x + getWidth() - 15) && position.x > (detection.x + 5) && position.y < detection.y)
		return TOP_SIDE;
	else if (position.y < (detection.y + getHeight() - 5) && position.y > (detection.y + 5) && position.x > detection.x)
		return RIGHT_SIDE;
	else if (position.y < (detection.y + getHeight() - 5) && position.y > (detection.y + 5) && position.x < detection.x)
		return LEFT_SIDE;
	return NO_SIDE;

	//if (CollisionManager::lineRectCheck(glm::vec2(detection.x, detection.y), glm::vec2(detection.x + getWidth(), detection.y),
	//	position, getWidth(), getHeight()))
	//	return TOP_SIDE;
	//else if (CollisionManager::lineRectCheck(glm::vec2(detection.x, detection.y + getHeight()), glm::vec2(detection.x + getWidth(), detection.y + getHeight()),
	//	position, getWidth(), getHeight()))
	//	return BOTTOM_SIDE;
	//else if (CollisionManager::lineRectCheck(glm::vec2(detection.x, detection.y), glm::vec2(detection.x, detection.y + getHeight()),
	//	position, getWidth(), getHeight()))
	//	return LEFT_SIDE;
	//else if (CollisionManager::lineRectCheck(glm::vec2(detection.x + getWidth(), detection.y), glm::vec2(detection.x + getWidth(), detection.y + getHeight()),
	//	position, getWidth(), getHeight()))
	//	return RIGHT_SIDE;

}
