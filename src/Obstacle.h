#pragma once
#ifndef __OBSTACLE__
#define __OBSTACLE__
#include "Sprite.h"
#include "SDL.h"
#include "DetectRect.h"

class Obstacle : public Sprite
{
public:
	// constructors
	Obstacle() {};
	Obstacle(glm::vec2 pos, std::string sprite);

	// destructor
	~Obstacle();

	// life cycle functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	bool getDebugMode() const;
	void setDebugMode(bool mode);

	DetectRect* GetDetection();
	void UpdateDetection();

protected:
	DetectRect* m_detectRect;

private:
	virtual void m_buildAnimations() = 0;

	bool m_debugMode;
	std::string m_sprite;
};

#endif /* defined (__OBSTACLE__) */