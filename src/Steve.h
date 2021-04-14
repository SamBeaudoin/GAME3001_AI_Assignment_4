#pragma once
#ifndef __STEVE__
#define __STEVE__

#include "Agent.h"
#include "TextureManager.h"
#include <glm/vec4.hpp>
#include "SteveState.h"

class Steve final : public Agent
{
public:
	friend class Zombie;
	Steve();
	~Steve();

	// Inherited via GameObject
	void draw() override;
	void update() override;
	void clean() override;

	// Movement Functions
	void turnRight();
	void turnLeft();
	void moveForward();
	void moveBack();
	void move();
	void faceMouse();

	// getters
	float getMaxSpeed() const;
	SteveState getState() const;
	int getArrowCooldown() const;
	int getSwordSoundCooldown() const;

	// setters
	void setMaxSpeed(float newSpeed);
	void setState(SteveState state);
	void setArrowCooldown(int cooldown);
	void setSwordSoundCooldown(int cooldown);

private:
	void m_checkBounds();
	void m_reset();
	void m_buildAnimations();

	// steering behaviours
	float m_maxSpeed;
	float m_turnRate;
	int m_health;

	//others
	int m_arrowCooldown;
	int m_swordSoundCooldown;

	//State
	SteveState m_state;
};


#endif /* defined (__STEVE__) */
