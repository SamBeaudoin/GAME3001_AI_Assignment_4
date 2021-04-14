#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Button.h"
#include "Label.h"
#include "Obstacle.h"
#include "Tree.h"
#include "SpaceShip.h"
#include "Target.h"
#include "DecisionTree.h"
#include "Steve.h"
#include "Zombie.h"
#include "MapNodes.h"
#include "Arrow.h"
#include "Background.h"

class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene();

	// Scene LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;

	void updateCollisions();
	void loadSounds();

private:
	// IMGUI Function
	void GUI_Function();
	std::string m_guiTitle;
	glm::vec2 m_mousePosition;
	bool m_isGridEnabled;

	Target* m_pTarget;
	Steve* m_pSteve;
	std::vector<Enemy*> m_pZombieArmy;
	std::vector<Obstacle*> m_pObstacles;
	std::vector<MapNodes*> m_pMapNodes;
	std::vector<Arrow*> m_pArrowQuiver;
	void m_CheckForLOS(Agent* first_object, DisplayObject* target_object);

	Label* m_pHealth;
	Label* m_pZombieCount;
	Background* m_pBackground;

	bool m_debugToggle;
	bool m_zombieWalkToggle;
};



#endif /* defined (__PLAY_SCENE__) */
