#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	if(EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();	
	}

	drawDisplayList();

	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
}

void PlayScene::update()
{
	updateDisplayList();

	// Node Updates
	m_CheckPathNodeLOS();

	// Obtacle Collision
	updateCollisions();

	// Steve Facing
	m_pSteve->faceMouse();

		if (m_pWinMenu->getWin())
		{
			m_pWinMenu->update();
			return;
		}

	// For Pigman within range

	for (int i = 0; i < m_pZombieArmy.size(); i++)
	{
		//update LOS for zombies and make a decision
		m_pZombieArmy[i]->setStevePosition(m_pSteve->getTransform()->position);
		m_CheckForLOS(m_pZombieArmy[i], m_pSteve);
		m_pZombieArmy[i]->MakeDecision();

		//range for checking if Steve is within range of Zombie When attacking
		m_CheckForLOS(m_pSteve, m_pZombieArmy[i]);
		if (m_pSteve->getState() == STEVE_ATTACK &&
			(m_pZombieArmy[i]->getType() == ZOMBIE && static_cast<Zombie*>(m_pZombieArmy[i])->getState() != ZOMBIE_DAMAGED) &&
			m_pSteve->hasLOS() && static_cast<Zombie*>(m_pZombieArmy[i])->getState() != ZOMBIE_DEATH)
		{
			m_pZombieArmy[i]->takeDamage();

			if (m_pZombieArmy[i]->getHealth() <= 0) {
				SoundManager::Instance().playSound("zombieDeath");
				static_cast<Zombie*>(m_pZombieArmy[i])->StartDespawnTimer();
				static_cast<Zombie*>(m_pZombieArmy[i])->setState(ZOMBIE_DEATH);
			}
			else {
				SoundManager::Instance().playSound("zombieHurt" + std::to_string(rand() % 2));
				static_cast<Zombie*>(m_pZombieArmy[i])->resetCooldown();
				static_cast<Zombie*>(m_pZombieArmy[i])->setState(ZOMBIE_DAMAGED);
			}
		}

		//remove zombie when their despawn timer is up
		if (static_cast<Zombie*>(m_pZombieArmy[i])->getDespawnTimer() == 0 && static_cast<Zombie*>(m_pZombieArmy[i])->getState() == ZOMBIE_DEATH) {
			removeChild(m_pZombieArmy[i]);
			m_pZombieArmy[i] = nullptr;
			m_pZombieArmy.erase(m_pZombieArmy.begin() + i);
			m_pZombieArmy.shrink_to_fit();
			m_pZombieCount->setText("Zombie Count: " + std::to_string(m_pZombieArmy.size()));
			break;
		}
	}

	for (int i = 0; i < m_pPigmanSquad.size(); i++)
	{
		//update LOS for pigmans and make a decision
		m_pPigmanSquad[i]->setStevePosition(m_pSteve->getTransform()->position);
		m_CheckForLOS(m_pPigmanSquad[i], m_pSteve);

		m_pPigmanSquad[i]->MakeDecision();

		//range for checking if Steve is within range of Pigman When attacking
		m_CheckForLOS(m_pSteve, m_pPigmanSquad[i]);
		if (m_pSteve->getState() == STEVE_ATTACK &&
			(m_pPigmanSquad[i]->getType() == PIGMAN && static_cast<Pigman*>(m_pPigmanSquad[i])->getState() != PIGMAN_DAMAGED) &&
			m_pSteve->hasLOS() && static_cast<Pigman*>(m_pPigmanSquad[i])->getState() != PIGMAN_DEATH)
		{

			m_pPigmanSquad[i]->takeDamage();

			if (m_pPigmanSquad[i]->getHealth() <= 0) {
				SoundManager::Instance().playSound("pigmanDeath");
				static_cast<Pigman*>(m_pPigmanSquad[i])->StartDespawnTimer();
				static_cast<Pigman*>(m_pPigmanSquad[i])->setState(PIGMAN_DEATH);
			}
			else {
				SoundManager::Instance().playSound("pigmanHurt" + std::to_string(rand() % 2));
				static_cast<Pigman*>(m_pPigmanSquad[i])->resetCooldown();
				static_cast<Pigman*>(m_pPigmanSquad[i])->startHideCooldown();
				static_cast<Pigman*>(m_pPigmanSquad[i])->setState(PIGMAN_DAMAGED);
			}
		}

		//remove pigman when their despawn timer is up
		if (static_cast<Pigman*>(m_pPigmanSquad[i])->getDespawnTimer() == 0 && static_cast<Pigman*>(m_pPigmanSquad[i])->getState() == PIGMAN_DEATH) {
			removeChild(m_pPigmanSquad[i]);
			m_pPigmanSquad[i] = nullptr;
			m_pPigmanSquad.erase(m_pPigmanSquad.begin() + i);
			m_pPigmanSquad.shrink_to_fit();
			m_pPigmanCount->setText("Pigman Count: " + std::to_string(m_pPigmanSquad.size()));
			break;
		}
	}

	// Destroyable Obstacle Checks
	if (m_pDestroyable != nullptr)
	{
		for (auto nodes : m_pGrid)
		{
			if (CollisionManager::AABBCheck(m_pDestroyable, nodes))
			{
				nodes->setEnabled(false);
			}
		}

		// Melee Destroyable
		m_CheckForLOS(m_pSteve, m_pDestroyable);
		if (m_pSteve->getState() == STEVE_ATTACK && m_pSteve->hasLOS())
		{
			m_pDestroyable->takeDamage();
		}

		// Shooting Destroyable obstacle
		for (int i = 0; i < m_pArrowQuiver.size(); i++)
		{
			if (CollisionManager::AABBCheck(m_pArrowQuiver[i], m_pDestroyable))
			{
				removeChild(m_pArrowQuiver[i]);
				m_pArrowQuiver[i] = nullptr;
				m_pArrowQuiver.erase(m_pArrowQuiver.begin() + i);
				m_pArrowQuiver.shrink_to_fit();
				m_pDestroyable->takeDamage();
				break;
			}
		}

		// Delete Destroyable Obstacle
		if (m_pDestroyable->getHealth() <= 0)
		{
			removeChild(m_pDestroyable);
			m_pDestroyable = nullptr;
			delete m_pDestroyable;
			m_pLOSDisplayObjects.pop_back();
			m_pLOSDisplayObjects.shrink_to_fit();
		}
	}

	// enable hidden nodes behind Destructible obstacle
	if (m_pDestroyable == nullptr)
	{
		for (auto nodes : m_pGrid)
		{
			if (!nodes->isEnabled())
				nodes->setEnabled(true);
		}
	}

	// Delete Arrows
	for (int i = 0; i < m_pArrowQuiver.size(); i++)
	{
		for (int j = 0; j < m_pObstacles.size(); j++)
			if (CollisionManager::AABBCheck(m_pArrowQuiver[i], m_pObstacles[j]))
			{
				removeChild(m_pArrowQuiver[i]);
				m_pArrowQuiver[i] = nullptr;
				m_pArrowQuiver.erase(m_pArrowQuiver.begin() + i);
				m_pArrowQuiver.shrink_to_fit();
				break;
			}
	}

	// Arrow deletion with Zombois
	for (int i = 0; i < m_pArrowQuiver.size(); i++)
	{
		for (int j = 0; j < m_pZombieArmy.size(); j++)
		{
			//Did arrow hit a zombie?
			if (CollisionManager::AABBCheck(m_pArrowQuiver[i], m_pZombieArmy[j]))
			{
				removeChild(m_pArrowQuiver[i]);
				m_pArrowQuiver[i] = nullptr;
				m_pArrowQuiver.erase(m_pArrowQuiver.begin() + i);
				m_pArrowQuiver.shrink_to_fit();

				m_pZombieArmy[j]->takeDamage();

				if (m_pZombieArmy[j]->getHealth() <= 0) {
					SoundManager::Instance().playSound("zombieDeath");
					static_cast<Zombie*>(m_pZombieArmy[j])->StartDespawnTimer();
					static_cast<Zombie*>(m_pZombieArmy[j])->setState(ZOMBIE_DEATH);
				}
				else {
					SoundManager::Instance().playSound("zombieHurt" + std::to_string(rand() % 2));
					static_cast<Zombie*>(m_pZombieArmy[j])->resetCooldown();
					static_cast<Zombie*>(m_pZombieArmy[j])->setState(ZOMBIE_DAMAGED);
				}
				break;
			}
		}
	}

	// Arrow deletion with piggies
	for (int i = 0; i < m_pArrowQuiver.size(); i++)
	{
		for (int j = 0; j < m_pPigmanSquad.size(); j++)
		{
			//Did arrow hit a zombie?
			if (CollisionManager::AABBCheck(m_pArrowQuiver[i], m_pPigmanSquad[j]))
			{
				removeChild(m_pArrowQuiver[i]);
				m_pArrowQuiver[i] = nullptr;
				m_pArrowQuiver.erase(m_pArrowQuiver.begin() + i);
				m_pArrowQuiver.shrink_to_fit();

				m_pPigmanSquad[j]->takeDamage();

				m_pPigmanSquad[j]->setIsHideCooldownRunning(true);				// TODO: to be changed later

				if (m_pPigmanSquad[j]->getHealth() <= 0) {
					SoundManager::Instance().playSound("pigmanDeath");
					static_cast<Pigman*>(m_pPigmanSquad[j])->StartDespawnTimer();
					static_cast<Pigman*>(m_pPigmanSquad[j])->setState(PIGMAN_DEATH);
				}
				else {
					SoundManager::Instance().playSound("pigmanHurt" + std::to_string(rand() % 2));
					static_cast<Pigman*>(m_pPigmanSquad[j])->resetCooldown();
					static_cast<Pigman*>(m_pPigmanSquad[j])->startHideCooldown();
					static_cast<Pigman*>(m_pPigmanSquad[j])->setState(PIGMAN_DAMAGED);
				}
				break;
			}
		}
	}

	// Range checks for all enemies
	for (auto enemy : m_pGangOfEnemies)
	{
		if (Util::distance(m_pSteve->getTransform()->position, enemy->getTransform()->position) < enemy->getLOSDistance())
		{
			if (enemy->hasLOS())
				enemy->setStevePosition(m_pSteve->getTransform()->position);
			else
				m_findClosestPathNodeWithLOS(enemy);
		}

		if (Util::distance(m_pSteve->getTransform()->position, enemy->getTransform()->position) < enemy->getAttackRange() && enemy->hasLOS()) {
			enemy->setIsWithinAttackRange(true);
		}

		else {
			enemy->setIsWithinAttackRange(false);
		}
	}

	// For triggering of Create Distance node in Pigman tree
	for (auto pigman : m_pPigmanSquad) {
		if (pigman->getIsHideCooldownRunning())
			m_findClosestPathNodeWithoutLOS(pigman);

		// Distance Check
		if (Util::distance(pigman->getTransform()->position, m_pSteve->getTransform()->position) < 100)
			pigman->setisSteveTooClose(true);
		else
			pigman->setisSteveTooClose(false);
	}

	// enemy off screen deletion
	for (int i = 0; i < m_pZombieArmy.size(); i++)
	{
		if (m_pZombieArmy[i]->getTransform()->position.y <= -100)
		{
			removeChild(m_pZombieArmy[i]);
			m_pZombieArmy[i] = nullptr;
			m_pZombieArmy.erase(m_pZombieArmy.begin() + i);
			m_pZombieArmy.shrink_to_fit();
			m_pZombieCount->setText("Zombie Count: " + std::to_string(m_pZombieArmy.size()));
			m_enemyNeedsSpawn = true;
			continue;
		}
		if (m_pZombieArmy[i]->getTransform()->position.y >= 700)
		{
			removeChild(m_pZombieArmy[i]);
			m_pZombieArmy[i] = nullptr;
			m_pZombieArmy.erase(m_pZombieArmy.begin() + i);
			m_pZombieArmy.shrink_to_fit();
			m_pZombieCount->setText("Zombie Count: " + std::to_string(m_pZombieArmy.size()));
			m_enemyNeedsSpawn = true;
			continue;
		}
	}
	// Delete Piggies if off screen
	for (int i = 0; i < m_pPigmanSquad.size(); i++)
	{
		if (m_pPigmanSquad[i]->getTransform()->position.y <= -100)
		{
			removeChild(m_pPigmanSquad[i]);
			m_pPigmanSquad[i] = nullptr;
			m_pPigmanSquad.erase(m_pPigmanSquad.begin() + i);
			m_pPigmanSquad.shrink_to_fit();
			m_pPigmanCount->setText("Pigman Count: " + std::to_string(m_pPigmanSquad.size()));
			m_enemyNeedsSpawn = true;
			continue;
		}
		if (m_pPigmanSquad[i]->getTransform()->position.y >= 700)
		{
			removeChild(m_pPigmanSquad[i]);
			m_pPigmanSquad[i] = nullptr;
			m_pPigmanSquad.erase(m_pPigmanSquad.begin() + i);
			m_pPigmanSquad.shrink_to_fit();
			m_pPigmanCount->setText("Pigman Count: " + std::to_string(m_pPigmanSquad.size()));
			m_enemyNeedsSpawn = true;
			continue;
		}
	}

	// Dynamic spawning of enemies // Can add timer for a wait period
	if (m_enemyNeedsSpawn)
	{
		if (rand() % 2 == 0)
		{
			Zombie* zomb = new Zombie();
			zomb->getTransform()->position = glm::vec2(707.0f, -90.0f);
			zomb->setDestinationNode(m_pMapNodes[6]);
			zomb->AddNode(m_pMapNodes[6]);
			zomb->AddNode(m_pMapNodes[3]);
			zomb->AddNode(m_pMapNodes[0]);
			addChild(zomb);
			m_pZombieArmy.push_back(zomb);
			m_pZombieCount->setText("Zombie Count: " + std::to_string(m_pZombieArmy.size()));
			m_pGangOfEnemies.push_back(zomb);
			m_enemyNeedsSpawn = false;
		}
		else
		{
			Pigman* pig = new Pigman();
			pig->getTransform()->position = glm::vec2(382.0f, 640.0f);
			addChild(pig);
			pig->setDestinationNode(m_pMapNodes[5]);
			pig->AddNode(m_pMapNodes[5]);
			pig->AddNode(m_pMapNodes[2]);
			pig->AddNode(m_pMapNodes[8]);
			m_pPigmanSquad.push_back(pig);
			m_pPigmanCount->setText("Pigman Count: " + std::to_string(m_pPigmanSquad.size()));
			m_pGangOfEnemies.push_back(pig);
			m_enemyNeedsSpawn = false;
		}
	}

	// Zombie damges Steve
	for (auto zombie : m_pZombieArmy)
	{
		if (zombie->getState() == ZOMBIE_ATTACK && Util::distance(m_pSteve->getTransform()->position, zombie->getTransform()->position) < 100)
			m_pSteve->m_takeDamage();
	}

	if (m_pSteve->getHealth() == 0)
	{
		// insert label display and restart button and refreshing of scene
	}
}

