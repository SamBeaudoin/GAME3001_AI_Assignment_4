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

	m_CheckPathNodeLOS(); // MAJOUR DROP IN FPS!!

	updateCollisions();

	m_pSteve->faceMouse();

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

}

void PlayScene::updateCollisions()
{
	for (auto obstacle : m_pObstacles) {
		CollisionManager::AABBCheck(m_pSteve, obstacle);
	}
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
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
	}
	else if (EventManager::Instance().isKeyUp(SDL_SCANCODE_P)) {
		if (m_zombieWalkToggle)
			m_zombieWalkToggle = false;
	}
	
	if(EventManager::Instance().isKeyDown(SDL_SCANCODE_K))
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

	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_H))
	{
		// Debug view enabled here...

		// Will display the Line Of Sight (LOS) Rays, Detection Radius, Collider 
		// Boundaries, and Node points.
		if (!m_debugToggle) {
			m_debugToggle = true;

			for (int i = 0; i < m_pZombieArmy.size(); i++)
				m_pZombieArmy[i]->getDebugMode() ? m_pZombieArmy[i]->setDebugMode(false) : m_pZombieArmy[i]->setDebugMode(true);

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
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_G))
	{
		m_gridVisible = !m_gridVisible;
		m_toggleGrid(m_gridVisible);
	}
}

void PlayScene::start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";

	m_pBackground = new Background();
	addChild(m_pBackground);

	m_buildGrid();

	//load sounds used
	loadSounds();
	SoundManager::Instance().playMusic("calm1");

	// add the ship to the scene as a start point
	m_pSteve= new Steve();
	m_pSteve->getTransform()->position = glm::vec2(90.0f, 300.0f);
	addChild(m_pSteve, 2);

	//Obstacles
	m_pObstacles.push_back(new Tree(glm::vec2(-50.0f, -50.0f)));
	m_pObstacles.push_back(new Tree(glm::vec2(150.0f, -50.0f)));
	m_pObstacles.push_back(new Tree(glm::vec2(350.0f, -50.0f)));
	m_pObstacles.push_back(new Tree(glm::vec2(550.0f, -50.0f)));
	m_pObstacles.push_back(new Tree(glm::vec2(750.0f, -50.0f)));
	m_pObstacles.push_back(new Tree(glm::vec2(-50.0f, 150.0f)));
	m_pObstacles.push_back(new Tree(glm::vec2(850.0f, 150.0f)));
	m_pObstacles.push_back(new Tree(glm::vec2(-50.0f, 350.0f)));
	m_pObstacles.push_back(new Tree(glm::vec2(850.0f, 350.0f)));
	m_pObstacles.push_back(new Tree(glm::vec2(-50.0f, 550.0f)));
	m_pObstacles.push_back(new Tree(glm::vec2(850.0f, 550.0f)));
	m_pObstacles.push_back(new Tree(glm::vec2(150.0f, 650.0f)));
	m_pObstacles.push_back(new Tree(glm::vec2(350.0f, 650.0f)));
	m_pObstacles.push_back(new Tree(glm::vec2(550.0f, 650.0f)));
	m_pObstacles.push_back(new Tree(glm::vec2(750.0f, 650.0f)));

	//Obstacles in the middle of the map
	m_pObstacles.push_back(new Tree(glm::vec2(225.0f, 225.0f)));
	m_pObstacles.push_back(new Tree(glm::vec2(225.0f, 375.0f)));
	m_pObstacles.push_back(new Tree(glm::vec2(525.0f, 275.0f)));
	m_pObstacles.push_back(new Tree(glm::vec2(575.0f, 325.0f)));
	
	for (auto obstacle : m_pObstacles)
		addChild(obstacle);

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

	zomb = new Zombie();
	zomb->getTransform()->position = m_pMapNodes[8]->getNodeMiddle();
	addChild(zomb);
	zomb->AddNode(m_pMapNodes[8]);
	zomb->AddNode(m_pMapNodes[5]);
	zomb->setDestinationNode(m_pMapNodes[5]);
	zomb->AddNode(m_pMapNodes[2]);
	m_pZombieArmy.push_back(zomb);

	//Labels
	m_pHealth = new Label("Current HP: ", "Minecraft", 30);
	m_pHealth->getTransform()->position = glm::vec2(95.0f, 25.0f);
	addChild(m_pHealth);

	m_pZombieCount = new Label("", "Minecraft", 30);
	m_pZombieCount->setText("Zombie Count: " + std::to_string(m_pZombieArmy.size()));
	m_pZombieCount->getTransform()->position = glm::vec2(550.0f, 25.0f);
	addChild(m_pZombieCount);
}

