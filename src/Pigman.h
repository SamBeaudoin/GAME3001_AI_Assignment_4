#pragma once
#include "Enemy.h"
#include "PigmanState.h"
#include "RangedCombatDecisionTree.h"

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
	int getHideCooldown() const;

	// setters
	void setState(PigmanState state);
<<<<<<< HEAD
	void startHideCooldown();
	void UpdateHideCooldown();
	
=======
	void setHideCooldown(int x);
>>>>>>> 21eefb24dd2236214ec65afceaa5e65d3693bbbd
	void resetCooldown();
	void StartDespawnTimer();

private:
	virtual void m_buildAnimations();
	int m_cooldown;
	int m_hideCooldown;
	int m_despawnTimer;
	int m_soundCooldown;
	int m_hideCooldown;

	//State
	PigmanState m_state;
};

#endif /* defined (__PIGMAN__) */