void PlayScene::updateCollisions()
{
	for (auto obstacle : m_pObstacles) {
		CollisionManager::AABBCheck(m_pSteve, obstacle);

		for (auto enemy : m_pGangOfEnemies)
			CollisionManager::AABBCheck(enemy, obstacle);
	}
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	if (m_pZombieArmy.size() <= 0 && m_pPigmanSquad.size() <= 0 && m_enemyNeedsSpawn == false)
	{
		//std::cout << "Not a Win" << std::endl;
		std::cout << "Win" << std::endl;
		m_pWinMenu->setWin(true);
	}

	if (m_pWinMenu->getRestart())
	{
		TheGame::Instance()->changeSceneState(START_SCENE);
	}

	EventManager::Instance().update();

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(START_SCENE);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		TheGame::Instance()->changeSceneState(END_SCENE);
	}

	//Debug: swap between Zombie Idle and Zombie Walk animations
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_P))
	{
		if (!m_zombieWalkToggle) {
			m_zombieWalkToggle = true;

			for (auto zombie : m_pZombieArmy)
				static_cast<Zombie*>(zombie)->setState(static_cast<Zombie*>(zombie)->getState() == ZOMBIE_IDLE ? ZOMBIE_WALK : ZOMBIE_IDLE);
		}

		if (!m_pigmanWalkToggle) {
			m_pigmanWalkToggle = true;

			for (auto pigman : m_pPigmanSquad)
				static_cast<Pigman*>(pigman)->setState(static_cast<Pigman*>(pigman)->getState() == PIGMAN_IDLE ? PIGMAN_WALK : PIGMAN_IDLE);
		}
	}
	else if (EventManager::Instance().isKeyUp(SDL_SCANCODE_P)) {
		if (m_zombieWalkToggle)
			m_zombieWalkToggle = false;

		if (m_pigmanWalkToggle)
			m_pigmanWalkToggle = false;
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_K))
	{
		// Debug key
		// All enemies take damage

		for (int i = 0; i < m_pZombieArmy.size(); i++)
		{
			if (static_cast<Zombie*>(m_pZombieArmy[i])->getState() != ZOMBIE_DAMAGED)
			{

				m_pZombieArmy[i]->takeDamage();

				if (m_pZombieArmy[i]->getHealth() <= 0) {
					SoundManager::Instance().playSound("zombieDeath");
					static_cast<Zombie*>(m_pZombieArmy[i])->StartDespawnTimer();
					static_cast<Zombie*>(m_pZombieArmy[i])->setState(ZOMBIE_DEATH);
				}
				else {
					SoundManager::Instance().playSound("zombieHurt" + std::to_string(rand() % 2));
					static_cast<Zombie*>(m_pZombieArmy[i])->resetCooldown();
					static_cast<Zombie*>(m_pZombieArmy[i])->setState(ZOMBIE_DAMAGED);
				}

				if (static_cast<Zombie*>(m_pZombieArmy[i])->getDespawnTimer() == 0 && static_cast<Zombie*>(m_pZombieArmy[i])->getState() == ZOMBIE_DEATH) {
					removeChild(m_pZombieArmy[i]);
					m_pZombieArmy[i] = nullptr;
					m_pZombieArmy.erase(m_pZombieArmy.begin() + i);
					m_pZombieArmy.shrink_to_fit();
					break;
				}
			}
		}

		for (int i = 0; i < m_pPigmanSquad.size(); i++)
		{
			if (static_cast<Pigman*>(m_pPigmanSquad[i])->getState() != PIGMAN_DAMAGED)
			{
				m_pPigmanSquad[i]->takeDamage();
				m_pPigmanSquad[i]->startHideCooldown();

				if (m_pPigmanSquad[i]->getHealth() <= 0) {
					SoundManager::Instance().playSound("pigmanDeath");
					static_cast<Pigman*>(m_pPigmanSquad[i])->StartDespawnTimer();
					static_cast<Pigman*>(m_pPigmanSquad[i])->setState(PIGMAN_DEATH);
				}
				else {
					SoundManager::Instance().playSound("pigmanHurt" + std::to_string(rand() % 2));
					static_cast<Pigman*>(m_pPigmanSquad[i])->resetCooldown();
					static_cast<Pigman*>(m_pPigmanSquad[i])->setState(PIGMAN_DAMAGED);
				}

				if (static_cast<Pigman*>(m_pPigmanSquad[i])->getDespawnTimer() == 0 && static_cast<Pigman*>(m_pPigmanSquad[i])->getState() == PIGMAN_DEATH) {
					removeChild(m_pPigmanSquad[i]);
					m_pPigmanSquad[i] = nullptr;
					m_pPigmanSquad.erase(m_pPigmanSquad.begin() + i);
					m_pPigmanSquad.shrink_to_fit();
					break;
				}
			}
		}
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_H))
	{
		// Debug view enabled here...

		// Will display the Line Of Sight (LOS) Rays, Detection Radius, Collider 
		// Boundaries, and Node points.
		if (!m_debugToggle) {
			m_debugToggle = true;
			for (auto path_node : m_pGrid)
			{
				path_node->getDebugMode() ? path_node->setDebugMode(false) : path_node->setDebugMode(true);
			}

			for (int i = 0; i < m_pZombieArmy.size(); i++)
				m_pZombieArmy[i]->getDebugMode() ? m_pZombieArmy[i]->setDebugMode(false) : m_pZombieArmy[i]->setDebugMode(true);

			for (int i = 0; i < m_pPigmanSquad.size(); i++)
				m_pPigmanSquad[i]->getDebugMode() ? m_pPigmanSquad[i]->setDebugMode(false) : m_pPigmanSquad[i]->setDebugMode(true);

			m_pSteve->getDebugMode() ? m_pSteve->setDebugMode(false) : m_pSteve->setDebugMode(true);

			for (int i = 0; i < m_pMapNodes.size(); i++)
				m_pMapNodes[i]->getDebugMode() ? m_pMapNodes[i]->setDebugMode(false) : m_pMapNodes[i]->setDebugMode(true);

			for (int i = 0; i < m_pObstacles.size(); i++)
				m_pObstacles[i]->getDebugMode() ? m_pObstacles[i]->setDebugMode(false) : m_pObstacles[i]->setDebugMode(true);
		}
	}
	else if (EventManager::Instance().isKeyUp(SDL_SCANCODE_H))
	{
		if (m_debugToggle)
			m_debugToggle = false;
	}

	//Up and Down Movement
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_W)) {
		m_pSteve->setState(STEVE_WALK);
		m_pSteve->setMaxSpeed(4);
		m_pSteve->moveForward();
	}
	else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_S)) {
		m_pSteve->setState(STEVE_WALK);
		m_pSteve->setMaxSpeed(4);
		m_pSteve->moveBack();
	}

	//Left and Right Movement
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A)) {
		m_pSteve->setState(STEVE_WALK);
		m_pSteve->setMaxSpeed(4);
		m_pSteve->turnLeft();
	}
	else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D)) {
		m_pSteve->setState(STEVE_WALK);
		m_pSteve->setMaxSpeed(4);
		m_pSteve->turnRight();
	}
	else if (m_pSteve->getMaxSpeed() == 0)
		m_pSteve->setState(STEVE_IDLE);

	//Left mouse Click, when Steve Attacks with Sword
	if (EventManager::Instance().getMouseButton(0)) {
		m_pSteve->setState(STEVE_ATTACK);
		if (m_pSteve->getSwordSoundCooldown() == 0) {
			m_pSteve->setSwordSoundCooldown(50);
			SoundManager::Instance().playSound("attack");
		}
	}
	//Right Mouse Click: when steve shoots
	else if (EventManager::Instance().getMouseButton(2)) {
		m_pSteve->setState(STEVE_SHOOT);

		if (m_pSteve->getArrowCooldown() <= 0) {
			m_pSteve->setArrowCooldown(52);

			// Fire Arrow
			Arrow* newArrow = new Arrow(m_pSteve->getTransform()->position, m_pSteve->getCurrentHeading());
			m_pArrowQuiver.push_back(newArrow);
			addChild(newArrow);
			newArrow = nullptr;
			SoundManager::Instance().playSound("shoot");
		}
	}
}

