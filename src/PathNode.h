#pragma once
#ifndef __PATH_NODE__
#define __PATH_NODE__
#include "Agent.h"

class PathNode : public Agent
{
public:
	PathNode();
	~PathNode();

	// DisplayObject Life Cycle Functions
	void draw() override;
	void update() override;
	void clean() override;

	void setHasEnemyLOS(bool state);
	bool hasEnemyLOS();
	glm::vec2 getNodeMiddle();
	void setNodeMiddle(glm::vec2 location);
private:
	bool m_enemyLOS;
	glm::vec2 m_middleOfNode;
};

#endif /* defined (__PATH_NODE__) */