#include "GameMessageImpl.h"
#include <wx/datstrm.h>

GameErrorCode GameMessageBase::LoadHeader(wxInputStream& istream)
{
	wxDataInputStream iStream(istream);
	iStream.BigEndianOrdered(true);
	m_msgType = (GameMessageType) iStream.Read32();
	m_msgVer = (GameVersionType) iStream.Read32();
	m_source = (GameAddrType) iStream.Read32();
	m_sourceId = (GameAddrType) iStream.Read32();
	m_target = (GameAddrType) iStream.Read32();
	m_targetId = (GameAddrType) iStream.Read32();
	
	return FWG_NO_ERROR;	
}

GameErrorCode GameMessageBase::StoreHeader(wxOutputStream& ostream)
{
	wxDataOutputStream oStream(ostream);
	oStream.BigEndianOrdered(true);
	oStream.Write32((wxUint32) m_msgType);
	oStream.Write32((wxUint32) m_msgVer);
	oStream.Write32((wxUint32) m_source);
	oStream.Write32((wxUint32) m_sourceId);
	oStream.Write32((wxUint32) m_target);
	oStream.Write32((wxUint32) m_targetId);
	
	return FWG_NO_ERROR;
}



