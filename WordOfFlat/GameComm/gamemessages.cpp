#include "gamemessages.h"
#include <wx/datstrm.h>

wxDword TestMessage::GetTestValue()
{
	return m_testValue;
}



GameErrorCode TestMessage::Load(wxInputStream& istream)
{
	wxDataInputStream iStream(istream);
	iStream.BigEndianOrdered(true);
	m_testValue = (wxDword) iStream.Read32();
	
	return FWG_NO_ERROR;
}

void TestMessage::SetTestValue(wxDword test)
{
	m_testValue = test;
}

GameErrorCode TestMessage::Store(wxOutputStream& ostream)
{
	wxDataOutputStream oStream(ostream);
	oStream.BigEndianOrdered(true);
	oStream.Write32((wxUint32) m_testValue);
	
	return FWG_NO_ERROR;
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
