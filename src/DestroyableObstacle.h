#pragma once
#ifndef __DESTROYABLE_OBSTACLE__
#define __DESTROYABLE_OBSTACLE__
#include "Obstacle.h"

class DestroyableObstacle : public Obstacle
{
public:
	// constructors
	DestroyableObstacle(glm::vec2 pos);

	// destructor
	~DestroyableObstacle();

	// life cycle functions
	void draw() override;
	void update() override;
	void clean() override;

	bool getDebugMode() const;
	void setDebugMode(bool mode);

	void takeDamage();
	int getHealth();

private:
	bool m_debugMode;
	int m_health;
};

#endif /* defined (__OBSTACLE__) */