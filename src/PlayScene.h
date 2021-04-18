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
#include "Pigman.h"
#include "MapNodes.h"
#include "Arrow.h"
#include "GoldenNuggs.h"
#include "Background.h"
#include "PathNode.h"
#include "WinMenu.h"
#include "DestroyableObstacle.h"

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
	std::vector<Zombie*> m_pZombieArmy;
	std::vector<Pigman*> m_pPigmanSquad;
	std::vector<Obstacle*> m_pObstacles;
	DestroyableObstacle* m_pDestroyable;
	std::vector<MapNodes*> m_pMapNodes;
	std::vector<Arrow*> m_pArrowQuiver;
	std::vector<GoldenNugg*> m_pGoldenNuggies;
	std::vector<DisplayObject*> m_pLOSDisplayObjects;
	std::vector<Enemy*> m_pGangOfEnemies;
	void m_CheckForLOS(Agent* first_object, DisplayObject* target_object);

	bool m_enemyNeedsSpawn;

	Label* m_pHealth;
	Label* m_pZombieCount;
	Label* m_pPigmanCount;
	WinMenu* m_pWinMenu;
	Background* m_pBackground;

	bool m_debugToggle;
	bool m_zombieWalkToggle;
	bool m_pigmanWalkToggle;

	std::vector<PathNode*> m_pGrid;
	void m_buildGrid();
	bool m_CheckForEnemyLOS(PathNode* node, Enemy* enemy);

	void m_CheckPathNodeLOS();

	bool m_gridVisible;

	void m_findClosestPathNodeWithLOS(Agent* agent);
	void m_findClosestPathNodeWithoutLOS(Agent* agent);
};
#endif /* defined (__PLAY_SCENE__) */
