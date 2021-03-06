#include "LOSCondition.h"

LOSCondition::LOSCondition(bool LOS)
{
	setLOS(LOS);
	name = "LOS Condition";
}

LOSCondition::~LOSCondition() = default;

void LOSCondition::setLOS(bool state)
{
	data = m_hasLOS = state;
}

bool LOSCondition::Condition()
{
	return m_hasLOS;
}