void PlayScene::start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";

	m_pBackground = new Background();
	addChild(m_pBackground);

	//load sounds used
	loadSounds();
	SoundManager::Instance().playMusic("calm1");

	// add the ship to the scene as a start point
	m_pSteve= new Steve();
	m_pSteve->getTransform()->position = glm::vec2(90.0f, 300.0f);
	addChild(m_pSteve, 2);
	m_pLOSDisplayObjects.push_back(m_pSteve);

	//Obstacles
	//m_pObstacles.push_back(new Tree(glm::vec2(-50.0f, -50.0f)));
	//m_pObstacles.push_back(new Tree(glm::vec2(150.0f, -50.0f)));
	//m_pObstacles.push_back(new Tree(glm::vec2(350.0f, -50.0f)));
	//m_pObstacles.push_back(new Tree(glm::vec2(550.0f, -50.0f)));
	//m_pObstacles.push_back(new Tree(glm::vec2(750.0f, -50.0f)));
	m_pObstacles.push_back(new Tree(glm::vec2(-50.0f, 150.0f)));
	m_pObstacles.push_back(new Tree(glm::vec2(850.0f, 150.0f)));
	m_pObstacles.push_back(new Tree(glm::vec2(-50.0f, 350.0f)));
	m_pObstacles.push_back(new Tree(glm::vec2(850.0f, 350.0f)));
	m_pObstacles.push_back(new Tree(glm::vec2(-50.0f, 550.0f)));
	m_pObstacles.push_back(new Tree(glm::vec2(850.0f, 550.0f)));
	//m_pObstacles.push_back(new Tree(glm::vec2(150.0f, 650.0f)));
	//m_pObstacles.push_back(new Tree(glm::vec2(350.0f, 650.0f)));
	//m_pObstacles.push_back(new Tree(glm::vec2(550.0f, 650.0f)));
	//m_pObstacles.push_back(new Tree(glm::vec2(750.0f, 650.0f)));

	//Obstacles in the middle of the map
	//m_pObstacles.push_back(new Tree(glm::vec2(225.0f, 225.0f)));
	m_pObstacles.push_back(new Tree(glm::vec2(225.0f, 375.0f)));
	m_pObstacles.push_back(new Tree(glm::vec2(545.0f, 300.0f)));
	m_pObstacles.push_back(new Tree(glm::vec2(585.0f, 335.0f)));
	
	for (auto obstacle : m_pObstacles) {
		addChild(obstacle);
		m_pLOSDisplayObjects.push_back(obstacle);
	}

	// Destroyable Obstacle
	m_pDestroyable = new DestroyableObstacle();
	m_pDestroyable->getTransform()->position = glm::vec2(225.0f, 225.0f);
	addChild(m_pDestroyable);
	m_pLOSDisplayObjects.push_back(m_pDestroyable);

	// Build Grid
	m_buildGrid();

	//nodes for navigation
	m_pMapNodes.push_back(new MapNodes(glm::vec2(55.0f,50.0f)));
	m_pMapNodes.push_back(new MapNodes(glm::vec2(55.0f,250.0f)));
	m_pMapNodes.push_back(new MapNodes(glm::vec2(55.0f,485.0f)));
	m_pMapNodes.push_back(new MapNodes(glm::vec2(350.0f,50.0f)));
	m_pMapNodes.push_back(new MapNodes(glm::vec2(350.0f,250.0f)));
	m_pMapNodes.push_back(new MapNodes(glm::vec2(350.0f,485.0f)));
	m_pMapNodes.push_back(new MapNodes(glm::vec2(675.0f,50.0f)));
	m_pMapNodes.push_back(new MapNodes(glm::vec2(675.0f,250.0f)));
	m_pMapNodes.push_back(new MapNodes(glm::vec2(675.0f,485.0f)));

	for (auto nodes : m_pMapNodes)
		addChild(nodes);

	// add Zombies and set their paths
	Zombie* zomb = new Zombie();
	zomb->getTransform()->position = m_pMapNodes[6]->getNodeMiddle();
	zomb->AddNode(m_pMapNodes[6]);
	zomb->AddNode(m_pMapNodes[3]);
	zomb->setDestinationNode(m_pMapNodes[3]);
	zomb->AddNode(m_pMapNodes[0]);
	addChild(zomb);
	m_pZombieArmy.push_back(zomb);
	m_pGangOfEnemies.push_back(zomb);

	zomb = new Zombie();
	zomb->getTransform()->position = m_pMapNodes[8]->getNodeMiddle();
	addChild(zomb);
	zomb->AddNode(m_pMapNodes[8]);
	zomb->AddNode(m_pMapNodes[5]);
	zomb->setDestinationNode(m_pMapNodes[5]);
	zomb->AddNode(m_pMapNodes[2]);
	m_pZombieArmy.push_back(zomb);
	m_pGangOfEnemies.push_back(zomb);

	// Pigman Path
	Pigman* pig = new Pigman();
	pig->getTransform()->position = m_pMapNodes[3]->getNodeMiddle();
	pig->AddNode(m_pMapNodes[3]);
	pig->AddNode(m_pMapNodes[0]);
	pig->setDestinationNode(m_pMapNodes[0]);
	pig->AddNode(m_pMapNodes[6]);
	addChild(pig);
	m_pPigmanSquad.push_back(pig);
	m_pGangOfEnemies.push_back(pig);

	pig = new Pigman();
	pig->getTransform()->position = m_pMapNodes[5]->getNodeMiddle();
	addChild(pig);
	pig->AddNode(m_pMapNodes[5]);
	pig->AddNode(m_pMapNodes[2]);
	pig->setDestinationNode(m_pMapNodes[2]);
	pig->AddNode(m_pMapNodes[8]);
	m_pPigmanSquad.push_back(pig);
	m_pGangOfEnemies.push_back(pig);

	//Labels
	m_pHealth = new Label("Current HP: ", "Minecraft", 30);
	m_pHealth->getTransform()->position = glm::vec2(95.0f, 25.0f);
	addChild(m_pHealth);

	m_pZombieCount = new Label("", "Minecraft", 27);
	m_pZombieCount->setText("Zombie Count: " + std::to_string(m_pZombieArmy.size()));
	m_pZombieCount->getTransform()->position = glm::vec2(675.0f, 25.0f);
	addChild(m_pZombieCount);

	m_pPigmanCount = new Label("", "Minecraft", 27);
	m_pPigmanCount->setText("Pigman Count: " + std::to_string(m_pPigmanSquad.size()));
	m_pPigmanCount->getTransform()->position = glm::vec2(420.0f, 25.0f);
	addChild(m_pPigmanCount);

	//Win Menu
	m_pWinMenu = new WinMenu();
	addChild(m_pWinMenu);
}

