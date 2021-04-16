#include "WithinRangeCondition.h"

WithinRangeCondition::WithinRangeCondition(bool within_radius)
{
	setIsWithinRange(within_radius);
	name = "Range Condition";
}

WithinRangeCondition::~WithinRangeCondition() = default;

void WithinRangeCondition::setIsWithinRange(bool state)
{
	data = m_isWithinRange = state;
}

bool WithinRangeCondition::Condition()
{
	return m_isWithinRange;
}
