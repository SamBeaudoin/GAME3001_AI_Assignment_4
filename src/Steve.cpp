#include "Steve.h"
#include "glm/gtx/string_cast.hpp"
#include "PlayScene.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "Renderer.h"
#include "Util.h"
#include "EventManager.h"

Steve::Steve() : m_maxSpeed(10.0f)
{
	TextureManager::Instance()->loadSpriteSheet("../Assets/sprites/Steve_Animations.txt" ,"../Assets/sprites/Sprite_Sheet.png", "steve");

	TextureManager::Instance()->load("../Assets/sprites/heart.png", "heart");	// Change this out for Minecraft heart if you want

	setSpriteSheet(TextureManager::Instance()->getSpriteSheet("steve"));

	setWidth(40);
	setHeight(40);

	m_buildAnimations();

	getTransform()->position = glm::vec2(400.0f, 300.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(PLAYER);
	setState(STEVE_IDLE);

	setCurrentHeading(0.0f); // current facing angle
	setCurrentDirection(glm::vec2(1.0f, 0.0f)); // facing right
	setLOSColour(glm::vec4(1, 0, 0, 1));
	setLOSDistance(80.0f);
	m_health = 3;
}

Steve::~Steve() = default;

void Steve::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw le Steve
	switch (m_state)
	{
	case STEVE_IDLE:
		TextureManager::Instance()->playAnimation("steve", getAnimation("idle"), x, y, 0.0625f, getCurrentHeading(), 255, true);
		break;
	case STEVE_WALK:
		TextureManager::Instance()->playAnimation("steve", getAnimation("walk"), x, y, 1.1f, getCurrentHeading(), 255, true);
		break;
	case STEVE_ATTACK:
		TextureManager::Instance()->playAnimation("steve", getAnimation("attack"), x, y, 1.5f, getCurrentHeading(), 255, true);
		break;
	case STEVE_SHOOT:
		TextureManager::Instance()->playAnimation("steve", getAnimation("shoot"), x, y, 0.15, getCurrentHeading(), 255, true);
		break;
	}

	if (getDebugMode())
	{
		SDL_Rect colliderBoundry = { getTransform()->position.x-22, getTransform()->position.y-5, getWidth(), getHeight() };
		SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 0, 0, 0, 255);
		SDL_RenderDrawRect(Renderer::Instance()->getRenderer(), &colliderBoundry);

		Util::DrawLine(getTransform()->position, getTransform()->position + getCurrentDirection() * getLOSDistance(), getLOSColour());
	}

	if (m_health >= 1)TextureManager::Instance()->draw("heart", 190, 15);
	if (m_health >= 2)TextureManager::Instance()->draw("heart", 215, 15);
	if (m_health >= 3)TextureManager::Instance()->draw("heart", 240, 15);

	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
}

void Steve::update()
{
	setMaxSpeed(0.0f);
	move();
	m_checkBounds();

	if (m_arrowCooldown > 0)
		m_arrowCooldown--;
	else if (m_swordSoundCooldown > 0)
		m_swordSoundCooldown--;
}

void Steve::clean()
{
}

void Steve::turnRight()
{
	getRigidBody()->velocity.x = 1  * m_maxSpeed;
	setDistanceWalked(getDistanceWalked() + m_maxSpeed);

	//Sound
	if (getDistanceWalked() >= 60) {
		setDistanceWalked(getDistanceWalked() - 60);
		SoundManager::Instance().playSound("grass" + std::to_string(rand() % 6));
	}
}

void Steve::turnLeft()
{
	getRigidBody()->velocity.x = -1 * m_maxSpeed;
	setDistanceWalked(getDistanceWalked() + m_maxSpeed);

	//Sound
	if (getDistanceWalked() >= 60) {
		setDistanceWalked(getDistanceWalked() - 60);
		SoundManager::Instance().playSound("grass" + std::to_string(rand() % 6));
	}
}

