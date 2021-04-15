#pragma once
#include "Enemy.h"
#include "PigmanState.h"
#include "CloseCombatDecisionTree.h"

#ifndef __PIGMAN__
#define __PIGMAN__

class Pigman : public Enemy
{
public:

	Pigman();
	~Pigman();

	// Inherited via GameObject
	void draw() override;
	void update() override;
	void clean() override;

	// getters
	PigmanState getState() const;
	int getCooldown() const;
	int getDespawnTimer() const;

	// setters
	void setState(PigmanState state);
	void resetCooldown();
	void StartDespawnTimer();

private:
	virtual void m_buildAnimations();
	int m_cooldown;
	int m_despawnTimer;
	int m_soundCooldown;

	//State
	PigmanState m_state;
};

#endif /* defined (__PIGMAN__) */
