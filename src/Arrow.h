#pragma once
#ifndef __ARROW__
#define __ARROW__
#include "DisplayObject.h"

class Arrow : public DisplayObject
{
public:
	// constructors
	Arrow(glm::vec2 pos, float rotation);

	// destructor
	~Arrow();

	//setters
	void setTurningAngle(float angle);
	void setSpeed(float speed);

	//getteres
	float getTurningAngle();
	float getSpeed();

	// life cycle functions
	void draw() override;
	void update() override;
	void clean() override;
private:
	float m_turningAngle;
	float m_speed;
};

#endif /* defined (__ARROW__) */