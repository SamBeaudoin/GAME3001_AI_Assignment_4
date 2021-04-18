#include "WinMenu.h"
#include "Game.h"
#include "SceneState.h"

WinMenu::WinMenu()
{
	TextureManager::Instance()->load("../Assets/textures/pausemenu.png", "winmenu");
	auto size = TextureManager::Instance()->getTextureSize("winmenu");
	setWidth(size.x);
	setHeight(size.y);
	getTransform()->position = glm::vec2(250.0f, 100.0f);
	setType(WIN_MENU);
	getRigidBody()->isColliding = false;

	m_pResumeButton = new Button("../Assets/textures/Restart_Button.png", "ResumeButton", RESUME_BUTTON);
	m_pResumeButton->getTransform()->position = glm::vec2(400.0f, 220.0f);

	m_pResumeButton->addEventListener(CLICK, [&]()-> void
		{
			setWin(false);
		});

	m_pResumeButton->addEventListener(MOUSE_OVER, [&]()->void
		{
			m_pResumeButton->setAlpha(128);
		});

	m_pResumeButton->addEventListener(MOUSE_OUT, [&]()->void
		{
			m_pResumeButton->setAlpha(255);
		});


	m_pExitButton = new Button("../Assets/textures/PauseExitButton.png", "ExitButton", EXIT_BUTTON);
	m_pExitButton->getTransform()->position = glm::vec2(400.0f, 394.0f);

	m_pExitButton->addEventListener(CLICK, [&]()-> void
		{
			setWin(false);
			TheGame::Instance()->changeSceneState(START_SCENE);
		});

	m_pExitButton->addEventListener(MOUSE_OVER, [&]()->void
		{
			m_pExitButton->setAlpha(128);
		});

	m_pExitButton->addEventListener(MOUSE_OUT, [&]()->void
		{
			m_pExitButton->setAlpha(255);
		});

}

WinMenu::~WinMenu()
{
}


void WinMenu::draw()
{
	if (m_paused)
	{
		TextureManager::Instance()->draw("winmenu", getTransform()->position.x, getTransform()->position.y);
		m_pResumeButton->draw();
		m_pExitButton->draw();
	}
}
void WinMenu::update()
{
	if (m_paused)
	{
		m_pResumeButton->update();
		m_pExitButton->update();
	}
}

void WinMenu::clean()
{
}

void WinMenu::setWin(const bool paused)
{
	m_paused = paused;
}

bool WinMenu::getWin() const
{
	return m_paused;
}
