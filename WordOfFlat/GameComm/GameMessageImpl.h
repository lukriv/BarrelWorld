#ifndef __GAME_MESSAGE_IMPL_H__
#define __GAME_MESSAGE_IMPL_H__


#include "GameMessage.h"
#include <wx/datstrm.h>


class GameMessageBase : public IGameMessage {
	GameMessageType m_msgType;
	GameVersionType m_msgVer;
	
	GameAddrType m_source;
	GameAddrType m_sourceId;
	
	GameAddrType m_target;
	GameAddrType m_targetId;
	


public:
	GameMessageBase(GameMessageType type, GameVersionType ver): m_msgType(type),
																m_msgVer(ver),
																m_source(GAME_ADDR_UNKNOWN),
																m_sourceId(0),
																m_target(GAME_ADDR_UNKNOWN),
																m_targetId(0) {}
																
	GameMessageBase(const GameMessageBase & msg) {
		m_msgType = msg.m_msgType;
		m_msgVer = msg.m_msgVer;
		m_source = msg.m_source;
		m_sourceId = msg.m_sourceId;
		m_target = msg.m_target;
		m_targetId = msg.m_targetId;
	}
	
	virtual ~GameMessageBase() {}
	
	virtual GameMessageType GetType() {return m_msgType;}
	virtual GameVersionType GetVersion() {return m_msgVer;}
    
	virtual void SetSource (GameAddrType sourceCli) {m_source = sourceCli;}
	virtual GameAddrType GetSource() {return m_source;}
	
	virtual void SetSourceId (GameAddrType sourceId) {m_sourceId = sourceId;}
	virtual GameAddrType GetSourceId() {return m_sourceId;}
	
	virtual void SetTarget (GameAddrType targetCli) {m_target = targetCli;}
	virtual GameAddrType GetTarget() {return m_target;}
	
	virtual void SetTargetId(GameAddrType targetId) {m_targetId = targetId;}
	virtual GameAddrType GetTargetId() {return m_targetId;}
	
	
	GameErrorCode Load(wxInputStream &istream);
	GameErrorCode Store(wxOutputStream &ostream);

protected:
	virtual GameErrorCode LoadHeader(wxDataInputStream &iDataStream);
	virtual GameErrorCode StoreHeader(wxDataOutputStream &oDataStream);
	
	virtual GameErrorCode LoadInternal(wxDataInputStream &iDataStream) = 0;
	virtual GameErrorCode StoreInternal(wxDataOutputStream &oDataStream) = 0;	
};



#endif //__GAME_MESSAGE_IMPL_H__
