#pragma once
#ifndef __WINMENU__
#define __WINMENU__

#include "Button.h"
#include "DisplayObject.h"
#include "TextureManager.h"

class WinMenu final : public DisplayObject
{
public:
	WinMenu();
	~WinMenu();
	void draw() override;
	void update() override;
	void clean() override;

	void setWin(bool win);
	bool getWin() const;

	void setRestart(bool restart);
	bool getRestart() const;
private:
	bool m_win = false;
	bool m_Restart = false;

	Button* m_pResumeButton;
	Button* m_pExitButton;
};


#endif /* DEFINED (__WINMENU__) */
