#ifndef __GAME_CLIENT_ENGINE_H__
#define __GAME_CLIENT_ENGINE_H__


#include "../GameSystem/refobjectsmptr.h"
#include "../GameResHold/gresholder.h"
#include "../GameResHold/gscenegen.h"
#include "../GameResHold/gentityfactory.h"
#include "../WorldCli/gflatworldcliimpl.h"
#include "../WorldSrv/gflatworldsrvimpl.h"


struct GameEngineSettings {
	wxDword m_screenWidth;
	wxDword m_screenHeight;
	wxString m_screenTitle;
	wxString m_worldName;
	
	GameEngineSettings() : 
		m_screenWidth(800),
		m_screenHeight(600) {}
};

/*! \brief Engine for control game states and processes
 * 
 * 	It is singleton and can be created only once.
 */
class GameClientEngine {
private:
	GameLoggerPtr m_pLogger;
	GameEngineSettings m_settings;
	GameFlatWorldClient *m_pActualFlatWorldClient; 
	sf::RenderWindow *m_renderWindow;
	RefObjSmPtr<GameResourceHolder> m_spResHolder;
	IGameSceneGenerator *m_pSceneGenerator;
	GameEntityFactory *m_pEntityFactory;
	bool m_isWindowCreated;
	bool m_isSettingLoaded;
	bool m_isInitialized;
private:
	GameErrorCode CreateWindow();
	GameErrorCode LoadSettings(wxChar* pFileName = NULL);

public:
	GameClientEngine() : m_pActualFlatWorldClient(NULL),
					m_renderWindow(NULL),
					m_pEntityFactory(NULL),
					m_isWindowCreated(false),
					m_isSettingLoaded(false),
					m_isInitialized(false) {}

	/*! \brief Destructor */
	~GameClientEngine() {}

	/*! \brief Initialize game engine
	 * 
	 * 	It loads settings, create window and initialize events.
	 * \return 
	 */
	GameErrorCode Initialize(GameLogger* pLogger = NULL);
	GameErrorCode MainLoop();
	
	GameErrorCode CreateTestingWorld();

};

#endif //__GAME_CLIENT_ENGINE_H__
