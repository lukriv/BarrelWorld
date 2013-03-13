#include "gamemessages.h"

wxDword TestMessage::GetTestValue()
{
	return m_testValue;
}



GameErrorCode TestMessage::Load(wxInputStream& istream)
{
}

void TestMessage::SetTestValue(wxDword test)
{
	m_testValue = test;
}

GameErrorCode TestMessage::Store(wxOutputStream& ostream)
{
}


GameErrorCode TestMessage::CreateCopy(IGameMessage*& pMsgCopy)
{
	pMsgCopy = NULL;
	
	pMsgCopy = new (std::nothrow) TestMessage(*this);
	if (!pMsgCopy)
	{
		return FWG_E_MEMORY_ALLOCATION_ERROR;
	}
	
	return FWG_NO_ERROR;
}
