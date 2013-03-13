#ifndef __GAME_MESSAGES_IMPL_H__
#define __GAME_MESSAGES_IMPL_H__


#include "GameMessageImpl.h"

class TestMessage : public GameMessageBase {
	static const GameVersionType TEST_MESSAGE_VERSION = 1;
	wxDword m_testValue;
public:
	
	TestMessage() : GameMessageBase(GAME_MSG_TYPE_TEST, TEST_MESSAGE_VERSION), m_testValue(0) {}
	TestMessage(const TestMessage& msg) : 
	
	void SetTestValue(wxDword test);
	wxDword GetTestValue();
	
public:
	GameMessageType GetType();
	GameVersionType GetVersion();
	
	GameErrorCode Load(wxInputStream &istream);
	GameErrorCode Store(wxOutputStream &ostream);
	
	GameErrorCode CreateCopy(IGameMessage*& pMsgCopy);
	
};


#endif //__GAME_MESSAGES_IMPL_H__