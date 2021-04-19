#pragma once
#ifndef __HEALTH_BAR__
#define __HEALTH_BAR__
#include "DisplayObject.h"

class HealthBar : public DisplayObject {
public:
	HealthBar() {};
	HealthBar(int health, glm::vec2 pos);
	~HealthBar();

	virtual void draw();
	virtual void update();
	virtual void clean();

	int getHealth();
	void setHealth(int health);
	void takeDamage();

	glm::vec2 getPos();
	void setPos(glm::vec2 pos);

private:
	int m_health;
	int m_total;
};

#endif // !__HEALTH_BAR__
