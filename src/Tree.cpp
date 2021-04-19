#include "Tree.h"

Tree::Tree() : Obstacle() {}

Tree::Tree(glm::vec2 pos) : Obstacle(pos, "obstacle")
{
	//getTransform()->position = pos;
}

Tree::~Tree() = default;

void Tree::update() {
	Obstacle::update();
}

void Tree::draw()
{
	Obstacle::draw();
}

void Tree::clean()
{
	Obstacle::clean();
}
