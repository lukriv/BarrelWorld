/*! \brief 
 * 
 * 
 */
 
#ifndef __GAME_MESSAGE_H__
#define __GAME_MESSAGE_H__

#include <wx/stream.h>
#include <wx/datstrm.h>
#include "../GameSystem/gdefs.h"
#include "../GameSystem/gerror.h"
#include "gamedata.h"



static const GameAddrType GAME_ADDR_UNKNOWN = 0; //!< Unknown address
static const GameAddrType GAME_ADDR_SERVER = 1; //!< Unique address of server
static const GameAddrType GAME_ADDR_BROADCAST = wxINT32_MAX; //!< Address to send message to all clients


/*! \brief Game message type
 * 
 * This types are for control that the type of message is same. Every message implementation
 * should return static constant which is unique for every message type.
 * 
 * If callback is registered on GAME_MSG_TYPE_DEFAULT_ALL message type, it would be called when no callback 
 * on exact type is not registered.
 */
enum GameMessageType {
	GAME_MSG_TYPE_UNKNOWN = 0,
	
	GAME_MSG_TYPE_CLIENT_ID_REQUEST = 1,
	
	GAME_MSG_TYPE_TEST = 9999,
	
	GAME_MSG_TYPE_DEFAULT_ALL = wxINT32_MAX //!< Type for filtering rest of all types
};

/*! \brief Game message class
 * 
 * Message class has several unique values inside which is necessary for addressing this message to target.
 * 
 * Every message has source value. This values identify a sender. It has also optional source id which 
 * helps in sending answer message back to target. If SourceId is not set it has GAME_ADDR_UNKNOWN value.
 * 
 * Message can be addressed only on one target (and optional target id). This is only one exception - BROADCAST message.
 * 
 * Load and Store methods can serialize message to stream and vice versa.
 */
class IGameMessage {
public:
	
	virtual GameMessageType GetType() const = 0;
	virtual GameVersionType GetVersion() const = 0;
    
	virtual void SetSource (GameAddrType sourceCli) = 0;
	virtual GameAddrType GetSource() const = 0;
	
	virtual void SetSourceId (GameAddrType sourceId) = 0;
	virtual GameAddrType GetSourceId() const = 0;
	
	virtual void SetTarget (GameAddrType targetCli) = 0;
	virtual GameAddrType GetTarget() const = 0;
	
	virtual void SetTargetId(GameAddrType targetId) = 0;
	virtual GameAddrType GetTargetId() const = 0;
	
	virtual GameErrorCode SetMessage(const IGameData &data, GameMessageType msgType) = 0;
	virtual GameErrorCode GetMessage(IGameData &data) const = 0;
	
	virtual GameErrorCode Load(wxInputStream &istream) = 0;
	virtual GameErrorCode Store(wxOutputStream &ostream) = 0;

	virtual GameErrorCode CreateCopy(IGameMessage *&pMsgCopy) = 0;
	
	virtual ~IGameMessage() {};
	
};


#endif //__GAME_MESSAGE_H__