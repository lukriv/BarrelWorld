#include "state.h"



BW::State::State(StateHandler* pHandler, int32_t stateID) : m_pStateHandler(pHandler), m_stateId(stateID)
{
}

int32_t BW::State::GetStateId()
{
	return m_stateId;
}

BW::State::~State()
{
}

