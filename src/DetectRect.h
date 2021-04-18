#pragma once
#ifndef __DETECT_RECT__
#define __DETECT_RECT__
#include "DisplayObject.h"
#include "CollidingSide.h"
#include "SDL.h"

class DetectRect : public DisplayObject {
public:
	DetectRect();
	DetectRect(glm::vec2 position, int width, int height);
	~DetectRect();

	virtual void draw();
	virtual void update();
	virtual void clean();

	SDL_Rect* getDetectRect();
	void setPos(glm::vec2 pos);

	CollidingSide getCollidingSide(DisplayObject* object);

private:
	SDL_Rect* m_detectRect;
};

#endif