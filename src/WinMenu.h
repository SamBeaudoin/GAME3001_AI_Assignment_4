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

	void setWin(bool paused);
	bool getWin() const;
private:
	bool m_paused = false;

	Button* m_pResumeButton;
	Button* m_pExitButton;
};


#endif /* DEFINED (__WINMENU__) */
