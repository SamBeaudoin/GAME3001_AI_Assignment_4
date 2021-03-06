#include "Pigman.h"
#include "Config.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "Renderer.h"
#include "Util.h"

Pigman::Pigman() : Enemy()
{
	TextureManager::Instance()->loadSpriteSheet("../Assets/sprites/Pigman_Animations.txt", "../Assets/sprites/Sprite_Sheet.png", "pigman");
	setSpriteSheet(TextureManager::Instance()->getSpriteSheet("pigman"));

	m_buildAnimations();

	setWidth(40);
	setHeight(40);

	m_health = HealthBar(4, getTransform()->position);
	m_hideCooldown = 0;

	m_decisionTree = new RangedCombatDecisionTree();
	m_decisionTree->setAgent(this);

	m_attackRange = 150;

	setType(PIGMAN);
	setState(PIGMAN_WALK);
	m_soundCooldown = (rand() % 301) + 300;
}

Pigman::~Pigman() = default;

void Pigman::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw piggy boi
	switch (m_state)
	{
	case PIGMAN_IDLE:
		TextureManager::Instance()->playAnimation("pigman", getAnimation("idle"), x, y, 0.0155f, getCurrentHeading(), 255, true);
		break;
	case PIGMAN_WALK:
		TextureManager::Instance()->playAnimation("pigman", getAnimation("walk"), x, y, 1.1f, getCurrentHeading(), 255, true);
		break;
	case PIGMAN_DEATH:
		TextureManager::Instance()->playAnimation("pigman", getAnimation("death"), x, y, 0.9f, getCurrentHeading(), 255, true);
		break;
	case PIGMAN_DAMAGED:
		TextureManager::Instance()->playAnimation("pigman", getAnimation("damage"), x, y, 0.5f, getCurrentHeading(), 255, true);
		break;
	case PIGMAN_ATTACK:
		TextureManager::Instance()->playAnimation("pigman", getAnimation("attack"), x, y, 0.1f, getCurrentHeading(), 255, true);
		break;
	}

	Enemy::draw();
}

void Pigman::update()
{
	if (m_cooldown > 0) {
		m_cooldown--;
		if (m_cooldown == 0)
			setState(PIGMAN_WALK);
	}

	if (m_hideCooldown > 0) {
		setIsHideCooldownRunning(true);
	}
	else
		setIsHideCooldownRunning(false);
	
	if (m_despawnTimer > 0) {
		setState(PIGMAN_DEATH);
		m_despawnTimer--;
	}
	else if (m_soundCooldown > 0) {
		m_soundCooldown--;
		if (m_soundCooldown == 0) {
			m_soundCooldown = (rand() % 301) + 300;
			SoundManager::Instance().playSound("pigmanIdle");
		}
	}

	//if (m_hideCooldown > 0) m_hideCooldown--;  // This is being called by UpdateHideCooldown()
	if (m_hideCooldown <= 0) { setIsHideCooldownRunning(false); }
	if (m_nuggCooldown > 0) { m_nuggCooldown--; }

	Enemy::update();
	m_decisionTree->Update();
}

void Pigman::clean()
{
	Enemy::clean();
}

PigmanState Pigman::getState() const
{
	return m_state;
}

int Pigman::getCooldown() const
{
	return m_cooldown;
}

int Pigman::getDespawnTimer() const
{
	return m_despawnTimer;
}

int Pigman::getHideCooldown() const
{
	return m_hideCooldown;
}

int Pigman::getNuggCooldown() const
{
	return m_nuggCooldown;
}

void Pigman::setState(PigmanState state)
{
	m_state = state;
}

void Pigman::startHideCooldown()
{
	m_hideCooldown = 100;
}

void Pigman::UpdateHideCooldown()
{
	m_hideCooldown--;
}

void Pigman::startNuggCooldown()
{
	m_nuggCooldown = 200;
}

void Pigman::resetCooldown()
{
	m_cooldown = 50;
}

void Pigman::StartDespawnTimer()
{
	m_despawnTimer = 30;
}

void Pigman::m_buildAnimations()
{
	Animation idle = Animation();
	idle.name = "idle";

	idle.frames.push_back(getSpriteSheet()->getFrame("pigman_idle0"));
	idle.frames.push_back(getSpriteSheet()->getFrame("pigman_idle1"));
	setAnimation(idle);

	Animation walk = Animation();
	walk.name = "walk";

	for (int i = 0; i < 12; i++)
		walk.frames.push_back(getSpriteSheet()->getFrame("pigman_walk" + std::to_string(i)));
	setAnimation(walk);

	Animation death = Animation();
	death.name = "death";

	for (int i = 0; i < 8; i++)
		death.frames.push_back(getSpriteSheet()->getFrame("pigman_death" + std::to_string(i)));
	setAnimation(death);

	Animation damage = Animation();
	damage.name = "damage";

	for (int i = 0; i < 2; i++)
		damage.frames.push_back(getSpriteSheet()->getFrame("pigman_damage" + std::to_string(i)));
	setAnimation(damage);

	Animation attack = Animation();
	attack.name = "attack";

	for (int i = 0; i < 6; i++)
		attack.frames.push_back(getSpriteSheet()->getFrame("pigman_attack" + std::to_string(i)));
	setAnimation(attack);
}