void Steve::moveForward()	// Up
{
	getRigidBody()->velocity.y = -1 * m_maxSpeed;
	setDistanceWalked(getDistanceWalked() + m_maxSpeed);

	//Sound
	if (getDistanceWalked() >= 60) {
		setDistanceWalked(getDistanceWalked() - 60);
		SoundManager::Instance().playSound("grass" + std::to_string(rand() % 6));
	}
}

void Steve::moveBack()	// down
{
	getRigidBody()->velocity.y = 1  * m_maxSpeed;
	setDistanceWalked(getDistanceWalked() + m_maxSpeed);

	//Sound
	if (getDistanceWalked() >= 60) {
		setDistanceWalked(getDistanceWalked() - 60);
		SoundManager::Instance().playSound("grass" + std::to_string(rand() % 6));
	}
}

void Steve::move()
{
	getTransform()->position += getRigidBody()->velocity;
	getRigidBody()->velocity *= 0.9f;
}

void Steve::faceMouse()
{
	float mX = EventManager::Instance().getMousePosition().x;
	float mY = EventManager::Instance().getMousePosition().y;
	float sX = getTransform()->position.x;
	float sY = getTransform()->position.y;
	float delta_x = mX - sX;
	float delta_y = mY - sY;
	setCurrentHeading((atan2(delta_y, delta_x) * 180.0000) / 3.1416);
}

float Steve::getMaxSpeed() const
{
	return m_maxSpeed;
}

SteveState Steve::getState() const
{
	return m_state;
}

int Steve::getArrowCooldown() const
{
	return m_arrowCooldown;
}

int Steve::getSwordSoundCooldown() const
{
	return m_swordSoundCooldown;
}

void Steve::setMaxSpeed(float newSpeed)
{
	m_maxSpeed = newSpeed;
}

void Steve::setState(SteveState state)
{
	m_state = state;
}

void Steve::setArrowCooldown(int cooldown)
{
	m_arrowCooldown = cooldown;
}

void Steve::setSwordSoundCooldown(int cooldown)
{
	m_swordSoundCooldown = cooldown;
}

void Steve::m_checkBounds()
{

	if (getTransform()->position.x > Config::SCREEN_WIDTH)
	{
		getTransform()->position.x = Config::SCREEN_WIDTH;
	}

	if (getTransform()->position.x < 0)
	{
		getTransform()->position.x = 0;
	}

	if (getTransform()->position.y > Config::SCREEN_HEIGHT)
	{
		getTransform()->position.y = Config::SCREEN_HEIGHT;
	}

	if (getTransform()->position.y < 0)
	{
		getTransform()->position.y = 0;
	}

}

void Steve::m_reset()
{
	getRigidBody()->isColliding = false;
	const int halfWidth = getWidth() * 0.5f;
	const auto xComponent = rand() % (640 - getWidth()) + halfWidth + 1;
	const auto yComponent = -getHeight();
	getTransform()->position = glm::vec2(xComponent, yComponent);
}

void Steve::m_buildAnimations()
{
	Animation idle = Animation();
	idle.name = "idle";
	for (int i = 0; i < 2; i++)
		idle.frames.push_back(getSpriteSheet()->getFrame("steve_idle" + std::to_string(i)));
	setAnimation(idle);

	Animation walk = Animation();
	walk.name = "walk";

	for (int i = 0; i < 12; i++)
		walk.frames.push_back(getSpriteSheet()->getFrame("steve_walk" + std::to_string(i)));
	setAnimation(walk);

	Animation attack = Animation();
	attack.name = "attack";
	
	for (int i = 0; i < 12; i++)
		attack.frames.push_back(getSpriteSheet()->getFrame("steve_attack" + std::to_string(i)));
	setAnimation(attack);

	Animation shoot = Animation();
	shoot.name = "shoot";

	for (int i = 0; i < 4; i++)
		shoot.frames.push_back(getSpriteSheet()->getFrame("steve_shoot" + std::to_string(i)));
	setAnimation(shoot);
}