void PlayScene::GUI_Function() 
{
	//TODO: We need to deal with this
	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);
	
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("GAME3001 - Assignment 4", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	// allow ship rotation
	
	ImGui::Separator();
	
	ImGui::Separator();
	
	if (ImGui::Button("Start"))
	{

	}

	ImGui::SameLine();
	
	if (ImGui::Button("Reset"))
	{
		// reset everything back to initial values
		
	}

	ImGui::Separator();

	ImGui::End();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}

void PlayScene::m_CheckForLOS(Agent* first_object, DisplayObject* target_object)
{
	first_object->setHasLOS(false);
	if (first_object->getType() != PATH_NODE)
		first_object->setIsWithinRadius(false);
	// if ship to target distance is less than or equal to LOS Distance
	auto ObjectToTargetDistance = Util::distance(first_object->getTransform()->position, target_object->getTransform()->position);

	if (ObjectToTargetDistance <= first_object->getLOSDistance())
	{
		if (first_object->getType() != PATH_NODE) {
			first_object->setIsWithinRadius(true);
			first_object->setRadiusDistance(ObjectToTargetDistance);
		}
		std::vector<DisplayObject*> contactList;
		for (auto object : getDisplayList())
		{
			if ((object->getType() == PLAYER || object->getType() == OBSTACLE) && first_object != object) {

				// check if obstacle is farther than than the object
				auto AgentToObstacleDistance = Util::distance(first_object->getTransform()->position, object->getTransform()->position);

				if (AgentToObstacleDistance <= ObjectToTargetDistance)
				{
					contactList.push_back(object);
				}
			}
		}

		contactList.push_back(target_object); // add the target to the end of the list
		bool hasLOS;

		if (first_object->getType() == PLAYER)
			hasLOS = CollisionManager::LOSCheck(first_object, first_object->getTransform()->position + first_object->getCurrentDirection() * first_object->getLOSDistance(), contactList, target_object);
		else
			hasLOS = CollisionManager::LOSCheck(first_object, target_object->getTransform()->position, contactList, target_object);

		first_object->setHasLOS(hasLOS);
	}
}