void PlayScene::GUI_Function() 
{
	//TODO: We need to deal with this
	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);
	
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("GAME3001 - Assignment 3", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	// allow ship rotation
	
	ImGui::Separator();

	static bool gridVisible = true;
	if (ImGui::Checkbox("Toggle Grid", &gridVisible))
	{
		m_toggleGrid(gridVisible);
	}
	
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
	first_object->setIsWithinRadius(false);
	// if ship to target distance is less than or equal to LOS Distance
	auto ObjectToTargetDistance = Util::distance(first_object->getTransform()->position, target_object->getTransform()->position);

	if (ObjectToTargetDistance <= first_object->getLOSDistance())
	{
		first_object->setIsWithinRadius(true);
		first_object->setRadiusDistance(ObjectToTargetDistance);
		std::vector<DisplayObject*> contactList;
		for (auto object : getDisplayList())
		{
			if (object->getType() == MAP_NODE)
				continue;
			// check if object is farther than than the target
			auto ObjectToObjectDistance = Util::distance(first_object->getTransform()->position, object->getTransform()->position);

			if (ObjectToObjectDistance <= ObjectToTargetDistance)
			{
				if ((object->getType() != first_object->getType()) && (object->getType() != target_object->getType()))
				{
					contactList.push_back(object);
				}
			}
		}
		contactList.push_back(target_object); // add the target to the end of the list
		bool hasLOS;
		if (first_object->getType() == PLAYER)
			hasLOS = CollisionManager::LOSCheck(first_object->getTransform()->position, first_object->getTransform()->position + first_object->getCurrentDirection() * first_object->getLOSDistance(), contactList, target_object);
		else
			hasLOS = CollisionManager::LOSCheck(first_object->getTransform()->position, target_object->getTransform()->position, contactList, target_object);

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
			addChild(path_node);
			m_pGrid.push_back(path_node);
		}
	}
}

bool PlayScene::m_CheckAgentLOS(Agent* agent, DisplayObject* object)
{
	// initialize
	bool hasLOS = false;
	agent->setHasLOS(false);

	// if agent to object distance is less than or equal to LOS Distance
	auto AgentToObjectDistance = Util::distance(agent->getTransform()->position, object->getTransform()->position);
	if (AgentToObjectDistance <= agent->getLOSDistance())
	{
		std::vector<DisplayObject*> contactList;
		for (auto display_object : getDisplayList())
		{
			// check if obstacle is farther than than the object
			auto AgentToObstacleDistance = Util::distance(agent->getTransform()->position, display_object->getTransform()->position);

			if (AgentToObstacleDistance <= AgentToObjectDistance)
			{
				if (display_object->getType() == PLAYER || display_object->getType() == OBSTACLE)
				{
					contactList.push_back(display_object);
				}
			}
		}
		contactList.push_back(object); // add the target to the end of the list
		const auto agentTarget = agent->getTransform()->position + agent->getCurrentDirection() * agent->getLOSDistance();

		// New version...
		hasLOS = CollisionManager::LOSCheck(agent, agentTarget, contactList, object);

		agent->setHasLOS(hasLOS);
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
		m_CheckAgentLOS(path_node, m_pSteve);
	}
}

void PlayScene::m_toggleGrid(bool state)
{

	for (auto path_node : m_pGrid)
	{
		path_node->setVisible(state);
	}
}

PathNode* PlayScene::m_findClosestPathNode(Agent* agent)
{
	auto min = INFINITY;
	PathNode* closestPathNode = nullptr;

	// Alex's extra...
	std::vector<PathNode*> m_pNoLOSNodes;
	for (auto path_node : m_pGrid)
	{
		if (path_node->hasLOS() == false)
		{
			m_pNoLOSNodes.push_back(path_node);
		}
	}

	for (auto path_node : m_pNoLOSNodes) // Change to m_pGrid for Tom's
	{
		const auto distance = Util::distance(agent->getTransform()->position, path_node->getTransform()->position);
		if (distance < min)
		{
			min = distance;
			closestPathNode = path_node;
		}
	}
	return closestPathNode;
}
