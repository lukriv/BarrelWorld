#ifndef __GAME_MESSAGE_IMPL_H__
#define __GAME_MESSAGE_IMPL_H__


#include "GameMessage.h"
#include <wx/mstream.h>
#include <wx/datstrm.h>

static const GameVersionType GameMsgVersion = 1;

class GameMessage : public IGameMessage {
	GameMessageType m_msgType;
	GameVersionType m_msgVer;
	
	GameAddrType m_source;
	GameAddrType m_sourceId;
	
	GameAddrType m_target;
	GameAddrType m_targetId;
	
	wxUint64 m_streamSize;
	wxMemoryOutputStream m_internalStream;


public:
	GameMessage(): m_msgType(GAME_MSG_TYPE_UNKNOWN),
					m_msgVer(GameMsgVersion),
					m_source(GAME_ADDR_UNKNOWN),
					m_sourceId(0),
					m_target(GAME_ADDR_UNKNOWN),
					m_targetId(0),
					m_streamSize(0) {}
																
	GameMessage(const GameMessage & msg) {
		m_msgType = msg.m_msgType;
		m_msgVer = msg.m_msgVer;
		m_source = msg.m_source;
		m_sourceId = msg.m_sourceId;
		m_target = msg.m_target;
		m_targetId = msg.m_targetId;
		m_streamSize = msg.m_streamSize;
		if(msg.m_internalStream.GetLength() > 0)
		{
			wxMemoryInputStream istream(msg.m_internalStream);
			m_internalStream.Write(istream);
		}
	}
	
	virtual ~GameMessage() {}
	
	virtual GameMessageType GetType() const {return m_msgType;}
	virtual GameVersionType GetVersion() const {return m_msgVer;}
    
	virtual void SetSource (GameAddrType sourceCli) {m_source = sourceCli;}
	virtual GameAddrType GetSource() const {return m_source;}
	
	virtual void SetSourceId (GameAddrType sourceId) {m_sourceId = sourceId;}
	virtual GameAddrType GetSourceId() const {return m_sourceId;}
	
	virtual void SetTarget (GameAddrType targetCli) {m_target = targetCli;}
	virtual GameAddrType GetTarget() const {return m_target;}
	
	virtual void SetTargetId(GameAddrType targetId) {m_targetId = targetId;}
	virtual GameAddrType GetTargetId() const {return m_targetId;}
	
	virtual GameErrorCode SetMessage(const IGameData &data, GameMessageType msgType);
	virtual GameErrorCode GetMessage(IGameData &data) const;
	
	virtual GameErrorCode Load(wxInputStream &istream);
	virtual GameErrorCode Store(wxOutputStream &ostream);
	
	virtual GameErrorCode CreateCopy(IGameMessage *&pMsgCopy);
	
protected:
	virtual GameErrorCode LoadHeader(wxDataInputStream &iDataStream);
	virtual GameErrorCode StoreHeader(wxDataOutputStream &oDataStream);
};

#endif //__GAME_MESSAGE_IMPL_H__
