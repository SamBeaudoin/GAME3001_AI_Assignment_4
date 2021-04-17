#pragma once
#ifndef __ENEMY_H__
#define __ENEMY_H__
#include "Agent.h"
#include "DecisionTree.h"

class Enemy : public Agent {
public:

	Enemy();
	~Enemy();

	// Inherited via GameObject
	void draw() override;
	void update() override;
	void clean() override;

	//getters
	int getHealth();
	glm::vec2 getStevePosition();
	glm::vec2 getTargetPosition();
	int getAttackRange();

	// setters
	void setStevePosition(glm::vec2 position);
	void setTargetPosition(glm::vec2 position);

	void takeDamage();
	void MakeDecision();

	void move(glm::vec2 destination, float currentRotation);
	glm::vec2 turn(glm::vec2 destination, float currentRotation);

private:
	void m_checkBounds();
	virtual void m_buildAnimations() = 0;

	float m_maxSpeed;
	float m_turnRate;
	int m_health;
	glm::vec2 m_stevePosition;
	glm::vec2 m_targetPosition;

protected:
	DecisionTree* m_decisionTree;
	int m_attackRange;
};

#endif // !__ENEMY_H__