void PlayScene::loadSounds() {
	SoundManager::Instance().load("../Assets/audio/Calm1.mp3", "calm1", SOUND_MUSIC);
	SoundManager::Instance().load("../Assets/audio/Grass_hit1.ogg", "grass0", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/Grass_hit2.ogg", "grass1", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/Grass_hit3.ogg", "grass2", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/Grass_hit4.ogg", "grass3", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/Grass_hit5.ogg", "grass4", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/Grass_hit6.ogg", "grass5", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/Arrow_fire.wav", "shoot", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/Sweep_attack1.ogg", "attack", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/Zombie_idle1.ogg", "zombieIdle0", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/Zombie_idle2.ogg", "zombieIdle1", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/Zombie_idle3.ogg", "zombieIdle2", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/Zombie_hurt1.ogg", "zombieHurt0", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/Zombie_hurt2.ogg", "zombieHurt1", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/Zombie_death.ogg", "zombieDeath", SOUND_SFX);
}

void PlayScene::m_buildGrid()
{
	auto tileSize = Config::TILE_SIZE;

	// add path_nodes to the Grid
	for (int row = 0; row < Config::ROW_NUM; ++row)
	{
		for (int col = 0; col < Config::COL_NUM; ++col)
		{
			PathNode* path_node = new PathNode();
			path_node->getTransform()->position = glm::vec2(
				(col * tileSize) + tileSize * 0.5f, (row * tileSize) + tileSize * 0.5f);

			path_node->setNodeMiddle(path_node->getTransform()->position);

			bool isColliding = false;

			for (auto obstacle : m_pObstacles)
			{
				if (CollisionManager::AABBCheck(path_node, obstacle))
				{
					delete path_node;
					path_node = nullptr;
					isColliding = true;
					break;
				}
			}
			if (!isColliding)
			{
				addChild(path_node);
				m_pGrid.push_back(path_node);
			}
		}
	}
}

bool PlayScene::m_CheckForEnemyLOS(PathNode* node, Enemy* enemy)
{
	bool hasLOS = false;

	auto NodeToEnemyDistance = Util::distance(node->getTransform()->position, enemy->getTransform()->position);

	if (NodeToEnemyDistance <= enemy->getLOSDistance()) {
		std::vector<DisplayObject*> contactList;

		for (auto display_object : m_pObstacles) {

			if (display_object->getType() == OBSTACLE) {
				auto NodeToObstacleDistance = Util::distance(node->getTransform()->position, display_object->getTransform()->position);
				const auto nodeTarget = node->getTransform()->position + node->getCurrentDirection() * node->getLOSDistance();

				if (NodeToObstacleDistance <= NodeToEnemyDistance)
				{
					hasLOS = CollisionManager::LOSCheck(node, nodeTarget, contactList, enemy);
					contactList.push_back(display_object);
					node->setHasEnemyLOS(hasLOS);
				}
				else
				{
					contactList.push_back(display_object);
					node->setHasEnemyLOS(true);
				}
			}
		}
		contactList.push_back(enemy); // add the target to the end of the list
		const auto nodeTarget = node->getTransform()->position + node->getCurrentDirection() * node->getLOSDistance();

		hasLOS = CollisionManager::LOSCheck(node, nodeTarget, contactList, enemy);
		//node->setHasEnemyLOS(hasLOS);
	}
	else
	{
		std::vector<DisplayObject*> contactList;
		const auto nodeTarget = node->getTransform()->position + node->getCurrentDirection() * node->getLOSDistance();

		hasLOS = CollisionManager::LOSCheck(node, nodeTarget, contactList, enemy);
		node->setHasEnemyLOS(hasLOS);
	}
	return hasLOS;
}

void PlayScene::m_CheckPathNodeLOS()
{
	for (auto path_node : m_pGrid)
	{
		auto targetDirection = m_pSteve->getTransform()->position - path_node->getTransform()->position;
		auto normalizedDirection = Util::normalize(targetDirection);
		path_node->setCurrentDirection(normalizedDirection);
		m_CheckForLOS(path_node, m_pSteve);

		path_node->setHasEnemyLOS(false);
		for (auto zombie : m_pZombieArmy) {
			if (m_CheckForEnemyLOS(path_node, zombie))
				break;
		}
		
		if (path_node->hasEnemyLOS())
			continue;

		for (auto pigman : m_pPigmanSquad) {
			if (m_CheckForEnemyLOS(path_node, pigman))
				break;
		}
	}
}

void PlayScene::m_findClosestPathNodeWithLOS(Agent* agent)
{
	auto min = agent->getLOSDistance();
	PathNode* closestPathNode = nullptr;

	for (auto path_node : m_pGrid)
	{
		if (path_node->hasLOS() && path_node->hasEnemyLOS())
		{
			const auto distance = Util::distance(agent->getTransform()->position, path_node->getTransform()->position);
			if (distance < min)
			{
				min = distance;
				closestPathNode = path_node;
			}
		}
	}
	if (closestPathNode != nullptr)
		agent->setDestinationNode(closestPathNode->getNodeMiddle());
}

void PlayScene::m_findClosestPathNodeWithoutLOS(Agent* agent)
{
	auto min = INFINITY;
	PathNode* closestPathNode = nullptr;

	for (auto path_node : m_pGrid)
	{
		if (!path_node->hasLOS() && !path_node->hasEnemyLOS())
		{
			const auto distance = Util::distance(agent->getTransform()->position, path_node->getTransform()->position);
			if (distance < min)
			{
				min = distance;
				closestPathNode = path_node;
			}
		}
	}
	if (closestPathNode != nullptr)
		agent->setDestinationNode(closestPathNode->getNodeMiddle());
}
