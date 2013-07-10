#ifndef __GAME_CLIENT_ENGINE_H__
#define __GAME_CLIENT_ENGINE_H__



#include "../WorldCli/gflatworldcliimpl.h"
#include "../WorldSrv/gflatworldsrvimpl.h"


struct GameEngineSettings {
	wxDword m_screenWidth;
	wxDword m_screenHeight;
	wxChar* m_screenTitle;
	wxChar* m_worldName;
	
	GameEngineSettings() : 
		m_screenWidth(800),
		m_screenHeight(600),
		m_screenTitle(nullptr),
		m_worldName(nullptr) {}
}

/*! \brief Engine for control game states and processes
 * 
 * 	It is singleton and can be created only once.
 */
class GameClientEngine {
private:
	static GameClientEngine* m_pClientEngine;
private:
	GameLoggerPtr m_pLogger;
	GameEngineSettings m_settings;
	GameFlatWorldClient *m_pActualFlatWorldClient; 
	sf::RenderWindow *m_renderWindow;
	RefObjSmPtr<GameResourceHolder> *m_spResHolder;
	IGameSceneGenerator *m_pSceneGenerator;
	GameEntityFactory *m_pEntityFactory;
	bool m_isWindowCreated;
	bool m_isSettingLoaded;
	bool m_isInitialized;
private:
	GameClientEngine() : m_pActualFlatWorldServer(NULL),
					m_pActualFlatWorldClient(NULL),
					m_renderWindow(NULL),
					m_pEntityFactory(NULL),
					m_isWindowCreated(false),
					m_isSettingLoaded(false),
					m_isInitialized(false) {}
	GameErrorCode CreateWindow();
	GameErrorCode LoadSettings(wxChar* pFileName = NULL);

public:
	/*! \brief Destructor */
	~GameClietEngine() {}
	/*! \brief Initialize game engine
	 * 
	 * 	It loads settings, create window and initialize events.
	 * \return 
	 */
	GameErrorCode Initialize(GameLogger* pLogger = NULL);
	GameErrorCode MainLoop();
	
	GameErrorCode CreateTestingWorld();

// static methods
public:
	/*! \brief Create new engine
	 * 
	 *  If engine already exists it returns the existing one. Engine is also initialized in this method.
	 * \param[out] pEngine Returned GameClientEngine
	 * \param[in] pLogger Logger for initialization
	 * \retval FWG_NO_ERROR On success
	 * \retval Some errorcode when failed
	 */
	static GameErrorCode CreateEngine(GameClientEngine *&pEngine, GameLogger *pLogger = NULL);
	
	/*! \brief Get game engine
	 * \return Actual game engine (can be NULL)
	 */
	static GameClientEngine* GetEngine();

};

#endif //__GAME_CLIENT_ENGINE_H__
