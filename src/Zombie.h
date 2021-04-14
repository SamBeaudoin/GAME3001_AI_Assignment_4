#pragma once
#include "Enemy.h"
#include "ZombieState.h"
#include "DecisionTree.h"

#ifndef __ZOMBIE__
#define __ZOMBIE__

class Zombie : public Enemy
{
public:
	
	Zombie();
	~Zombie();

	// Inherited via GameObject
	void draw() override;
	void update() override;
	void clean() override;

	// getters
	ZombieState getState() const;
	int getCooldown() const;
	int getDespawnTimer() const;

	// setters
	void setState(ZombieState state);
	void resetCooldown();
	void StartDespawnTimer();

private:
	virtual void m_buildAnimations();
	int m_cooldown;
	int m_despawnTimer;
	int m_soundCooldown;

	//State
	ZombieState m_state;
};

#endif /* defined (__ZOMBIE__) */