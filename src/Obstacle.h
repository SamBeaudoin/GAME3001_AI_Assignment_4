#pragma once
#ifndef __OBSTACLE__
#define __OBSTACLE__
#include "DisplayObject.h"
#include "SDL.h"
#include "DetectRect.h"

class Obstacle : public DisplayObject
{
public:
	// constructors
	Obstacle();

	// destructor
	~Obstacle();

	// life cycle functions
	void draw() override;
	void update() override;
	void clean() override;

	bool getDebugMode() const;
	void setDebugMode(bool mode);

	DetectRect* GetDetection();
	void UpdateDetection();

protected:
	DetectRect* m_detectRect;

private:
	bool m_debugMode;
};

#endif /* defined (__OBSTACLE__) */