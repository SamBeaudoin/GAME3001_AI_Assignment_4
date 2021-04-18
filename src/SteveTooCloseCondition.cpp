#include "SteveTooCloseCondition.h"

SteveTooCloseCondition::SteveTooCloseCondition(bool Too_close)
{
	setIsTooClose(Too_close);
	name = "Steve Too Close Condition";
}

SteveTooCloseCondition::~SteveTooCloseCondition() = default;

void SteveTooCloseCondition::setIsTooClose(bool state)
{
	data = m_isTooClose = state;
}

bool SteveTooCloseCondition::Condition()
{
	return m_isTooClose;
}
