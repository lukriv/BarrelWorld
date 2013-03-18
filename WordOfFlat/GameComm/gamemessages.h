#ifndef __GAME_MESSAGES_IMPL_H__
#define __GAME_MESSAGES_IMPL_H__


#include "GameMessageImpl.h"

class TestMessage : public GameMessageBase {
	static const GameVersionType TEST_MESSAGE_VERSION = 1;
	wxDword m_testValue;
public:
	
	TestMessage() : GameMessageBase(GAME_MSG_TYPE_TEST, TEST_MESSAGE_VERSION), m_testValue(0) {}
	TestMessage(const TestMessage& msg) : GameMessageBase(msg) {
		m_testValue = msg.m_testValue;
	}
	
	void SetTestValue(wxDword test);
	wxDword GetTestValue();
	
public:

	
	GameErrorCode CreateCopy(IGameMessage*& pMsgCopy);
protected:
	virtual GameErrorCode LoadInternal(wxDataInputStream &istream);
	virtual GameErrorCode StoreInternal(wxDataOutputStream &ostream);	
};


#endif //__GAME_MESSAGES_IMPL_H__