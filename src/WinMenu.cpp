#include "WinMenu.h"
#include "Game.h"
#include "SceneState.h"

WinMenu::WinMenu()
{
	TextureManager::Instance()->load("../Assets/textures/You_Won.png", "winmenu");
	auto size = TextureManager::Instance()->getTextureSize("winmenu");
	setWidth(size.x);
	setHeight(size.y);
	getTransform()->position = glm::vec2(150.0f, 100.0f);
	setType(WIN_MENU);
	getRigidBody()->isColliding = false;

	m_pResumeButton = new Button("../Assets/textures/Restart_Button.png", "ResumeButton", RESUME_BUTTON);
	m_pResumeButton->getTransform()->position = glm::vec2(400.0f, 320.0f);


	m_pResumeButton->addEventListener(CLICK, [&]()-> void
		{
			m_Restart = true;
		});

	m_pResumeButton->addEventListener(MOUSE_OVER, [&]()->void
		{
			m_pResumeButton->setAlpha(128);
		});

	m_pResumeButton->addEventListener(MOUSE_OUT, [&]()->void
		{
			m_pResumeButton->setAlpha(255);
		});
}

WinMenu::~WinMenu()
{
}


void WinMenu::draw()
{
	if (m_win)
	{
		TextureManager::Instance()->draw("winmenu", getTransform()->position.x, getTransform()->position.y);
		m_pResumeButton->draw();
	}
}
void WinMenu::update()
{
	if (m_win)
	{
		m_pResumeButton->update();
	}
}

void WinMenu::clean()
{
}

void WinMenu::setWin(const bool win)
{
	m_win = win;
}

bool WinMenu::getWin() const
{
	return m_win;
}

void WinMenu::setRestart(bool restart)
{
	m_Restart = restart;
}

bool WinMenu::getRestart() const
{
	return m_Restart;
}
