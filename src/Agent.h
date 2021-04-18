#pragma once
#pragma once
#ifndef __AGENT__
#define __AGENT__

#include "Sprite.h"
#include <glm/vec4.hpp>
#include "MapNodes.h"

class Agent : public Sprite
{
public:
	friend class DecisionTree;

	Agent();
	~Agent();

	// Inherited via GameObject
	void draw() override = 0;
	void update() override = 0;
	void clean() override = 0;

	// getters
	glm::vec2 getTargetPosition() const;
	glm::vec2 getDestination() const;
	glm::vec2 getCurrentDirection() const;
	float getLOSDistance() const;
	bool hasLOS() const;
	bool withinRadius() const;
	bool withinCombatRange() const;
	float getRadiusDistance() const;
	float getCurrentHeading() const;
	glm::vec4 Agent::getLOSColour() const;
	bool getDebugMode() const;
	float getDistanceWalked() const;
	bool getIsWithinHealthThreshold() const;
	bool getIsWithinAttackRange() const;
	bool getIsHideCooldownRunning() const;
	bool getIsSteveTooClose() const;

	MapNodes* getDestinationNode() const;
	std::vector<MapNodes*> getNodes() const;

	// setters
	void setTargetPosition(glm::vec2 newPosition);
	void setCurrentDirection(glm::vec2 newDirection);
	void setLOSDistance(float distance);
	void setHasLOS(bool state);
	void setIsWithinRadius(bool state);
	void setRadiusDistance(float distance);
	void setCurrentHeading(float heading);
	void setLOSColour(glm::vec4 colour);
	void setDebugMode(bool mode);
	void setDestinationNode(MapNodes* destination);

	void setDestinationNode(glm::vec2 destination);

	void setDistanceWalked(float distance);
	void AddNode(MapNodes* node);
	MapNodes* NextNode();
	void setIsWithinHealthThreshold(bool state);
	void setIsWithinAttackRange(bool state);
	void setIsHideCooldownRunning(bool state);
	void setisSteveTooClose(bool state);


private:
	void m_changeDirection();
	float m_currentHeading;
	glm::vec2 m_currentDirection;
	glm::vec2 m_targetPosition;

	// DeBug view
	bool m_debugMode;

	// LOS
	float m_LOSDistance;
	bool m_hasLOS;
	glm::vec4 m_LOSColour;

	//Within Radius
	bool m_isWithinRadius;
	float m_radiusDistance;

	// Steve Too Close Check
	bool m_SteveTooClose;

	// Health Check
	bool m_isWithinHealthRange;

	// Range Check
	bool m_isWithinAttackRange;

	// Hide Cooldown Check
	bool m_isHideCooldownRunning;

	glm::vec2 m_destination;

	//other
	MapNodes* m_destinationNode;
	std::vector<MapNodes*> m_pNodes;
	int m_nodeCounter;
	int m_distanceWalked;

//protected:
//	//Decision Tree
//	DecisionTree* m_decisionTree;
};


#endif /* defined (__AGENT__) */

