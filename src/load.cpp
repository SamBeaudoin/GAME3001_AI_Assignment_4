#include "load.h"
#include <algorithm>
#include "Game.h"
#include "glm/gtx/string_cast.hpp"
#include "EventManager.h"

LoadScene::LoadScene()
{
	LoadScene::start();
}

LoadScene::~LoadScene()
= default;

void LoadScene::draw()
{
	drawDisplayList();
}

void LoadScene::update()
{
	updateDisplayList();
}

void LoadScene::clean()
{
	removeAllChildren();
}

void LoadScene::handleEvents()
{
	EventManager::Instance().update();
}

void LoadScene::start()
{
	const SDL_Color gold = { 218, 165, 32 };

	m_pRuebearInfo = new Label("LOADING...", "Minecraft", 40, gold, glm::vec2(400.0f, 200.0f));
	m_pRuebearInfo->setParent(this);
	addChild(m_pRuebearInfo);

	TheGame::Instance()->changeSceneState(PLAY_SCENE);
}

