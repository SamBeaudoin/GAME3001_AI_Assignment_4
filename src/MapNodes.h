#pragma once
#ifndef __MAP_NODES__
#define __MAP_NODES__
#include "DisplayObject.h"

class MapNodes : public DisplayObject
{
public:
	// constructors
	MapNodes();
	MapNodes(glm::vec2 pos);

	// destructor
	~MapNodes();

	// getters
	bool getDebugMode();
	glm::vec2 getNodeMiddle();

	// setters
	void setDebugMode(bool mode);

	// life cycle functions
	void draw() override;
	void update() override;
	void clean() override;
private:
	bool m_DebugMode;
	glm::vec2 m_middleOfNode;
};

#endif /* defined (__MAP_NODES__) */