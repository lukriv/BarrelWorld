#include "glogicbase.h"
#include "glogicsystem.h"


LogicBase::~LogicBase()
{
	if(m_pLogicSystem != nullptr)
	{
		m_pLogicSystem->RemoveLogicComp(this);
	}
}

LogicBase::LogicBase(GameLogicSystem* pLogicSystem) : m_pLogicSystem(pLogicSystem)
{
}
