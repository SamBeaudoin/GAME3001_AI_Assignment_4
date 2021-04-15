#include "HealthCheckCondition.h"

HealthCheckCondition::HealthCheckCondition(bool within_combat_range)
{
	setIsWithinHealthRange(within_combat_range);
	name = "Health Check Condition";
}

HealthCheckCondition::~HealthCheckCondition() = default;

void HealthCheckCondition::setIsWithinHealthRange(bool state)
{
	data = m_isHealthAboveThreshold = state;
}

bool HealthCheckCondition::Condition()
{
	return m_isHealthAboveThreshold;
}
