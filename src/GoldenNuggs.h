#pragma once
#ifndef __GOLDEN_NUGGS__
#define __GOLDEN_NUGGS__
#include "DisplayObject.h"

class GoldenNugg : public DisplayObject
{
public:
	// constructors
	GoldenNugg(glm::vec2 pos, float rotation);

	// destructor
	~GoldenNugg();

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

#endif /* defined (__GOLDEN_NUGGS__) */