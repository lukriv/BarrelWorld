#ifndef __GAME_DATA_IMPLEMENTATION_H__
#define __GAME_DATA_IMPLEMENTATION_H__


#include "gamedata.h"
#include "../GameSystem/gstream.h"


class GameDataBase: public IGameData {
	GameVersionType m_dataVer;
	
protected:
	virtual GameErrorCode LoadData(wxDataInputStream &iDataStream) = 0;
	virtual GameErrorCode StoreData(wxDataOutputStream &oDataStream) const = 0;
	
public:
	GameDataBase(GameVersionType dataVer) : m_dataVer(dataVer) {}
	virtual GameVersionType GetVersion() const {return m_dataVer;}
	
	virtual GameErrorCode Load(wxInputStream &istream) 
	{
		istream.Reset();
		wxDataInputStream iDataStream(istream);
		iDataStream.BigEndianOrdered(true);
	
		m_dataVer = (GameVersionType) iDataStream.Read32();
		
		FWG_RETURN_FAIL(LoadData(iDataStream));
		
		return GameConvertWxStreamErr2GameErr(istream.GetLastError());
	}

	virtual GameErrorCode Store(wxOutputStream &ostream) const
	{	
		ostream.Reset();
		wxDataOutputStream oDataStream(ostream);
		oDataStream.BigEndianOrdered(true);
		
		oDataStream.Write32((wxUint32) m_dataVer);
		
		FWG_RETURN_FAIL(StoreData(oDataStream));
		return GameConvertWxStreamErr2GameErr(ostream.GetLastError());	
	}
	
	
};





#endif //__GAME_DATA_IMPLEMENTATION_H__