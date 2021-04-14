#include "RangedCombatCondition.h"

RangedCombatCondition::RangedCombatCondition(bool within_combat_range)
{
	setIsWithinCombatRange(within_combat_range);
	name = "Ranged Combat Condition";
}

RangedCombatCondition::~RangedCombatCondition()
= default;

void RangedCombatCondition::setIsWithinCombatRange(bool state)
{
	data = m_isWithinCombatRange = state;
}

bool RangedCombatCondition::Condition()
{
	return m_isWithinCombatRange;
}
