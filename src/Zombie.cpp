#include "Zombie.h"
#include "Config.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "Renderer.h"
#include "Util.h"

Zombie::Zombie() : Enemy()
{
	TextureManager::Instance()->loadSpriteSheet("../Assets/sprites/Zombie_Animations.txt", "../Assets/sprites/Sprite_Sheet.png", "zombie");
	setSpriteSheet(TextureManager::Instance()->getSpriteSheet("zombie"));

	m_buildAnimations();

	setWidth(40);
	setHeight(40);

	m_health = HealthBar(4, getTransform()->position);

	m_decisionTree = new CloseCombatDecisionTree();
	m_decisionTree->setAgent(this);

	m_attackRange = 50;

	setType(ZOMBIE);
	setState(ZOMBIE_WALK);
	m_soundCooldown = (rand() % 301) + 300;
}

Zombie::~Zombie() = default;

void Zombie::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw le Zomboi
	switch (m_state)
	{
	case ZOMBIE_IDLE:
		TextureManager::Instance()->playAnimation("zombie", getAnimation("idle"), x, y, 0.0155f, getCurrentHeading(), 255, true);
		break;
	case ZOMBIE_WALK:
		TextureManager::Instance()->playAnimation("zombie", getAnimation("walk"), x, y, 1.1f, getCurrentHeading(), 255, true);
		break;
	case ZOMBIE_DEATH:
		TextureManager::Instance()->playAnimation("zombie", getAnimation("death"), x, y, 0.9f, getCurrentHeading(), 255, true);
		break;
	case ZOMBIE_DAMAGED:
		TextureManager::Instance()->playAnimation("zombie", getAnimation("damage"), x, y, 0.5f, getCurrentHeading(), 255, true);
		break;
	case ZOMBIE_ATTACK:
		TextureManager::Instance()->playAnimation("zombie", getAnimation("attack"), x, y, 1.0f, getCurrentHeading(), 255, true);
		break;
	}

	Enemy::draw();
}

void Zombie::update()
{
	if (m_cooldown > 0) {
		m_cooldown--;
		if (m_cooldown == 0)
			setState(ZOMBIE_WALK);
	}
	else if (m_despawnTimer > 0) {
		setState(ZOMBIE_DEATH);
		m_despawnTimer--;
	}
	else if (m_soundCooldown > 0) {
		m_soundCooldown--;
		if (m_soundCooldown == 0) {
			m_soundCooldown = (rand() % 301) + 300;
			SoundManager::Instance().playSound("zombieIdle" + std::to_string(rand() % 3));
		}
	}

	Enemy::update();
	m_decisionTree->Update();
}

void Zombie::clean()
{
	Enemy::clean();
}

ZombieState Zombie::getState() const
{
    return m_state;
}

int Zombie::getCooldown() const
{
	return m_cooldown;
}

int Zombie::getDespawnTimer() const
{
	return m_despawnTimer;
}

void Zombie::setState(ZombieState state)
{
	m_state = state;
}

void Zombie::resetCooldown()
{
	m_cooldown = 50;
}

void Zombie::StartDespawnTimer()
{
	m_despawnTimer = 30;
}

void Zombie::m_buildAnimations()
{
	Animation idle = Animation();
	idle.name = "idle";

	idle.frames.push_back(getSpriteSheet()->getFrame("zombie_idle0"));
	idle.frames.push_back(getSpriteSheet()->getFrame("zombie_idle1"));
	setAnimation(idle);

	Animation walk = Animation();
	walk.name = "walk";

	for (int i = 0; i < 12; i++)
		walk.frames.push_back(getSpriteSheet()->getFrame("zombie_walk" + std::to_string(i)));
	setAnimation(walk);

	Animation death = Animation();
	death.name = "death";

	for (int i = 0; i < 8; i++)
		death.frames.push_back(getSpriteSheet()->getFrame("zombie_death" + std::to_string(i)));
	setAnimation(death);

	Animation damage = Animation();
	damage.name = "damage";

	for (int i = 0; i < 2; i++)
		damage.frames.push_back(getSpriteSheet()->getFrame("zombie_damage" + std::to_string(i)));
	setAnimation(damage);

	Animation attack = Animation();
	attack.name = "attack";

	for (int i = 0; i < 11; i++)
		attack.frames.push_back(getSpriteSheet()->getFrame("zombie_attack" + std::to_string(i)));
	setAnimation(attack);
}