#include "../GameSystem/gstream.h"
#include "GameMessageImpl.h"

GameErrorCode GameMessage::LoadHeader(wxDataInputStream& iDataStream)
{
	m_msgType = (GameMessageType) iDataStream.Read32();
	m_msgVer = (GameVersionType) iDataStream.Read32();
	m_source = (GameAddrType) iDataStream.Read32();
	m_sourceId = (GameAddrType) iDataStream.Read32();
	m_target = (GameAddrType) iDataStream.Read32();
	m_targetId = (GameAddrType) iDataStream.Read32();
	
	// load stream size
	m_streamSize = iDataStream.Read64();
	return FWG_NO_ERROR;	
}

GameErrorCode GameMessage::StoreHeader(wxDataOutputStream& oDataStream)
{
	oDataStream.Write32((wxUint32) m_msgType);
	oDataStream.Write32((wxUint32) m_msgVer);
	oDataStream.Write32((wxUint32) m_source);
	oDataStream.Write32((wxUint32) m_sourceId);
	oDataStream.Write32((wxUint32) m_target);
	oDataStream.Write32((wxUint32) m_targetId);
	
	// store stream size
	oDataStream.Write64(m_streamSize);
	return FWG_NO_ERROR;
}




GameErrorCode GameMessage::Load(wxInputStream& istream)
{
	GameErrorCode resultErrcode = FWG_NO_ERROR;
	wxDataInputStream iDataStream(istream);
	iDataStream.BigEndianOrdered(true);
	{
		resultErrcode = LoadHeader(iDataStream);
		if (FWG_FAILED(resultErrcode))
		{
			return resultErrcode;
		}
	}

	
	// resize stream to 0
	m_internalStream.SeekO(0);
	m_internalStream.GetOutputStreamBuffer()->Truncate();
	
	if(m_streamSize > 0)
	{
		// reset stream state
		m_internalStream.Reset();
		
		// fill stream
		m_internalStream.Write(istream);
		
		resultErrcode = GameConvertWxStreamErr2GameErr(m_internalStream.GetLastError());
	}
	return resultErrcode;
}

GameErrorCode GameMessage::Store(wxOutputStream& ostream)
{
	wxDataOutputStream oDataStream(ostream);
	oDataStream.BigEndianOrdered(true);

	FWG_RETURN_FAIL(StoreHeader(oDataStream));
	
	if(m_streamSize > 0) {
		wxMemoryInputStream istream(m_internalStream);
		ostream.Write(istream);
	}
	
	return GameConvertWxStreamErr2GameErr(ostream.GetLastError());
}


GameErrorCode GameMessage::GetMessage(IGameData& data) const
{
	if(m_streamSize > 0)
	{
		wxMemoryInputStream iMemStream(m_internalStream);
		FWG_RETURN_FAIL(data.Load(iMemStream));
	}
	return FWG_NO_ERROR;
}

GameErrorCode GameMessage::SetMessage(const IGameData &data, GameMessageType msgType)
{
		//reset stream (resize to 0)
		m_internalStream.SeekO(0);
		m_internalStream.GetOutputStreamBuffer()->Truncate();
		
		//set type
		m_msgType = msgType;
		
		FWG_RETURN_FAIL(data.Store(m_internalStream));
		
		m_streamSize = (wxUint64) m_internalStream.GetLength();
		
		return FWG_NO_ERROR;
}

GameErrorCode GameMessage::CreateCopy(IGameMessage*& pMsgCopy)
{
	pMsgCopy = NULL;
	
	pMsgCopy = new (std::nothrow) GameMessage(*this);
	if (!pMsgCopy)
	{
		return FWG_E_MEMORY_ALLOCATION_ERROR;
	}
	
	return FWG_NO_ERROR;
}
