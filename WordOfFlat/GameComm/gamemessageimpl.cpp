#include "GameMessageImpl.h"
#include <wx/datstrm.h>

GameErrorCode GameMessageBase::LoadHeader(wxDataInputStream& iDataStream)
{
	m_msgType = (GameMessageType) iDataStream.Read32();
	m_msgVer = (GameVersionType) iDataStream.Read32();
	m_source = (GameAddrType) iDataStream.Read32();
	m_sourceId = (GameAddrType) iDataStream.Read32();
	m_target = (GameAddrType) iDataStream.Read32();
	m_targetId = (GameAddrType) iDataStream.Read32();
	
	return FWG_NO_ERROR;	
}

GameErrorCode GameMessageBase::StoreHeader(wxDataOutputStream& oDataStream)
{
	oDataStream.Write32((wxUint32) m_msgType);
	oDataStream.Write32((wxUint32) m_msgVer);
	oDataStream.Write32((wxUint32) m_source);
	oDataStream.Write32((wxUint32) m_sourceId);
	oDataStream.Write32((wxUint32) m_target);
	oDataStream.Write32((wxUint32) m_targetId);
	
	return FWG_NO_ERROR;
}




GameErrorCode GameMessageBase::Load(wxInputStream& istream)
{
	wxDataInputStream iDataStream(istream);
	iDataStream.BigEndianOrdered(true);
	{
		GameErrorCode resultErrcode = LoadHeader(iDataStream);
		if (FWG_FAILED(resultErrcode))
		{
			return resultErrcode;
		}
	}

	{
		GameErrorCode resultErrcode = LoadInternal(iDataStream);
		if (FWG_FAILED(resultErrcode))
		{
			return resultErrcode;
		}
	}
	
	return FWG_NO_ERROR;
}

GameErrorCode GameMessageBase::Store(wxOutputStream& ostream)
{
	wxDataOutputStream oDataStream(ostream);
	oDataStream.BigEndianOrdered(true);

	FWG_RETURN_FAIL(StoreHeader(oDataStream));
	FWG_RETURN_FAIL(StoreInternal(oDataStream));
	
	return FWG_NO_ERROR;
}


