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
private:
	bool m_enemyLOS;
};

#endif /* defined (__PATH_NODE__) */