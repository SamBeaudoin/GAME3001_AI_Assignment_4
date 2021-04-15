#include "Agent.h"
#include "glm/gtx/string_cast.hpp"
#include "PlayScene.h"
#include "TextureManager.h"
#include "Util.h"
#include <iostream>

Agent::Agent() {
	m_decisionTree = new DecisionTree();
	m_decisionTree->setAgent(this);
}

Agent::~Agent() = default;

// getters
glm::vec2 Agent::getTargetPosition() const
{
	return m_targetPosition;
}

glm::vec2 Agent::getCurrentDirection() const
{
	return m_currentDirection;
}

float Agent::getLOSDistance() const
{
	return m_LOSDistance;
}

bool Agent::hasLOS() const
{
	return m_hasLOS;
}

bool Agent::withinRadius() const
{
	return m_isWithinRadius;
}

bool Agent::withinCombatRange() const
{
	return (m_radiusDistance <= 50.0f ? true : false);
}

float Agent::getRadiusDistance() const
{
	return m_radiusDistance;
}

float Agent::getCurrentHeading() const
{
	return m_currentHeading;
}

glm::vec4 Agent::getLOSColour() const
{
	return m_LOSColour;
}

bool Agent::getDebugMode() const
{
	return m_debugMode;
}

float Agent::getDistanceWalked() const
{
	return m_distanceWalked;
}

bool Agent::getIsWithinHealthThreshold() const
{
	return m_isWithinHealthRange;
}

MapNodes* Agent::getDestinationNode() const
{
	return m_destinationNode;
}

std::vector<MapNodes*> Agent::getNodes() const
{
	return m_pNodes;
}

void Agent::setLOSDistance(const float distance)
{
	m_LOSDistance = distance;
}

void Agent::setHasLOS(const bool state)
{
	m_hasLOS = state;
	m_LOSColour = (m_hasLOS) ? glm::vec4(0, 1, 0, 1) : glm::vec4(1, 0, 0, 1);
}

void Agent::setIsWithinRadius(bool state)
{
	m_isWithinRadius = state;
}

void Agent::setRadiusDistance(float distance)
{
	m_radiusDistance = distance;
}

void Agent::setCurrentHeading(const float heading)
{
	m_currentHeading = heading;
	m_changeDirection();
}

void Agent::setTargetPosition(glm::vec2 newPosition)
{
	m_targetPosition = newPosition;
}

void Agent::setCurrentDirection(glm::vec2 newDirection)
{
	m_currentDirection = newDirection;
}

void Agent::setLOSColour(const glm::vec4 colour)
{
	m_LOSColour = colour;
}

void Agent::setDebugMode(bool mode)
{
	m_debugMode = mode;
}

void Agent::setDestinationNode(MapNodes* destination)
{
	m_destinationNode = destination;
}

void Agent::setDistanceWalked(float distance)
{
	m_distanceWalked = distance;
}

void Agent::AddNode(MapNodes* node)
{
	m_pNodes.push_back(node);
}

MapNodes* Agent::NextNode()
{
	m_nodeCounter++;
	if (m_nodeCounter == m_pNodes.size())
		m_nodeCounter = 0;
	return m_pNodes[m_nodeCounter];
}

void Agent::setIsWithinHealthThreshold(bool state)
{
	m_isWithinHealthRange = state;
}

void Agent::MakeDecision()
{
	std::cout << m_decisionTree->MakeDecision() << std::endl;
}

void Agent::m_changeDirection()
{
	const auto x = cos(m_currentHeading * Util::Deg2Rad);
	const auto y = sin(m_currentHeading * Util::Deg2Rad);
	m_currentDirection = glm::vec2(x, y);
}
