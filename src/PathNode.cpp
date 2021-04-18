#include "PathNode.h"
#include "TextureManager.h"
#include "Util.h"
#include "Config.h"

PathNode::PathNode()
{
	setWidth(10);
	setHeight(10);
	//getTransform()->position = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(PATH_NODE);

	setCurrentHeading(0.0f);// current facing angle
	setCurrentDirection(glm::vec2(1.0f, 0.0f)); // facing right

	m_middleOfNode = glm::vec2(getTransform()->position.x - 5, getTransform()->position.y - 5);

	setLOSDistance(1000.0f);
	setLOSColour(glm::vec4(1, 0, 0, 1));
}

PathNode::~PathNode()
= default;

void PathNode::draw()
{
	// draw node
	if (getDebugMode())
		Util::DrawRect(getTransform()->position - glm::vec2(5, 5), getWidth(), getHeight(), getLOSColour());
}

void PathNode::update()
{
}

void PathNode::clean()
{
}

void PathNode::setHasEnemyLOS(bool state)
{
	m_enemyLOS = state;
	if (hasLOS()) {
		if (state)
			setLOSColour(glm::vec4(0, 1, 0, 1));
		else
			setLOSColour(glm::vec4(1, 1, 0, 1));
	}
	else
		setLOSColour(glm::vec4(1, 0, 0, 1));
}

bool PathNode::hasEnemyLOS()
{
	return m_enemyLOS;
}

glm::vec2 PathNode::getNodeMiddle()
{
	return m_middleOfNode;
}

void PathNode::setNodeMiddle(glm::vec2 location)
{
	m_middleOfNode = location;
}
