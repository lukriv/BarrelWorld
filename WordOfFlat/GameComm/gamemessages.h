#ifndef __GAME_MESSAGES_IMPL_H__
#define __GAME_MESSAGES_IMPL_H__




class TestMessage : public IGameMessage {
	wxDword m_testValue;
public:
	
	
	void SetTestValue(wxDword test);
	
public:
	GameMessageType GetType();
	GameVersionType GetVersion();
	
	GameErrorCode Load(wxInputStream &istream);
	GameErrorCode Store(wxOutputStream &ostream);
	
};


#endif //__GAME_MESSAGES_IMPL_H__