#pragma once
#ifndef __GAME_OBJECT_TYPE__
#define __GAME_OBJECT_TYPE__
enum GameObjectType {
	NONE = -1,
	AGENT,
	PATH_NODE,
	SPACE_SHIP,
	OBSTACLE,
	SHIP,
	TARGET,
	PLAYER,
	PLANE,
	START_BUTTON,
	RESTART_BUTTON,
	BACK_BUTTON,
	NEXT_BUTTON,
	ZOMBIE,
	PIGMAN,
	MAP_NODE,
	PROJECTILE,
	NUM_OF_TYPES
};
#endif /* defined (__GAME_OBJECT_TYPE__) */