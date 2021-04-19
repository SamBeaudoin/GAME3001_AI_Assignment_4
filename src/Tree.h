#pragma once
#ifndef __TREE__
#define __TREE__
#include "Obstacle.h"

class Tree : public Obstacle {
public:
	Tree();
	Tree(glm::vec2 pos);
	~Tree();

	virtual void update() override;
	virtual void draw() override;
	virtual void clean() override;

private:
	virtual void m_buildAnimations();
};

#endif