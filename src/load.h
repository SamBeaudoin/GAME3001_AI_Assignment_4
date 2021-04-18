#pragma once
#ifndef __LOAD_SCENE__
#define __LOAD_SCENE__

#include "Scene.h"
#include "Label.h"
#include "ship.h"
#include "Button.h"

class LoadScene final : public Scene
{
public:
	LoadScene();
	~LoadScene();

	// Inherited via Scene
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;

private:

	Label* m_pRuebearInfo{};
};

#endif /* defined (__LOAD_SCENE__) */
