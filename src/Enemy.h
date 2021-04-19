#pragma once
#ifndef __ENEMY_H__
#define __ENEMY_H__
#include "Agent.h"
#include "DecisionTree.h"
#include "DetectRect.h"
#include "HealthBar.h"

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
	DetectRect* getDetectRect();

	// setters
	void setStevePosition(glm::vec2 position);
	void setTargetPosition(glm::vec2 position);
	void setDetectRect(DetectRect* rect);

	void takeDamage();
	void MakeDecision();

	void move(glm::vec2 destination, float currentRotation, bool reverse = false);
	glm::vec2 turn(glm::vec2 destination, float currentRotation, bool reverse = false);

private:
	void m_checkBounds();
	virtual void m_buildAnimations() = 0;

	float m_maxSpeed;
	float m_turnRate;
	glm::vec2 m_stevePosition;
	glm::vec2 m_targetPosition;

protected:
	DecisionTree* m_decisionTree;
	int m_attackRange;
	DetectRect* m_detectRect;
	HealthBar m_health;
};

#endif // !__ENEMY_H__
