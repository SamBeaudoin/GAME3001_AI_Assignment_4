#include "HideCooldownCondition.h"

HideCooldownCondition::HideCooldownCondition(bool within_radius)
{
	setIsCooldownRunning(within_radius);
	name = "Hide Cooldown Condition";
}

HideCooldownCondition::~HideCooldownCondition() = default;

void HideCooldownCondition::setIsCooldownRunning(bool state)
{
	data = m_isCooldownRunning = state;
}

bool HideCooldownCondition::Condition()
{
	return m_isCooldownRunning;
}
