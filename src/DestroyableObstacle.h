#pragma once
#ifndef __DESTROYABLE_OBSTACLE__
#define __DESTROYABLE_OBSTACLE__
#include "Obstacle.h"
#include "HealthBar.h"

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

	void startDespawnTimer();
	void updateDespawnTimer();
	int getDespawnTimer();

	void setIsDestroyed(bool destroyed);
	bool isDestroyed();

private:
	virtual void m_buildAnimations();

	bool m_debugMode;
	HealthBar m_health;

	int m_despawnTimer;
	bool m_isDestroyed;
};

#endif /* defined (__OBSTACLE__) */